import os
import subprocess
import argparse
import time

def compile(filename, base_dir):
    print(f"Compiling {" ".join(['/opt/homebrew/bin/g++-14', f"{filename}.cpp", '-o', f"build/{filename}"])}...")
    subprocess.run(
        " ".join(['/opt/homebrew/bin/g++-14', f"{filename}.cpp", '-o', f"build/{filename}"]),
        shell=True,
        cwd=base_dir
    )
    
def run(filename, base_dir, In=None, Out=None):
    command = [f"./{filename}"]
    if In != None: command += ['<', In]
    if Out != None: command += ['>', Out]
    # print(f"running command {" ".join(command)}...")
    subprocess.run(
        " ".join(command),
        shell=True,
        cwd=base_dir,
        check=True
    )

def benchmark(base_dir, rep, m1="build/1299", md="build/1299_datamaker"):
    fin = "judge/in.txt"
    o1 = "judge/out1.txt"
    o2 = "judge/out2.txt"
    tt = 0.0
    for i in range(rep):
        run(md, base_dir, Out=fin)
        st = time.perf_counter()
        run(m1, base_dir, In=fin, Out=o1)
        ed = time.perf_counter()
        tt += ed - st
    return tt / rep

if __name__ == "__main__":
    base_dir = os.path.dirname(os.path.abspath(__file__))
    subprocess.run(" ".join(['mkdir', '-p', 'judge']), shell=True, cwd=base_dir)
    compile("1299_multiset", base_dir)
    compile("1299_datamaker", base_dir)
    parser = argparse.ArgumentParser(description="Closest pair profiler.")
    parser.add_argument("--warmup", type=int, help="warm up times", default=3)
    parser.add_argument("--repeat", type=int, help="repeat times", default=10)
    args = parser.parse_args()
    warmup = args.warmup
    repeat = args.repeat
    benchmark(base_dir, rep=warmup)
    print("warmuped, start benchmark...")
    print(f"avg time: {benchmark(base_dir, rep=repeat):.6f} s")
    
    