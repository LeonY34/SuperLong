#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>
#include "SuperLong.hpp"

#ifndef MUL_METHOD
#define MUL_METHOD 0
#endif

#ifndef WARM_UP
#define WARM_UP 5
#endif

#ifndef REPEAT
#define REPEAT 100
#endif

#ifndef MAX_BIT
#define MAX_BIT 10000
#endif

#if MUL_METHOD < 0 || MUL_METHOD > 2
#error "MUL_METHOD must be 0, 1, or 2"
#endif

static std::string random_decimal_with_max_bits(std::mt19937_64& rng, int max_bit) {
	std::uniform_int_distribution<int> bit_dist(1, max_bit);
	const int bits = bit_dist(rng);
	const int digits = std::max(1, int(bits * 0.30103) + 1);

	std::uniform_int_distribution<int> first_digit(1, 9);
	std::uniform_int_distribution<int> digit_dist(0, 9);
	std::bernoulli_distribution sign_dist(0.5);

	std::string s;
	s.reserve(size_t(digits) + 1);
	if (sign_dist(rng)) {
		s.push_back('-');
	}
	s.push_back(char('0' + first_digit(rng)));
	for (int i = 1; i < digits; ++i) {
		s.push_back(char('0' + digit_dist(rng)));
	}
	return s;
}

int main() {
	SuperLong::setMulMethod(MUL_METHOD);

	std::random_device rd;
	std::mt19937_64 rng(rd());

	const int total = WARM_UP + REPEAT;
	std::vector<SuperLong> lhs;
	std::vector<SuperLong> rhs;
	lhs.reserve(total);
	rhs.reserve(total);

	for (int i = 0; i < total; ++i) {
		lhs.emplace_back(random_decimal_with_max_bits(rng, MAX_BIT));
		rhs.emplace_back(random_decimal_with_max_bits(rng, MAX_BIT));
	}

	volatile size_t sink = 0;
	for (int i = 0; i < WARM_UP; ++i) {
		SuperLong c = lhs[size_t(i)] * rhs[size_t(i)];
		sink += size_t(bool(c));
	}

	const auto begin = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < REPEAT; ++i) {
		const size_t idx = size_t(WARM_UP + i);
		SuperLong c = lhs[idx] * rhs[idx];
		sink += size_t(bool(c));
	}
	const auto end = std::chrono::high_resolution_clock::now();

	const double total_us = std::chrono::duration<double, std::micro>(end - begin).count();
	const double avg_us = total_us / double(REPEAT);

	std::cout << std::fixed << std::setprecision(3)
			  << "mul_method=" << MUL_METHOD
			  << ",warmup=" << WARM_UP
			  << ",repeat=" << REPEAT
			  << ",max_bit=" << MAX_BIT
			  << ",avg_us=" << avg_us
			  << '\n';

	if (sink == 0) {
		std::cerr << "sink=0\n";
	}

	return 0;
}