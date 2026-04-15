import os
import subprocess
import random
import util

base_dir = os.path.dirname(os.path.abspath(__file__))

def make_data(n: int, processor: str, in_file: str, out_file: str) -> list[int]:
    l = []
    B = 100000
    maxn = B // n
    for i in range(n):
        l.append(random.randint(-maxn, maxn))
    with open(os.path.join(base_dir, in_file), 'w') as f:
        f.write(f"{n}\n")
        for i in l:
            f.write(f"{i} ")
        f.write('\n')
    util.run(processor, in_file, out_file)
    return l

def judge(std_ls: list[int], processor: str, ans_file: str, tmp_file: str, in_file: str, out_file: str) -> bool:
    with open(os.path.join(base_dir, ans_file), 'r') as f:
        s = f.readline().strip()
        if s == "NO":
            return False
        l = list(map(int, f.readline().strip().split()))
    with open(os.path.join(base_dir, in_file), 'w') as f:
        f.write(f"{len(l)}\n")
        for i in l:
            f.write(f"{i} ")
        f.write('\n')
    util.run(processor, in_file, tmp_file)
    with open(os.path.join(base_dir, tmp_file), 'r') as f1, open(os.path.join(base_dir, out_file), 'r') as f2:
        f1.readline(), f2.readline()
        l1 = list(map(int, f1.readline().strip().split()))
        l2 = list(map(int, f2.readline().strip().split()))
        return sorted(l1) == sorted(l2)
        

def run(processor: str, test: str, in_file: str, out_file: str, ans_file: str, tmp_file: str) -> bool:
    n = random.randint(1, 15)
    l = make_data(n, processor, in_file, out_file)
    util.run(test, out_file, ans_file)
    return judge(l, processor, ans_file, tmp_file, in_file, out_file)

def main():
    in_file = "judge/din.txt"
    out_file = "judge/dtest.txt"
    ans_file = "judge/out.txt"
    tmp_file = "judge/tmp.txt"
    
    processor = "1303_processor"
    test = "1303"
    util.compile(processor)
    util.compile(test)
    for i in range(100):
        print(f"running testcase {i + 1}...")
        if not run(processor, test, in_file, out_file, ans_file, tmp_file):
            print("WA")
            break
        else: print("AC")
    else:
        print("ALL good")

    
if __name__ == "__main__":
    main()