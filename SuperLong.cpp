#include "SuperLong.hpp"

static constexpr HF halfMask = HF(-1);
static constexpr size_t halfBits = sizeof(HF) * 8;
static constexpr size_t fullBits = sizeof(LL) * 8;

static std::string toString(LL x) {
    /*
        debug function, change LL to string
    */
    std::string s;
    do {
        // s += std::to_string(unsigned(x % 10));
        s += char(x % 10 + 48);
        x /= 10;
    } while (x);
    std::reverse(s.begin(), s.end());
    return s;
}

void SuperLong::stripForce() {
    /*
        Strip the zeros at the front of the string.
        Use it carefully! It can make it very slow.
    */
    if (!len) return;
    while (size && !data[size - 1]) --size;
    LL* tmp = nullptr;
    if (size > 0) {
        tmp = new LL[size];
        len = size;
        memcpy(tmp, data, size * sizeof(LL));
    }
    delete[] data;
    data = tmp;
}

void SuperLong::strip() {
    /*
        Strip the zeros at the front of the string (in size).

        Also sign = 0 if *this = 0
    */
    while (size && !data[size - 1]) --size;
    if (!size) sign = 0;
}

void SuperLong::maintainSpace(size_t x) {
    /*
        reserve the space for x blocks
        size >= x after the operation
    */
    if (x <= size) return; // if already have size
    if (x <= len) { // if malloced, but size < x
        memset(data + size, 0, (x - size) * sizeof(LL));
        size = x;
        return;
    }
    size_t new_size = x;
    while (x != (x & (-x))) x -= (x & (-x));
    len = x << 1;
    LL* tmp = new LL[len];
    memcpy(tmp, data, size * sizeof(LL));
    memset(tmp + size, 0, (new_size - size) * sizeof(LL));
    size = new_size; // x changed!
    delete[] data;
    data = tmp;
}

void SuperLong::ilsh(const size_t x) {
    /*
        left shift x bits.
        strip-persist operation

        Sign not considered.
    */
    if (!size) return;
    const size_t x1 = x / fullBits;
    const size_t x2 = x % fullBits;
    if (x1) {
        maintainSpace(size + x1); // size = size + x1
        for (size_t i = size - 1; ~(i - x1); --i) {
            data[i] = data[i - x1];
        }
        memset(data, 0, x1 * sizeof(LL));
    }
    if (x2) {
        // top x2 bits
        LL top = (data[size - 1] >> (fullBits - x2));
        for (size_t i = size - 1; ~i; --i) {
            data[i] <<= x2; // lshift
            // top x2 bits of data[i - 1]
            if (i) data[i] |= (data[i - 1] >> (fullBits - x2));
        }
        if (top) {
            maintainSpace(size + 1); // size += 1
            data[size - 1] = top;
        }    
    }
}

void SuperLong::irsh(const size_t x) {
    /*
        right shift x bits.
        strip-persist operation

        Sign not considered.
    */
    if (!size) return;
    const size_t x1 = x / fullBits;
    const size_t x2 = x % fullBits;
    if (x1) {
        if (size < x1) size = 0;
        else size -= x1;
        for (size_t i = 0; i < size; ++i) {
            data[i] = data[i + x1];
        }
    }

    if (x2) {
        for (size_t i = 0; i < size; ++i) {
            data[i] >>= x2;
            if (i + 1 < size) data[i] |= (data[i + 1] << (fullBits - x2));
        } 
    }
    strip();
}

void SuperLong::iadd(const SuperLong& other) {
    /*
        add another.
        strip-persist operation

        assume have the same sign.
    */
    bool tag = 0;
    maintainSpace(other.size); // this will change size to max(size, other.size)
    for (size_t i = 0; i < size; ++i) {
        LL now = (i < other.size ? other.data[i] : 0);
        if (tag) {
            ++now;
            tag = 0;
            if (!now) {
                tag = 1;
            }
        }
        tag |= (now != 0 && data[i] >= (-now));
        data[i] += now;
    }
    maintainSpace(size + tag); // this will change size
    if (tag) data[size - 1] = 1;
}

void SuperLong::isub(const SuperLong& other) {
    /*
        sub another.
        stripped.

        Assume sign the same
    */
    size_t size_a = size;
    size_t size_b = other.size;
    const size_t maxSize = std::max(size_a, size_b);
    maintainSpace(maxSize);
    const LL* a = data;
    const LL* b = other.data;
    if (isAbsSmaller(*this, other)) {
        std::swap(a, b), std::swap(size_a, size_b), sign = (!sign);
    }
    bool tag = 0;
    
    // printf("\na:::\n");
    // for (size_t i = 0; i < size_a; ++i) printf("%s ", toString(a[i]).c_str());
    // printf("\nb:::\n");
    // for (size_t i = 0; i < size_b; ++i) printf("%s ", toString(b[i]).c_str());
    // printf("\n");
    // printf("size_a = %lu, size_b = %lu\n", size_a, size_b);
    for (size_t i = 0; i < maxSize; ++i) {
        LL now_a = (i < size_a ? a[i] : 0);
        LL now_b = (i < size_b ? b[i] : 0);
        bool new_tag = (now_a < now_b || (now_a == now_b && tag));
        data[i] = now_a - (now_b + tag);
        // printf("now_a = %s, now_b = %s, tag = %d\n", toString(now_a).c_str(), toString(now_b).c_str(), tag);
        tag = new_tag;
    }
    if (tag) THROW_ERROR("TAG NOT WELCOMED!");
    strip();
}

void SuperLong::imulNaive(const SuperLong& other) {
    /*
        mul another
        using naive implementation.

        Assume abs.
    */
    SuperLong now = 0;
    for (size_t i = 0; i < other.size; ++i) {
        now.iadd(mulSmall(other.data[i]) << (i * fullBits));
    }
    now.sign = sign;
    (*this) = now;
}

SuperLong SuperLong::mulNaive(const SuperLong& other) const {
    /*
        mul another
        using naive implementation.

        Assume abs.
    */
    SuperLong now = 0;
    for (size_t i = 0; i < other.size; ++i) {
        now.iadd(mulSmall(other.data[i]) << (i * fullBits));
    }
    now.sign = sign;
    return now;
}

HF SuperLong::getHF(const size_t i) const {
    /*
        get a digit in halfbits
    */
    const size_t idx = i >> 1;
    if (idx >= size) return 0;
    return ((i & 1) ? (data[idx] >> halfBits) : data[idx] & halfMask);
}

SuperLong SuperLong::idivKnuth(const SuperLong& other) {
    /*
        div another
        using Knuth Algorithm

        assume abs.
    */
    if (!other.size) THROW_ERROR("div by 0! idivKnuth.");
    // const bool origSign = sign;
    // sign = 0;
    // SuperLong divisorAbs = (other.sign ? -other : other);
    // divisorAbs.sign = 0;
    if (!size) return 0;
    if (size < other.size) {
        SuperLong remain(0);
        swap(*this, remain);
        remain.sign = 0;
        return remain;
    }
    size_t pos, otherPos;
    pos = getHF(size * 2 - 1) ? size * 2 : size * 2 - 1;
    otherPos = other.getHF(other.size * 2 - 1) ? other.size * 2 : other.size * 2 - 1;
    // otherPos = divisorAbs.getHF(divisorAbs.size * 2 - 1) ? divisorAbs.size * 2 : divisorAbs.size * 2 - 1;
    if (otherPos == 1) {
        const HF remain = idivSmall(other.getHF(0));
        // sign = origSign;
        return SuperLong(LL(remain));
    }
    HF v1_ = other.getHF(otherPos - 1);
    HF d = 1;
    if (v1_ <= (halfMask >> 1)) d = (LL(halfMask) + 1) / (v1_ + 1);
    // SuperLong otherNorm(divisorAbs);
    SuperLong now(other);
    if (d > 1) {
        imulSmall(d);
        // otherNorm.imulSmall(d);
        now.imulSmall(d);
        // pos = getHF(size * 2 - 1) ? size * 2 : size * 2 - 1;
        // otherPos = otherNorm.getHF(otherNorm.size * 2 - 1) ? otherNorm.size * 2 : otherNorm.size * 2 - 1;
    }
    // const HF v1 = otherNorm.getHF(otherPos - 1);
    // const HF v2 = otherNorm.getHF(otherPos - 2);
    const HF v1 = now.getHF(otherPos - 1);
    const HF v2 = now.getHF(otherPos - 2);
    const LL b = LL(halfMask) + 1;
    const bool sg = sign;
    SuperLong ans = 0;
    // for (size_t k = pos - otherPos + 1; k; --k) {
    for (size_t i = pos - 1; ~(i - otherPos + 1); --i) {
        // const size_t shiftHF = k - 1;
        // const size_t i = shiftHF + otherPos - 1;
        HF u1 = getHF(i + 1); // u1 <= v1
        HF u2 = getHF(i);
        HF u3 = getHF(i - 1);
        LL q_ = (LL(u1) * b + u2) / v1;
        LL r_ = (LL(u1) * b + u2) % v1;
        while (q_ == b || LL(q_) * v2 > b * r_ + u3) {
            q_ --;
            r_ += v1;
            if (r_ >= b) break;
        }
        // isub(otherNorm.mulSmall(q_) << (shiftHF * halfBits));
        isub(now.mulSmall(q_) << ((i - otherPos + 1) * halfBits));
        if (size && sign != sg) {
            --q_;
            // iadd(otherNorm << (shiftHF * halfBits));
            iadd(now << ((i - otherPos + 1) * halfBits));
        }
        ans <<= halfBits;
        if (!ans.size) {
            ans.maintainSpace(1);
        }
        ans.ior(q_);
    }
    ans.sign = sg;
    ans.strip();
    if (d > 1) idivSmall(d);
    strip();
    swap(*this, ans);
    ans.sign = 0;
    // printf("ans = %s\n", ans.toStr().c_str());
    return ans;
}

void SuperLong::ior(const LL& other) {
    if (!size) *this = other;
    else data[0] |= other;
}

void SuperLong::imulSmall(const LL x) {
    /*
        multiply a small integer.
        strip-persist operation.

        SIGN NOT CONSIDERED.
    */
    const HF x1 = (x & halfMask);
    const HF x2 = (x >> halfBits);
    SuperLong now;
    for (size_t i = 0; i < size; ++i) {
        const HF y1 = (data[i] & halfMask);
        const HF y2 = (data[i] >> halfBits);
        now.iaddSmall(LL(x2) * y2, i + 1);
        now.iaddSmall(LL(x1) * y1, i);
        const LL t1 = LL(x1) * y2;
        const LL t2 = LL(x2) * y1;
        now.iaddSmall(t1 >> halfBits, i + 1);
        now.iaddSmall(t2 >> halfBits, i + 1);
        now.iaddSmall(t1 << halfBits, i);
        now.iaddSmall(t2 << halfBits, i);
    }
    now.sign = sign;
    *this = now;
    strip();
}

SuperLong SuperLong::mulSmall(const LL x) const {
    /*
        multiply a small integer.
        strip-persist operation.

        SIGN NOT CONSIDERED.
    */
    const HF x1 = (x & halfMask);
    const HF x2 = (x >> halfBits);
    SuperLong now;
    for (size_t i = 0; i < size; ++i) {
        const HF y1 = (data[i] & halfMask);
        const HF y2 = (data[i] >> halfBits);
        now.iaddSmall(LL(x2) * y2, i + 1);
        now.iaddSmall(LL(x1) * y1, i);
        const LL t1 = LL(x1) * y2;
        const LL t2 = LL(x2) * y1;
        now.iaddSmall(t1 >> halfBits, i + 1);
        now.iaddSmall(t2 >> halfBits, i + 1);
        now.iaddSmall(t1 << halfBits, i);
        now.iaddSmall(t2 << halfBits, i);
    }
    now.sign = sign;
    now.strip();
    return now;
}



void SuperLong::iaddSmall(LL x, const size_t offset) {
    /*
        add a small integer at block offset;
        strip-persist operation

        SIGN NOT CONSIDERED
    */
    size_t i = offset;
    while (i < size && x && data[i] >= LL(-x)) {
        data[i++] += x;
        x = 1;
    }
    if (x) {
        maintainSpace(i + 1);
        data[i] += x;
    }
}

HF SuperLong::idivSmall(const HF x) {
    /*
        high precision divided by low precision.
        returns the mod.
        strip-persist (with strip function)

        SIGN NOT CONSIDERED
    */
    if (x == 0) THROW_ERROR("divided by 0!");
    LL remain = 0;
    for (size_t i = size - 1; ~i; --i) {
        const HF hi = (data[i] >> halfBits);
        const HF lo = (data[i] & halfMask);
        remain = ((remain << halfBits) | hi);
        data[i] = ((remain / x) << halfBits);
        remain = (((remain % x) << halfBits) | lo);
        data[i] |= (remain / x);
        remain %= x;
    }
    strip();
    return remain;
}

void SuperLong::isubSmall(const LL x) {
    /*
        big minus small
        strip persist (using strip)
        substract the abs

        SIGN will TOGGLE if x > *this
    */
    if (!size) {
        sign ^= 1;
        maintainSpace(1);
        data[0] = x;
        return;
    }
    if (size == 1 && data[0] < x) {
        sign ^= 1;
        data[0] = x - data[0];
        return;
    }
    bool tag = (data[0] < x);
    data[0] -= x;
    for (size_t i = 1; tag && i < size; ++i) {
        tag = (data[i] == 0);
        data[i] -= 1;
    }
    strip();
}

SuperLong SuperLong::fromInt(SLL x) {
    /*
        construct a SuperLong from a Int
    */
    SuperLong now;
    if (x) {
        now.sign = (x < 0);
        if (now.sign) x = -x;
        now.len = 1;
        now.data = new LL[1];
        now.data[0] = LL(x);
        now.size = 1;
    }
    return now;
}

static LL fromSmallStr(const std::string& s, const size_t l, const size_t r) {
    /*
        construct a LL from a substring of s (s[l, r))
        s should only contain digits.
    */
    LL now = 0;
    for (size_t i = l; i < r; ++i) {
        if (!std::isdigit(s[i])) THROW_ERROR("Not DIGIT! fromSmallStr");
        now = now * 10 + (s[i] ^ 48);
    }
    return now;
}

SuperLong SuperLong::fromStr(const std::string& s) {
    /*
        Construct from string
    */
    SuperLong now;
    bool sg = 0;
    if (s[0] == '-') {
        sg = 1;
    } else if (std::isdigit(s[0])) {
        now.iaddSmall(s[0] ^ 48);
    } else {
        THROW_ERROR("Invalid string, expected digit / -.");
    }
    constexpr LL base[] = {
        LL(1e0), LL(1e1), LL(1e2), LL(1e3), LL(1e4), LL(1e5),
        LL(1e6), LL(1e7), LL(1e8), LL(1e9), LL(1e10), LL(1e11),
        LL(1e12), LL(1e13), LL(1e14), LL(1e15), LL(1e16), LL(1e17), LL(1e18), LL(1e19),
        LL(1e19) * LL(1e1), LL(1e19) * LL(1e2), LL(1e19) * LL(1e3), LL(1e19) * LL(1e4), LL(1e19) * LL(1e5),
        LL(1e19) * LL(1e6), LL(1e19) * LL(1e7), LL(1e19) * LL(1e8), LL(1e19) * LL(1e9), LL(1e19) * LL(1e10), LL(1e19) * LL(1e11),
        LL(1e19) * LL(1e12), LL(1e19) * LL(1e13), LL(1e19) * LL(1e14), LL(1e19) * LL(1e15), LL(1e19) * LL(1e16), LL(1e19) * LL(1e17),
        LL(1e19) * LL(1e18), LL(1e19) * LL(1e19)
    };
    for (size_t i = 1; i < s.length(); i += 38) {
        const size_t l = std::min(size_t(38), s.length() - i);
        now.imulSmall(base[l]);
        now.iaddSmall(fromSmallStr(s, i, i + l));
    }
    now.sign = sg;
    now.strip();
    return now;
}

std::string SuperLong::toStr() const {
    /*
        change to string (for display)
    */
    constexpr HF base = 1e19;
    SuperLong cl(*this);
    std::string s;
    do {
        HF block = cl.idivSmall(base);
        std::string now = std::to_string(block);
        std::reverse(now.begin(), now.end());
        while (now.length() < 19 && cl) now += '0';
        s += now;
    } while (cl);
    if (sign) s += '-';
    std::reverse(s.begin(), s.end());
    return s;
}

void SuperLong::info() const {
    /*
        debug. display info about SuperLong object.
    */
    printf("sign = %d, size = %lu, len = %lu\n", sign, size, len);
    printf("array:: ");
    for (size_t i = 0; i < len; ++i) {
        // printf("%s ", toString(data[i]));
        std::cout << toString(data[i]) << ' ';
    }
    printf("\n");
    std::cout << ">>> " << toStr() << std::endl;
}

// big ones -----------------

SuperLong& SuperLong::operator = (const SuperLong& other) {
    if (this != &other) {
        sign = other.sign;
        size = other.size;
        if (len < other.size) {
            delete[] data; // delete nullptr是安全的
            len = other.size; // 优化一下
            data = new LL[len];
        }
        memcpy(data, other.data, size * sizeof(LL));
        memset(data + size, 0, (len - size) * sizeof(LL));
    }
    return *this;
}
SuperLong& SuperLong::operator += (const SuperLong& other) {
    if (sign ^ other.sign) {
        isub(other); // 绝对值相减，sign取前者
    } else {
        iadd(other);
    }
    return *this;
}
SuperLong& SuperLong::operator -= (const SuperLong& other) {
    if (sign ^ other.sign) {
        iadd(other);
    } else {
        isub(other);
    }
    return *this;
}

SuperLong& SuperLong::operator *= (const SuperLong& other) {
    imulNaive(other);
    sign ^= other.sign;
    strip();
    return *this;
}

SuperLong& SuperLong::operator /= (const SuperLong& other) {
    idivKnuth(other);
    sign ^= other.sign;
    strip();
    return *this;
}

SuperLong& SuperLong::operator %= (const SuperLong& other) {
    const bool sg = sign;
    const bool otherSg = other.sign;
    SuperLong mod = idivKnuth(otherSg ? -other : other);
    if (!mod)  return *this = mod;
    if (sg) {
        mod = (otherSg ? -other : other) - mod;
    }
    if (otherSg) {
        mod -= (otherSg ? -other : other);
    }
    (*this) = mod;
    return *this;
}

SuperLong& SuperLong::operator <<= (const SuperLong& x) {
    if (x.sign) THROW_ERROR("X < 0! in operator <<=");
    if (x.size > 1) THROW_ERROR("X TOO LARGE! in operator <<=");
    *this <<= x.data[0];
    return *this;
}

SuperLong& SuperLong::operator >>= (const SuperLong& x) {
    if (x.sign) THROW_ERROR("X < 0! in operator <<=");
    if (x.size > 1) {
        *this = 0;
    } else {
        *this >>= x.data[0];
    }
    return *this;
}

// small ones --------------------

SuperLong& SuperLong::operator = (const SLL& other) {
    if (!len) {
        len = 1;
        data = new LL[1];
    }
    data[0] = (other < 0 ? -other : other);
    sign = (other < 0);
    size = 1;
    strip();
    return *this;
}

SuperLong& SuperLong::operator += (const SLL& other) {
    if (sign ^ (other < 0)) {
        isubSmall(other > 0 ? other : -other);
    } else {
        iaddSmall(other > 0 ? other : -other);
    }
    return *this;
}
SuperLong& SuperLong::operator -= (const SLL& other) {
    if (sign ^ (other < 0)) {
        iaddSmall(other > 0 ? other : -other);
    } else {
        isubSmall(other > 0 ? other : -other);
    }
    return *this;
}
SuperLong& SuperLong::operator *= (const SLL& other) {
    imulSmall(other < 0 ? -other : other);
    sign ^= (other < 0);
    strip();
    return *this;
}

SuperLong& SuperLong::operator /= (const SHF& other) {
    // 向零取整
    idivSmall(other < 0 ? -other : other);
    sign ^= (other < 0);
    strip();
    return *this;
}

SuperLong& SuperLong::operator %= (const SHF& other) {
    // 和python行为一致
    SHF absOther = (other < 0 ? -other : other);
    const bool sg = sign;
    SHF mod = idivSmall(absOther);
    if (!mod) return *this = mod;
    if (sg) mod = absOther - mod;
    if (other < 0) mod = mod - absOther;
    *this = mod;
    return *this;
}

SuperLong& SuperLong::operator <<= (const size_t x) {
    // 不用补码，用符号位+原码
    ilsh(x);
    return *this;
}

SuperLong& SuperLong::operator >>= (const size_t x) {
    // 不用补码，用符号位+原码
    irsh(x);
    return *this;
}

// big ones -------------------------------

SuperLong SuperLong::operator + (const SuperLong& other) const {
    SuperLong now(*this);
    now += other;
    return now;
}
SuperLong SuperLong::operator - (const SuperLong& other) const {
    SuperLong now(*this);
    now -= other;
    return now;
}

SuperLong SuperLong::operator * (const SuperLong& other) const {
    SuperLong now = mulNaive(other);
    now.sign ^= other.sign;
    now.strip();
    return now;
}

SuperLong SuperLong::operator << (const SuperLong& x) const {
    SuperLong now(*this);
    now <<= x;
    return now;
}
SuperLong SuperLong::operator >> (const SuperLong& x) const {
    SuperLong now(*this);
    now >>= x;
    return now;
}

SuperLong SuperLong::operator / (const SuperLong& other) const {
    SuperLong now(*this);
    now /= other;
    return now;
}

SuperLong SuperLong::operator % (const SuperLong& other) const {
    SuperLong now(*this);
    now %= other;
    return now;
}

// small ones --------------

SuperLong SuperLong::operator + (const SLL& other) const {
    SuperLong now(*this);
    now += other;
    return now;
}
SuperLong SuperLong::operator - (const SLL& other) const {
    SuperLong now(*this);
    now -= other;
    return now;
}
SuperLong SuperLong::operator * (const SLL& other) const {
    SuperLong now = mulSmall(other);
    now.sign ^= (other < 0);
    now.strip();
    return now;
}
SuperLong SuperLong::operator / (const SHF& other) const {
    SuperLong now(*this);
    now /= other;
    return now;
}
SuperLong SuperLong::operator % (const SHF& other) const {
    SuperLong now(*this);
    now %= other;
    return now;
}
SuperLong SuperLong::operator << (const size_t& x) const {
    SuperLong now(*this);
    now <<= x;
    return now;
}
SuperLong SuperLong::operator >> (const size_t& x) const {
    SuperLong now(*this);
    now >>= x;
    return now;
}

SuperLong SuperLong::operator - () const {
    SuperLong now(*this);
    now.sign = (sign ^ 1);
    return now;
}

// all of these operators assumes that SuperLong is stripped!
// stripped: no leading zeros, no -0

bool SuperLong::isAbsSmaller(const SuperLong& a, const SuperLong& b) {
    if (a.size < b.size) return true;
    if (a.size > b.size) return false;
    for (size_t i = a.size - 1; ~i; --i) {
        if (a.data[i] < b.data[i]) return true;
        if (a.data[i] > b.data[i]) return false;
    }
    return false;
}

bool SuperLong::operator < (const SuperLong& other) const {
    if (sign ^ other.sign) return sign;
    if (sign) return isAbsSmaller(*this, other);
    return isAbsSmaller(other, *this);
}

bool SuperLong::operator > (const SuperLong& other) const {
    if (sign ^ other.sign) return !sign;
    if (sign) return isAbsSmaller(other, *this);
    return isAbsSmaller(*this, other);
}

bool SuperLong::operator == (const SuperLong& other) const {
    if (sign != other.sign || size != other.size) return false;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) return false;
    }
    return true;
}

std::istream& operator >> (std::istream& in, SuperLong& a) {
    std::string s; in >> s;
    a = SuperLong::fromStr(s);
    return in;
}

std::ostream& operator << (std::ostream& out, const SuperLong& a) {
    out << a.toStr();
    return out;
}

void swap(SuperLong& a, SuperLong& b) {
    std::swap(a.data, b.data);
    std::swap(a.sign, b.sign);
    std::swap(a.size, b.size);
    std::swap(a.len, b.len);
}