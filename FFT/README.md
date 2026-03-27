# FFT学习

P开头的题目来自[luogu.com.cn](https://www.luogu.com.cn)

## Introduction

FFT用于解决离散卷积问题。比如说已知

$$\begin{aligned}
\{f_0, f_1, \cdots, f_{n - 1}\} \\
\{g_0, g_1, \cdots, g_{n - 1}\}
\end{aligned}$$

求

$$h_i = \sum_{j = 0}^{i} f_j\cdot g_{i - j}$$

即

$$h = f * g$$

## Analysis

后面补

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