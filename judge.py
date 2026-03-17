import os
import argparse
import subprocess
import time
import random
import sys

# maxsize_t = 1000
# timeout = 20
# length = 1000
# n = 100
# caseNum = 0

def generate_big(length, zero=True):
    if zero:
        return ('-' if random.randint(0, 1) == 1 else '') + ''.join(random.choices('0123456789', k=random.randint(1, length)))
    else:
        return ('-' if random.randint(0, 1) == 1 else '') + ''.join(random.choices('123456789', k=1) + random.choices('0123456789', k=random.randint(0, length - 1)))

def generate_small(zero=True):
    if zero:
        return (random.randint(0, (1 << 63) - 1) if random.randint(0, 1) == 0 else -random.randint(0, (1 << 63)))
    else:
        return (random.randint(1, (1 << 63) - 1) if random.randint(0, 1) == 0 else -random.randint(1, (1 << 63)))

def generate_test_cases(case_num, filename, n, length, base_dir="test_cases"):
    # test case格式：
    # 1. 第一行输入一个整数n和一个测试方法case_num，表示测试数量和测试方式
    # 2. 接下来n行，每行输入两个整数a和b，表示测试数据
    #
    # 输出格式：
    # 对于每个测试数据，输出一行结果，表示运算结果。
    if case_num == 1: # 大整数+小整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_small()
                f.write(f"{a} {b}\n")
    elif case_num == 2: # 大整数+大整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_big(length)
                f.write(f"{a} {b}\n")
    elif case_num == 3: # 大整数-小整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_small()
                f.write(f"{a} {b}\n")
    elif case_num == 4: # 大整数-大整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_big(length)
                f.write(f"{a} {b}\n")
    elif case_num == 5: # 大整数*小整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_small()
                f.write(f"{a} {b}\n")
    elif case_num == 6: # 大整数*大整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_big(length)
                f.write(f"{a} {b}\n")
    elif case_num == 7: # 大整数/小整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_small(False)
                f.write(f"{a} {b}\n")
    elif case_num == 8: # 大整数/大整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_big(length, False)
                f.write(f"{a} {b}\n")
    elif case_num == 9: # 大整数%小整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_small(False)
                f.write(f"{a} {b}\n")
    elif case_num == 10: # 大整数%大整数
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = generate_big(length, False)
                f.write(f"{a} {b}\n")
    elif case_num == 11: # 大整数<<小整数（0-1000000)
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = random.randint(0, maxsize_t)
                f.write(f"{a} {b}\n")
    elif case_num == 12: # 大整数>>小整数（0-1000000)
        with open(os.path.join(base_dir, filename), 'w') as f:
            f.write(f"{n} {case_num}\n")
            for i in range(n):
                a = generate_big(length)
                b = random.randint(0, maxsize_t)
                f.write(f"{a} {b}\n")
    else:
        print("Invalid case_num.")

def judge_all(executable, test_cases_dir="test_cases", results_dir="results"):
    if not os.path.exists(results_dir):
        os.makedirs(results_dir)
    
    # for filename in sorted(os.listdir(test_cases_dir)):
    #     if filename.endswith(".in"):
    for case_num in range(1, 13):
        if caseNum != 0 and case_num != caseNum:
            continue
        print(f"Running test case for case_num={case_num}...")
        filename = f"case_{complement(case_num, 2)}.in"
        input_path = os.path.join(test_cases_dir, filename)
        output_path = os.path.join(results_dir, filename.replace(".in", ".out"))
        with open(input_path, 'r') as infile, open(output_path, 'w') as outfile:
            start_time = time.time()
            subprocess.run([executable], stdin=infile, stdout=outfile, timeout=timeout)
            end_time = time.time()
            print(f"Test case {filename} executed in {end_time - start_time:.2f} seconds.")

def check_results(test_cases_dir="test_cases", results_dir="results"):
    # for filename in sorted(os.listdir(test_cases_dir)):
    #     if filename.endswith(".in"):
    for case_num in range(1, 13):
        if caseNum != 0 and case_num != caseNum:
            continue
        print(f"Checking results for case_num={case_num}...")
        filename = f"case_{complement(case_num, 2)}.in"
        input_path = os.path.join(test_cases_dir, filename)
        output_path = os.path.join(results_dir, filename.replace(".in", ".out"))
        with open(input_path, 'r') as infile, open(output_path, 'r') as outfile:
            n, case_num = map(int, infile.readline().strip().split())
            for i in range(n):
                a, b = infile.readline().strip().split()
                a = int(a)
                b = int(b)
                if case_num == 1: # 大整数+小整数
                    expected = a + b
                elif case_num == 2: # 大整数+大整数
                    expected = a + b
                elif case_num == 3: # 大整数-小整数
                    expected = a - b
                elif case_num == 4: # 大整数-大整数
                    expected = a - b
                elif case_num == 5: # 大整数*小整数
                    expected = a * b
                elif case_num == 6: # 大整数*大整数
                    expected = a * b
                elif case_num == 7: # 大整数/小整数
                    # expected = a // b
                    # 向零取整
                    expected = (a // b if a * b >= 0 else -(-a // b))
                elif case_num == 8: # 大整数/大整数
                    # expected = a // b
                    expected = (a // b if a * b >= 0 else -(-a // b))
                elif case_num == 9: # 大整数%小整数
                    expected = a % b
                elif case_num == 10: # 大整数%大整数
                    expected = a % b
                elif case_num == 11: # 大整数<<小整数（0-1000000)
                    expected = (a << b if a > 0 else -((-a) << b))
                elif case_num == 12: # 大整数>>小整数（0-1000000)
                    expected = (a >> b if a > 0 else -((-a) >> b))
                
                result = outfile.readline().strip()
                if result == "":
                    print(f"Test case {filename} empty, skip.")
                    break
                if result != str(expected):
                    print(f"Test case {filename} failed on line {i+1}: expected {expected}, got {result}.")
                    break
            else:
                print(f"Test case {filename} passed.")

def complement(x: int, l: int) -> str:
    s = str(x)
    return '0' * (l - len(s)) + s

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Judge for SuperLong")
    parser.add_argument("--length", type=int, default=1000, help="Length of big integers")
    parser.add_argument("--timeout", type=int, default=20, help="Timeout for each test case in seconds")
    parser.add_argument("--maxsize", type=int, default=1000, help="Maximum shift size for shift operations")
    parser.add_argument("--n", type=int, default=100, help="Number of test cases per case_num")
    parser.add_argument("--case_num", type=int, default=0, help="Test case number (0-12), 0 for all")
    args = parser.parse_args()
    
    length = args.length
    timeout = args.timeout
    maxsize_t = args.maxsize
    n = args.n
    caseNum = args.case_num
    
    sys.set_int_max_str_digits(length * 2 + 100) # 设置整数最大字符串长度，避免超出默认限制
    
    print(f"Settings: length={length}, timeout={timeout}, maxsize={maxsize_t}, n={n}, caseNum={caseNum}")
    
    print("Compiling...")
    
    subprocess.run(
        ["g++", "-std=c++17", "SuperLong.cpp", "main.cpp", "-o", "build/superlong"],
        check=True
    )
    print("Compilation finished.")
    
    executable = "./build/superlong"
    for case_num in range(1, 13):
        if caseNum != 0 and case_num != caseNum:
            continue
        print(f"Generating test cases for case_num={case_num}...")
        generate_test_cases(case_num, f"case_{complement(case_num, 2)}.in", n, length)
    
    print("Running test cases...")
    judge_all(executable)
    print("Checking results...")
    check_results()