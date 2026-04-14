import os
import subprocess

def judge(input_file: str, method_file: str):
    with open(input_file, 'r') as f:
        n = int(f.readline().strip())
        a = [0] + list(map(int, f.readline().strip().split()))
    
    with open(method_file, 'r') as f:
        m = int(f.readline().strip())
        for i in range(m):
            line = list(map(int, f.readline().strip().split()))
            k = line[0]
            line = line[1:]
            if k * 2 != len(line):
                return f"Invalid output format"
            if k <= 0:
                return "Unnecessary swap"
            mp = {}
            for j in range(0, k * 2, 2):
                a[line[j]], a[line[j+1]] = a[line[j+1]], a[line[j]]
                if line[j] in mp or line[j+1] in mp:
                    return "Duplicate swap indices"
                mp[line[j]] = True
                mp[line[j+1]] = True
            
        for i in range(1, n):
            if a[i] != i:
                return f"Incorrect output, swapped array: {a[1:]}"
    return "Accepted"

if __name__ == "__main__":
    base_dir = os.path.dirname(os.path.abspath(__file__))
    os.makedirs(os.path.join(base_dir, "judge"), exist_ok=True)
    # input_file = "judge/input.txt"
    # method_file = "judge/method.txt"
    input_file = os.path.join(base_dir, "judge/input.txt")
    method_file = os.path.join(base_dir, "judge/method.txt")
    for i in range(100):
        print(f"Test case {i+1}:")
        subprocess.run(os.path.join(base_dir, "build/1296_datamaker"), stdout=open(input_file, "w"), shell=True, cwd=base_dir)
        subprocess.run(os.path.join(base_dir, "./build/1296"), stdin=open(input_file, "r"), stdout=open(method_file, "w"), shell=True, cwd=base_dir)
        result = judge(input_file, method_file)
        print(result)
        if result != "Accepted":
            break
            