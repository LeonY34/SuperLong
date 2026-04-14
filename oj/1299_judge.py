import os
import subprocess

def compare_files(filename1, filename2):
    with open(filename1, 'r') as f1, open(filename2, 'r') as f2:
        return f1.read().strip() == f2.read().strip()

def compile(filename, base_dir):
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
        cwd=base_dir
    )

def judge(base_dir, m1="build/1299", m2="build/1299_dp", md="build/1299_datamaker"):
    fin = "judge/in.txt"
    o1 = "judge/out1.txt"
    o2 = "judge/out2.txt"
    run(md, base_dir, Out=fin)
    run(m1, base_dir, In=fin, Out=o1)
    run(m2, base_dir, In=fin, Out=o2)
    return compare_files(o1, o2)

if __name__ == "__main__":
    base_dir = os.path.dirname(os.path.abspath(__file__))
    subprocess.run(" ".join(['mkdir', '-p', 'judge']), shell=True, cwd=base_dir)
    compile("1299_multiset", base_dir)
    compile("1299", base_dir)
    compile("1299_datamaker", base_dir)
    for i in range(10000):
        print(f"judging {i + 1} testcase...")
        if not judge(base_dir): 
            print("WA")
            break
        print("OK")
    else:
        print("ALL OK")