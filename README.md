# SuperLong

基于 C++17 的高精度整数（Big Integer）实现，支持有符号任意精度整数的常见算术与位移运算。

当前已实现并可在主程序中测试的运算：

- 加法：大整数 + 小整数、大整数 + 大整数
- 减法：大整数 - 小整数、大整数 - 大整数
- 乘法：大整数 * 小整数、大整数 * 大整数
- 除法：大整数 / 小整数、大整数 / 大整数
- 取模：大整数 % 小整数、大整数 % 大整数
- 位移：大整数 << x、大整数 >> x

## 项目结构

- `SuperLong.hpp`：类声明与接口定义
- `SuperLong.cpp`：核心实现（加减乘除、位移、字符串转换等）
- `main.cpp`：命令行评测入口（读取 case 并输出结果）
- `judge.py`：随机数据生成 + 调用可执行文件 + 与 Python 大整数结果对拍
- `benchmark.cpp`：乘法性能测试程序
- `run_benchmark.py`：编译并运行 benchmark，结果写入 CSV
- `plot.py`：批量 benchmark 并绘制 log-log 曲线
- `test_cases/`：测试输入
- `results/`：程序输出
- `benchmarks/`：benchmark CSV 与图像输出

## 环境要求

- macOS / Linux
- C++17 编译器（如 g++）
- Python 3.8+
- `matplotlib`（仅在运行 `plot.py` 时需要）

## 快速开始

### 1. 编译并运行 C++ 主程序

```bash
mkdir -p build
g++ -std=c++17 main.cpp SuperLong.cpp -O2 -o build/superlong
./build/superlong
```

### 2. 输入格式

程序输入第一行为：

- `n case_num`

其中：

- `n`：测试条数
- `case_num`：操作类型（1~12）

后续输入 `n` 行，每行两个参数（或一个大整数 + 一个位移量）。

`case_num` 含义如下：

- `1`：大整数 + 小整数
- `2`：大整数 + 大整数
- `3`：大整数 - 小整数
- `4`：大整数 - 大整数
- `5`：大整数 * 小整数
- `6`：大整数 * 大整数
- `7`：大整数 / 小整数
- `8`：大整数 / 大整数
- `9`：大整数 % 小整数
- `10`：大整数 % 大整数
- `11`：大整数 << x
- `12`：大整数 >> x

示例输入：

```text
3 2
12345678901234567890 9876543210
-10 7
99999999999999999999 1
```

## 正确性测试（judge）

使用 `judge.py` 自动完成以下流程：

1. 编译 `build/superlong`
2. 生成随机测试
3. 执行程序得到输出
4. 用 Python 大整数进行结果校验

运行示例：

```bash
python3 judge.py --n 200 --case_num 0 --length 1000 --timeout 20 --maxsize 1000
```

参数说明：

- `--n`：每个 case 的数据条数
- `--case_num`：只测某一个 case（1~12），`0` 表示全测
- `--length`：随机大整数最大十进制位数
- `--timeout`：每个 case 的执行超时时间（秒）
- `--maxsize`：位移测试（case 11/12）的最大位移量

## 性能测试（benchmark）

### 单次运行 benchmark

```bash
python3 run_benchmark.py --mul-method 1 --warmup 5 --repeat 100 --maxbit 10000 --timeout 300
```

参数说明：

- `--mul-method`：乘法方法，`-1` 表示依次跑 0/1/2
- `--warmup`：预热轮数
- `--repeat`：计时轮数
- `--maxbit`：随机数据最大 bit 长度
- `--timeout`：单次 benchmark 超时（秒）

结果会写入 `benchmarks/mul_bench_*.csv`。

### 批量扫点并画图

```bash
python3 plot.py --warmup 25 --repeat 200 --timeout 300
```

输出为 `benchmarks/` 下的 PNG 图，横纵轴分别为 `log(maxbit)` 与 `log(avg_us)`。

## 实现说明

- 内部使用分块数组存储高精度整数
- 支持字符串输入输出（可处理超长十进制）
- 除法核心采用 Knuth 思路实现
- 乘法目前包含：
	- 方法 `0`：朴素乘法
	- 方法 `1`：分治乘法
	- 方法 `2`：预留接口（尚未完整实现）

主程序 `main.cpp` 默认设置为：

```cpp
SuperLong::setMulMethod(1);
```

即默认使用分治乘法。

## 运算语义说明

- 除法 `/`：按“向零截断”语义处理
- 取模 `%`：与 Python 语义保持一致（余数符号与除数一致）
- 位移 `<<`、`>>`：按符号位与数值分离策略处理（非补码实现）

## 后续计划

- 完成 FFT 乘法实现
- 进一步优化除法/取模性能
- 增加更多边界测试与性能对比数据