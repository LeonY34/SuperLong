# FFT学习

P开头的题目来自[luogu.com.cn](https://www.luogu.com.cn)

更好阅读体验见[blog](https://leony34.github.io/blog/2026/03/28/FFT/#more)。

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
&\vdots\\
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

不妨把$f$，$g$数列都写成多项式的形式，即：

$$\begin{aligned}
f(x) = f_0 + f_1x + \cdots + f_{n-1}x^{n-1}\\
g(x) = g_0 + g_1x + \cdots + g_{n-1}x^{n-1}
\end{aligned}$$

则$f\times g$可以算出来：

$$
h(x) = f(x)\times g(x) = \sum_{i=0}^{2n-2} x^{i}\sum_{j} f_jg_{i-j}
$$

得到的$h(x)$是一个$2n-2$次多项式，且$h(x)$的$x^0, x^1, \cdots, x^{n-1}$的系数就是$h$数列。
 
假设我们选定$x_0, x_1, \cdots, x_{2n-2}$，用这$2n-1$个点作为横坐标对$f$和$g$两个函数进行插值，由于$f$和$g$是已知的，所以可以计算$f(x_0), f(x_1), \cdots, f(x_{2n-2})$和$g(x_0), g(x_1), \cdots, g(x_{2n-2})$

同时有

$$h(x) = f(x)g(x)$$

所以

$$\begin{aligned}
h(x_0) &= f(x_0)g(x_0)\\
h(x_1) &= f(x_1)g(x_1)\\
\vdots &\\
h(x_{2n-2}) &= f(x_{2n-2})g(x_{2n-2})
\end{aligned}$$

所以我们得到了$h$函数的$2n-1$个插值，于是唯一确定了$h(x)$这个函数。得到该函数后取前$n$个系数就可以得到$h$数列。

于是我们期望的流程是：

- 对$f$和$g$函数插值；
- 计算出$h$在各个插值点的值；
- 反解出$h$的系数。

**为了以后说明的方便，我们假设$n=2^m$。**

### Interpolate

接下来研究怎么对$f$和$g$函数插值。只研究$f$函数，因为$g$完全相同。

为了方便，我们现在假设只需要对$f$进行$n$个点的插值（本身需要$2n-1$个点，但是我们也可以直接把$n\leftarrow 2n$，然后假设$f$是一个$2n$次多项式，进行$2n$个点的插值，效果相同）。

计算$f(x_0)$是$O(n)$的，所以获得所有插值就$O(n^2)$了！

怎么解决这个问题呢？我们需要取一些特殊的插值点，以便重复利用插值点的数值。

如果有两个插值点$x_0$和$x_1$满足$x_0 = -x_1$，则我们如果对$f$的系数进行奇偶分类（**我们假设$n=2^m$**）：

$$\begin{aligned}
f_e(x) = f_0 + f_2x + f_4x^2 + \cdots + f_{n-2}x^{n/2-1}\\
f_o(x) = f_1 + f_3x + f_5x^2 + \cdots + f_{n-1}x^{n/2-1}
\end{aligned}$$

那么有

$$\begin{aligned}
f(x_0) = f_e(x_0^2) + x_0f_o(x_0^2)\\
f(x_1) = f_e(x_1^2) + x_1f_o(x_1^2)\\
\end{aligned}$$

由于$x_0^2 = x_1^2$，可以化为

$$\begin{aligned}
f(x_0) = f_e(x_0^2) + x_0f_o(x_0^2)\\
f(x_1) = f_e(x_0^2) - x_0f_o(x_0^2)\\
\end{aligned}$$

也就是说，我们只需要计算$f_e$和$f_o$两个长度为$n/2$的函数在$x_0^2$处的插值信息，我们就可以得到$f$在$x_0$和$x_1$处的插值信息。

但是，如果插值点都取实数的话，到第二层，所有$x\geq 0$，也就不存在相反数了（不能递归）。

解决这个问题的办法就是采用复数单位根来作为插值点！

对于长度为$n$的函数$f$，我们把复平面上的单位圆分成$n$份来取插值点。具体而言：

$$x_k = e^{\frac{2\mathrm j\pi k}{n}} = \cos(\frac{2\pi k}{n}) + \mathrm j \sin(\frac{2\pi k}{n})$$

其中$\mathrm j = \sqrt -1$。

为了方便，我们可以记$\omega = e^{\frac{2\mathrm j\pi}{n}}$，则

$$x_k = \omega^k$$

那么就有性质：

$$x_k = -x_{k + \frac{n}{2}}, k < \frac{n}{2}$$

这样就可以两两凑成一对，问题转化为了两个$\frac{n}{2}$的子问题，并且由于$x_k^2$刚好可以覆盖所有的$e^{\frac{\mathrm j\pi k}{n}}$，所以这个问题可以一直递归下去，边界就是返回当前的系数。

复杂度：

$$T(n) = 2T(n/2) + O(n)\implies T(n) = O(n\log n)$$

### Calculate $h$

第二步：计算出$h$函数在插值点的值，直接乘就好了，$O(n)$。接下来研究第三步：反解出$h$的系数。

我们不妨先回顾一下第一步解决了什么问题：

> 给定$n-1$次多项式$f$，求出$f$在$n$次单位根上的值。
> 形式化的说：已知$f_0, f_1, \cdots, f_{n-1}$，计算$y_0, y_1, \cdots, y_{n-1}$：
>
> $$\begin{pmatrix}
y_0\\y_1\\\vdots\\y_{n-1}
\end{pmatrix} = \begin{pmatrix}
1 & x_0 & \cdots & x_0^{n-1}\\
1 & x_1 & \cdots & x_1^{n-1}\\
\vdots & \vdots & \ddots & \vdots\\
1 & x_{n-1} & \vdots & x_{n-1}^{n-1}
\end{pmatrix}\begin{pmatrix}
f_0\\f_1\\\vdots\\f_{n-1}
\end{pmatrix}$$
> 其中$x_k = \omega^k$，$\omega = e^{2\mathrm j\pi/n}$。

为了方便，我们依然认为$h$函数的长度只有$n$（如果不够，用之前相同的方法，后面补$0$，把$f,g,h$都补到相同的$n=2^m$）。

那么当前的问题是

$$\begin{pmatrix}
y^{(h)}_0\\y^{(h)}_1\\\vdots\\y^{(h)}_{n-1}
\end{pmatrix} = \begin{pmatrix}
1 & x_0 & \cdots & x_0^{n-1}\\
1 & x_1 & \cdots & x_1^{n-1}\\
\vdots & \vdots & \ddots & \vdots\\
1 & x_{n-1} & \vdots & x_{n-1}^{n-1}
\end{pmatrix}\begin{pmatrix}
h_0\\h_1\\\vdots\\h_{n-1}
\end{pmatrix}$$

已知$\mathbf y^{(h)}$，求$\mathbf h$。

我们知道中间那个矩阵是可逆的，所以我们现在尝试求他的逆（普通方法是$O(n^3)$的，我们需要更快的办法）。

令中间的矩阵为$\bm X$，则

$$\bm X_{ij} = (\omega^{i})^j = \omega^{ij}$$

则可以看出$\bm X$是一个对称矩阵。计算$\bm X\bm X^{*T}$：

$$\bm X\bm X^{*T}_{ij} = \sum_{k=0}^{n-1} \omega^{ik}\omega^{-kj} = \frac{1 - \omega^{(i-j)n}}{1 - \omega^{i-j}} = \begin{cases}
0 & i\neq j\\
n & i = j\end{cases}$$

所以说$$\bm X^{-1} = \frac{1}{n} \bm X^{*T} = \frac{1}{n}\bm X^{*}$$

那么问题就转化为了

$$\bm h = \bm X^{*}\bm y^{(h)}$$

这个问题和第一阶段的问题非常相似，只不过第一阶段用的$\omega$换成了$\omega^{-1}$。

所以我们再做一次第一阶段的问题，就可以在$O(n\log n)$时间内完成反解！

## NTT

复数的计算依赖浮点数，容易丢失精度。

于是可以使用数论变换代替（寻找一个恰当的模数$MOD$，某个整数$\omega$在模$MOD$运算下可以形成一个$2^m$的环）。

## 实现技巧

1. 复数运算加速：使用手写的`complex`类，可以快1.5x~2x（见`P3803_fast.cpp`）；
2. 使用倍增替代递归：朴素的实现（不改变数据分布）不会带来好处，只会让cache全部未命中（stride太大）。见`P3803_loop.cpp`）；更优化的实现，将初始数据排布变为左边都是偶数，右边都是奇数，连续访问内存。见`P3803_loop_v2.cpp`）。
3. 减少运算冗余：每次插值计算只需要算一半，另一半用缓存好的取相反数；
4. 使用NTT解决精度问题。没有使用NTT很难过`P4721`，见`P4721_naiveFFT.cpp`。优化过的见`P4721_NTT.cpp`。可以通过`python3 P4721_judger.py`检验正确性。

## 练习题

- [P3803 【模板】多项式乘法（FFT）](https://www.luogu.com.cn/problem/P3803)
- [P1303 A*B Problem](https://www.luogu.com.cn/problem/P1303)
- [P4721 【模板】分治 FFT](https://www.luogu.com.cn/problem/P4721)

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