#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <cctype>
#include <cstring>
#include <algorithm>
#define THROW_ERROR(msg) throw std::runtime_error(std::string(msg) + " at " + __FILE__ + ":" + std::to_string(__LINE__))

using LL = __uint128_t;
using SLL = __int128_t;
using HF = __uint64_t;
using SHF = __int64_t;

class SuperLong {
private:
    bool sign; // sign bit
    LL* data; // compressed data
    size_t size; // blocks of data used
    size_t len; // total length of data
    static SuperLong fromStr(const std::string& s);
    void iadd(const SuperLong& other);
    void isub(const SuperLong& other);
    void imulNaive(const SuperLong& other);
    void imulDaC(const SuperLong& other);
    void imulFourier(const SuperLong& other);
    SuperLong mulNaive(const SuperLong& other) const;
    SuperLong mulDaC(const SuperLong& other) const;
    SuperLong mulFourier(const SuperLong& other) const;
    SuperLong idivKnuth(const SuperLong& other);
    SuperLong idivFourier(const SuperLong& other);
    // void imod(const SuperLong& other);
    void ilsh(const size_t x);
    void irsh(const size_t x);
    void ior(const SuperLong& other);
    void iand(const SuperLong& other);
    void ior(const LL& other);
    void iand(const LL& other);
    // static SuperLong add(const SuperLong& a, const SuperLong& b);
    // static SuperLong sub(const SuperLong& a, const SuperLong& b);
    // static SuperLong mul(const SuperLong& a, const SuperLong& b);
    // static SuperLong div(const SuperLong& a, const SuperLong& b);
    // static SuperLong lsh(const SuperLong& a, const size_t x);
    void imulSmall(const LL x);
    SuperLong mulSmall(const LL x) const;
    void iaddSmall(LL x, const size_t offset=0);
    void isubSmall(LL x);
    HF idivSmall(const HF x);
    void maintainSpace(size_t x);
    void strip();
    static SuperLong fromInt(SLL x);

    static bool isAbsSmaller(const SuperLong& a, const SuperLong& b);
    // static SuperLong rsh(const SuperLong& a, const size_t x);

    HF getHF(const size_t i) const;
public:
    SuperLong() : sign(0), data(nullptr), size(0), len(0) {}
    SuperLong(const SuperLong& other) : sign(other.sign), size(other.size), len(other.size) {
        data = new LL[len];
        memcpy(data, other.data, size * sizeof(LL));
    }
    SuperLong(const LL x) : SuperLong(fromInt(x)) {}
    SuperLong(const std::string& s) : SuperLong(fromStr(s)) {}
    ~SuperLong() {
        delete[] data;
    }

    void stripForce();
    std::string toStr() const;
    void info() const;
    // big ones
    SuperLong& operator = (const SuperLong& other);
    SuperLong& operator += (const SuperLong& other);
    SuperLong& operator -= (const SuperLong& other);
    SuperLong& operator *= (const SuperLong& other);
    SuperLong& operator /= (const SuperLong& other);
    SuperLong& operator %= (const SuperLong& other);
    SuperLong& operator <<= (const SuperLong& x);
    SuperLong& operator >>= (const SuperLong& x);

    // small ones
    SuperLong& operator = (const SLL& other);
    SuperLong& operator += (const SLL& other);
    SuperLong& operator -= (const SLL& other);
    SuperLong& operator *= (const SLL& other);
    SuperLong& operator /= (const SHF& other);
    SuperLong& operator %= (const SHF& other);
    SuperLong& operator <<= (const size_t x);
    SuperLong& operator >>= (const size_t x);
    
    // big ones
    SuperLong operator + (const SuperLong& other) const;
    SuperLong operator - (const SuperLong& other) const;
    SuperLong operator * (const SuperLong& other) const;
    SuperLong operator / (const SuperLong& other) const;
    SuperLong operator % (const SuperLong& other) const;
    SuperLong operator << (const SuperLong& x) const;
    SuperLong operator >> (const SuperLong& x) const;

    // small ones
    SuperLong operator + (const SLL& other) const;
    SuperLong operator - (const SLL& other) const;
    SuperLong operator * (const SLL& other) const;
    SuperLong operator / (const SHF& other) const;
    SuperLong operator % (const SHF& other) const;
    SuperLong operator << (const size_t& x) const;
    SuperLong operator >> (const size_t& x) const;

    // compare
    bool operator < (const SuperLong& other) const;
    bool operator > (const SuperLong& other) const;
    bool operator == (const SuperLong& other) const;
    operator bool() const {
        return size;
    }

    // easy
    SuperLong operator - () const;
    friend std::ostream& operator << (std::ostream& out, const SuperLong& a);
    friend std::istream& operator >> (std::istream& in, SuperLong& a);
    friend void swap(SuperLong& a, SuperLong& b);
};