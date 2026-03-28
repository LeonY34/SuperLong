# FFT学习

P开头的题目来自[luogu.com.cn](https://www.luogu.com.cn)

## Introduction

FFT用于解决离散卷积问题。比如说已知

$$\begin{aligned}
\{f_0, f_1, \cdots, f_{n - 1}\} \\
\{g_0, g_1, \cdots, g_{n - 1}\}
\end{aligned}$$

求

$$\{h_0, h_1, \cdots, h_{n - 1}\}$$

满足

$$h_i = \sum_{j = 0}^{i} f_j\cdot g_{i - j}$$

即

$$h = f * g$$

## Analysis

### 插值定理

已知$n$个点$(x_0, y_0), (x_1, y_1), \cdots, (x_{n - 1}, y_{n - 1})$，其中$x_i \neq x_j (i\neq j), x_i, y_i\in \mathbb C$，则可以唯一确定通过这$n$个点的$n-1$次多项式

$$p_{n-1}(x) = a_0 + a_1x + a_2 x^2 + \cdots + a_{n - 1}x^{n - 1}$$

<details>
<summary>证明</summary>

设多项式形式如上，则

$$\begin{cases}
y_0 &= a_0 + a_1x_0 + \cdots + a_{n - 1}x_0^{n-1}\\
y_1 &= a_0 + a_1x_1 + \cdots + a_{n - 1}x_1^{n-1}\\
\vdots &\vdots\\
y_{n-1} &= a_0 + a_1x_{n-1} + \cdots + a_{n - 1}x_{n-1}^{n-1}\\
\end{cases}$$

写成矩阵的形式：

$$\begin{pmatrix}
y_0\\y_1\\\vdots\\y_{n-1}
\end{pmatrix}=\begin{pmatrix}
1 & x_0 & \cdots & x_0^{n-1}\\
1 & x_1 & \cdots & x_{1}^{n-1}\\
\vdots & \vdots & \ddots & \vdots\\
1 & x_{n-1} & \cdots & x_{n-1}^{n-1}
\end{pmatrix}\begin{pmatrix}
a_0\\a_1\\\vdots\\a_{n-1}\end{pmatrix}
$$

中间大矩阵是一个vandermonde矩阵，行列式为$\prod_{i>j} (x_i - x_j)\neq 0$，所以是可逆的。

$\bf y$已知，求$\bf a$，用$\bf a = \bf X^{-1} \bf y$即可。

</details>

### 问题转换

不妨把$f$，$g$，$h$数列都写成多项式的形式，即：

$$\begin{aligned}
f(x) = f_0 + f_1x + \cdots + f_{n-1}x^{n-1}\\
g(x) = g_0 + g_1x + \cdots + g_{n-1}x^{n-1}\\
h(x) = h_0 + h_1x + \cdots + h_{n-1}x^{n-1}
\end{aligned}$$

那么$f * g$的前$n$项就是$h$函数。
 
假设我们选定$x_0, x_1, \cdots, x_{n-1}$，用这$n$个点作为横坐标对$f$和$g$两个函数进行插值。

## NTT

## 练习题

## 编译环境

g++14, macOS

参考vscode `tasks.json`

```json
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "shell",
			"label": "C++ build & run",
			"command": "/opt/homebrew/bin/g++-14",
			"args": [
				"-g",
				"${file}",
				"-o",
				"${fileDirname}/build/${fileBasenameNoExtension}",
				"&&",
				"${fileDirname}/build/${fileBasenameNoExtension}"
			],
			"options": {
				"cwd": "${fileDirname}"
			},
			"problemMatcher": [
				"$gcc"
			],
			"group": "build",
			"detail": "compiler: /opt/homebrew/bin/g++-14"
		}
	]
}
```