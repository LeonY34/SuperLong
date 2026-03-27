import subprocess
import os

def compare(filename1, filename2):
    with open(filename1, 'r') as f1, open(filename2, 'r') as f2:
        l1 = f1.read().strip().split()
        l2 = f2.read().strip().split()
        for i in range(min(len(l1), len(l2))):
            if l1[i] != l2[i]:
                return f"Token {i + 1} different!"
        if len(l1) != len(l2):
            return "Different length!"
        return "good"
    return "bad"

if __name__ == "__main__":
    if not os.path.exists("test"):
        os.mkdir("test")
    t = 100
    for i in range(t):
        subprocess.run(
            "./build/P4721_datamaker > test/in.txt",
            shell=True,
            check=True,
        )
        subprocess.run(
            "./build/P4721_naive < test/in.txt > test/out1.txt",
            shell=True,
            check=True,
        )
        subprocess.run(
            "./build/P4721_NTT < test/in.txt > test/out2.txt",
            shell=True,
            check=True,
        )
        res = compare("test/out1.txt", "test/out2.txt")
        if res != "good":
            print(res)
            break
    else:
        print(f"good, {t} testcases.")