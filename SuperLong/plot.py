import argparse
import csv
import math
import subprocess
import sys
from pathlib import Path

import matplotlib.pyplot as plt


SUPPORTED_METHODS = (0, 1)


def validate_method(mul_method: int) -> int:
	if mul_method not in SUPPORTED_METHODS:
		raise ValueError(f"--mul-method must be one of {', '.join(map(str, SUPPORTED_METHODS))}")
	return mul_method


def run_single_benchmark(root: Path, mul_method: int, maxbit: int, warmup: int, repeat: int, timeout: int) -> bool:
	"""
	Run one benchmark point. Returns True if successful, False if this method should stop.
	"""
	try:
		print(f"[mul_method={mul_method}] Running benchmark with maxbit={maxbit}...")
		cmd = [
			sys.executable,
			"run_benchmark.py",
			"--mul-method",
			str(mul_method),
			"--warmup",
			str(warmup),
			"--repeat",
			str(repeat),
			"--maxbit",
			str(maxbit),
			"--timeout",
			str(timeout),
		]
		result = subprocess.run(
			cmd,
			cwd=root,
			check=True,
			capture_output=True,
			text=True,
			timeout=timeout + 60,
		)
		if result.stdout:
			print(result.stdout.strip())
		return True
	except subprocess.TimeoutExpired:
		print(f"[mul_method={mul_method}] Benchmark timed out at maxbit={maxbit}. Stopping this method.")
		return False
	except subprocess.CalledProcessError as e:
		print(f"[mul_method={mul_method}] Benchmark failed at maxbit={maxbit}. Stopping this method.")
		if e.stderr:
			print(e.stderr.strip())
		return False


def run_benchmark_sweep(methods, warmup: int = 25, repeat: int = 200, timeout: int = 300):
	"""
	Run benchmarks from maxbit=20, doubling each round for each active method.
	When multiple methods are selected, stop only after all methods have stopped.
	"""
	root = Path(__file__).resolve().parent
	active = {method: 20 for method in methods}

	while active:
		for method in list(active.keys()):
			ok = run_single_benchmark(root, method, active[method], warmup, repeat, timeout)
			if ok:
				active[method] = int(active[method] * 2)
			else:
				del active[method]


def load_points(csv_path: Path):
	if not csv_path.exists():
		raise FileNotFoundError(f"CSV file not found: {csv_path}")

	points = []
	with csv_path.open("r", newline="") as f:
		reader = csv.DictReader(f)
		required = {"mul_method", "maxbit", "avg_us"}
		if not required.issubset(set(reader.fieldnames or [])):
			raise ValueError(
				f"Invalid CSV header in {csv_path}. "
				"Expected columns: mul_method,maxbit,avg_us"
			)

		for row in reader:
			maxbit = int(row["maxbit"])
			avg_us = float(row["avg_us"])
			if maxbit <= 0 or avg_us <= 0:
				continue
			points.append((maxbit, avg_us))

	if not points:
		raise ValueError(f"No valid data points found in: {csv_path}")

	# Sort by maxbit so the curve is monotonic along x-axis.
	points.sort(key=lambda item: item[0])
	return points


def draw_plot(method_points, output_path: Path):
	plt.figure(figsize=(8, 5))

	for method, points in method_points.items():
		x_values = [math.log(p[0]) for p in points]
		y_values = [math.log(p[1]) for p in points]
		plt.plot(x_values, y_values, marker="o", linewidth=1.8, label=f"mul_method={method}")

	plt.xlabel("log(maxbit)")
	plt.ylabel("log(avg_us)")
	plt.title("Multiplication Benchmark")
	if len(method_points) > 1:
		plt.legend()
	plt.grid(True, linestyle="--", alpha=0.35)
	plt.tight_layout()

	output_path.parent.mkdir(parents=True, exist_ok=True)
	plt.savefig(output_path, dpi=160)
	plt.close()


def main():
	parser = argparse.ArgumentParser(description="Benchmark and plot log(avg_us) vs log(maxbit).")
	parser.add_argument(
		"--mul-method",
		type=int,
		default=None,
		help="Multiplication method. Omit to run all supported methods.",
	)
	parser.add_argument("--warmup", type=int, default=25, help="Warm-up rounds for benchmark")
	parser.add_argument("--repeat", type=int, default=200, help="Timed rounds for benchmark")
	parser.add_argument("--timeout", type=int, default=300, help="Timeout in seconds for each benchmark")
	args = parser.parse_args()

	if args.mul_method is None:
		methods = list(SUPPORTED_METHODS)
	else:
		methods = [validate_method(args.mul_method)]

	print(f"Starting benchmark sweep for methods={methods}...")
	run_benchmark_sweep(methods, warmup=args.warmup, repeat=args.repeat, timeout=args.timeout)

	root = Path(__file__).resolve().parent
	method_points = {}
	for method in methods:
		csv_path = root / "benchmarks" / f"mul_bench_{method}.csv"
		method_points[method] = load_points(csv_path)

	if len(methods) == 1:
		output_path = root / "benchmarks" / f"mul_plot_{methods[0]}.png"
	else:
		output_path = root / "benchmarks" / "mul_plot_all.png"

	draw_plot(method_points, output_path)
	print(f"Saved plot to: {output_path}")


if __name__ == "__main__":
	main()
