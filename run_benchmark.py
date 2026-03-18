import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path


def run_cmd(cmd, cwd, timeout=None):
	subprocess.run(cmd, cwd=cwd, check=True, timeout=timeout)


def run_cmd_capture(cmd, cwd, timeout=None):
	return subprocess.run(cmd, cwd=cwd, check=True, capture_output=True, text=True, timeout=timeout)


def parse_avg_us(output):
	m = re.search(r"avg_us=([0-9]+(?:\.[0-9]+)?)", output)
	if not m:
		raise ValueError(f"Failed to parse avg_us from output: {output}")
	return float(m.group(1))


def append_csv(csv_path, row):
	need_header = not csv_path.exists()
	with csv_path.open("a", newline="") as f:
		writer = csv.writer(f)
		if need_header:
			writer.writerow(["mul_method", "warmup", "maxbit", "repeat", "avg_us"])
		writer.writerow(row)


def methods_from_arg(mul_method):
	if mul_method == -1:
		return [0, 1, 2]
	if mul_method not in (0, 1, 2):
		raise ValueError("--mul-method must be one of -1, 0, 1, 2")
	return [mul_method]


def main():
	parser = argparse.ArgumentParser(description="Run correctness judge then multiplication benchmark.")
	parser.add_argument("--warmup", type=int, default=5, help="Warm-up rounds for benchmark")
	parser.add_argument("--repeat", type=int, default=100, help="Timed rounds for benchmark")
	parser.add_argument("--maxbit", type=int, default=10000, help="Maximum random bit length")
	parser.add_argument("--mul-method", type=int, default=-1, help="-1 for all methods, or 0/1/2")
	parser.add_argument("--timeout", type=int, default=300, help="Timeout in seconds for each benchmark (default: 300)")
	args = parser.parse_args()

	if args.warmup < 0 or args.repeat <= 0 or args.maxbit <= 0:
		raise ValueError("--warmup must be >= 0, --repeat and --maxbit must be > 0")

	root = Path(__file__).resolve().parent
	build_dir = root / "build"
	bench_dir = root / "benchmarks"
	build_dir.mkdir(exist_ok=True)
	bench_dir.mkdir(exist_ok=True)

	methods = methods_from_arg(args.mul_method)

	for method in methods:
		print(f"[method={method}] Running correctness judge...")
		run_cmd([sys.executable, "judge.py"], cwd=root, timeout=args.timeout)

		exe_path = build_dir / f"benchmark_mul_{method}"
		compile_cmd = [
			"g++",
			"-std=c++17",
			"-O2",
			"SuperLong.cpp",
			"benchmark.cpp",
			"-o",
			str(exe_path),
			f"-DMUL_METHOD={method}",
			f"-DWARM_UP={args.warmup}",
			f"-DREPEAT={args.repeat}",
			f"-DMAX_BIT={args.maxbit}",
		]
		print(f"[method={method}] Compiling benchmark...")
		run_cmd(compile_cmd, cwd=root, timeout=args.timeout)

		print(f"[method={method}] Running benchmark...")
		result = run_cmd_capture([str(exe_path)], cwd=root, timeout=args.timeout)
		stdout = result.stdout.strip()
		if stdout:
			print(stdout)
		avg_us = parse_avg_us(stdout)

		csv_path = bench_dir / f"mul_bench_{method}.csv"
		append_csv(csv_path, [method, args.warmup, args.maxbit, args.repeat, f"{avg_us:.3f}"])
		print(f"[method={method}] Saved to {csv_path}")


if __name__ == "__main__":
	main()
