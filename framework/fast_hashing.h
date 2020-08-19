/* ***********************************************
 * Hash functions:
 * This file define several hash functions as
 * classes.
 *
 * If DEBUG is defined it asserts that initialization
 * is done properly.
 * ***********************************************/

#ifndef _FAST_HASHING_H_
#define _FAST_HASHING_H_

#include <vector>
#include <cstdint>
#include <type_traits>

#ifdef DEBUG
#include <cassert>
#endif

#include "randomgen.h"
#include "multiplication.h"

using namespace std;

/* ***************************************************
 * 2-wise independent hashing using polyhash.
 * ***************************************************/

class polytwo_64 {
    __uint128_t m_a;
    __uint128_t m_b;

    constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
public:
    void init();
    __uint128_t operator()(uint64_t x);
};

void polytwo_64::init() {
    m_a = getRandomUInt128() >> 39;
    m_b = getRandomUInt128() >> 39;
}

__uint128_t polytwo_64::operator()(uint64_t x) {
    __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
    if (h >= m_p) h -= m_p;

    return h;
}

/* ***************************************************
 * 4-wise independent hashing using polyhash.
 * ***************************************************/

class polyfour_64 {
    __uint128_t m_a;
    __uint128_t m_b;
    __uint128_t m_c;
    __uint128_t m_d;

    constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
public:
    void init();
    __uint128_t operator()(uint64_t x);
};

void polyfour_64::init() {
    m_a = getRandomUInt128() >> 39;
    m_b = getRandomUInt128() >> 39;
    m_c = getRandomUInt128() >> 39;
    m_d = getRandomUInt128() >> 39;
}

__uint128_t polyfour_64::operator()(uint64_t x) {
    __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
    h = fast_large_mult_mod(h, m_c, x);
    h = fast_large_mult_mod(h, m_d, x);
    if (h >= m_p) h -= m_p;

    return h;
}


class polyeight_64 {
    __uint128_t m_a;
    __uint128_t m_b;
    __uint128_t m_c;
    __uint128_t m_d;
    __uint128_t m_e;
    __uint128_t m_f;
    __uint128_t m_g;
    __uint128_t m_h;

    constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
public:
    void init();
    __uint128_t operator()(uint64_t x);
};

void polyeight_64::init() {
    m_a = getRandomUInt128() >> 39;
    m_b = getRandomUInt128() >> 39;
    m_c = getRandomUInt128() >> 39;
    m_d = getRandomUInt128() >> 39;
    m_d = getRandomUInt128() >> 39;
    m_e = getRandomUInt128() >> 39;
    m_f = getRandomUInt128() >> 39;
    m_g = getRandomUInt128() >> 39;
    m_h = getRandomUInt128() >> 39;
}

__uint128_t polyeight_64::operator()(uint64_t x) {
    __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
    h = fast_large_mult_mod(h, m_c, x);
    h = fast_large_mult_mod(h, m_d, x);
    h = fast_large_mult_mod(h, m_e, x);
    h = fast_large_mult_mod(h, m_f, x);
    h = fast_large_mult_mod(h, m_g, x);
    h = fast_large_mult_mod(h, m_h, x);
    if (h >= m_p) h -= m_p;

    return h;
}

/* ***************************************************
 * 2-wise independent hashing using polyhash using exact mod.
 * ***************************************************/
class polytwo_64_exact {
    __uint128_t m_a;
    __uint128_t m_b;

    constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
public:
    void init();
    __uint128_t operator()(uint64_t x);
};

void polytwo_64_exact::init() {
    m_a = getRandomUInt128() >> 39;
    m_b = getRandomUInt128() >> 39;
}

__uint128_t polytwo_64_exact::operator()(uint64_t x) {
    __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
    __uint128_t tmp = h + 1;
    return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
}

/* ***************************************************
 * 4-wise independent hashing using polyhash using exact mod.
 * ***************************************************/

class polyfour_64_exact {
    __uint128_t m_a;
    __uint128_t m_b;
    __uint128_t m_c;
    __uint128_t m_d;

    constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
public:
    void init();
    __uint128_t operator()(uint64_t x);
};

void polyfour_64_exact::init() {
    m_a = getRandomUInt128() >> 39;
    m_b = getRandomUInt128() >> 39;
    m_c = getRandomUInt128() >> 39;
    m_d = getRandomUInt128() >> 39;
}

__uint128_t polyfour_64_exact::operator()(uint64_t x) {
    __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
    h = fast_large_mult_mod(h, m_c, x);
    h = fast_large_mult_mod(h, m_d, x);
    __uint128_t tmp = h + 1;
    return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
}

/* ***************************************************
 * 8-wise independent hashing using polyhash using exact mod.
 * ***************************************************/

class polyeight_64_exact {
    __uint128_t m_a;
    __uint128_t m_b;
    __uint128_t m_c;
    __uint128_t m_d;
    __uint128_t m_e;
    __uint128_t m_f;
    __uint128_t m_g;
    __uint128_t m_h;

    constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
public:
    void init();
    __uint128_t operator()(uint64_t x);
};

void polyeight_64_exact::init() {
    m_a = getRandomUInt128() >> 39;
    m_b = getRandomUInt128() >> 39;
    m_c = getRandomUInt128() >> 39;
    m_d = getRandomUInt128() >> 39;
    m_e = getRandomUInt128() >> 39;
    m_f = getRandomUInt128() >> 39;
    m_g = getRandomUInt128() >> 39;
    m_h = getRandomUInt128() >> 39;
}

__uint128_t polyeight_64_exact::operator()(uint64_t x) {
    __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
    h = fast_large_mult_mod(h, m_c, x);
    h = fast_large_mult_mod(h, m_d, x);
    h = fast_large_mult_mod(h, m_e, x);
    h = fast_large_mult_mod(h, m_f, x);
    h = fast_large_mult_mod(h, m_g, x);
    h = fast_large_mult_mod(h, m_h, x);
    __uint128_t tmp = h + 1;
    return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
}

/* ***************************************************
 * 2-wise independent hashing using carryless multiplication.
 * ***************************************************/

class carrylesstwo_64 {
    uint64_t m_a;
    uint64_t m_b;

public:
    void init();
    uint64_t operator()(uint64_t x);
};

void carrylesstwo_64::init() {
    m_a = getRandomUInt64();
    m_b = getRandomUInt64();
}

uint64_t carrylesstwo_64::operator()(uint64_t x) {
    return gf64_mult(m_a, x) ^ m_b;
}


/* ***************************************************
 * 4-wise independent hashing using carryless multiplication.
 * ***************************************************/

class carrylessfour_64 {
    uint64_t m_a;
    uint64_t m_b;
    uint64_t m_c;
    uint64_t m_d;

public:
    void init();
    uint64_t operator()(uint64_t x);
};

void carrylessfour_64::init() {
    m_a = getRandomUInt64();
    m_b = getRandomUInt64();
    m_c = getRandomUInt64();
    m_d = getRandomUInt64();
}

uint64_t carrylessfour_64::operator()(uint64_t x) {
    uint64_t h = gf64_mult(m_a, x) ^ m_b;
    h = gf64_mult(h, x) ^ m_c;
    h = gf64_mult(h, x) ^ m_d;
    return h;
}

/* ***************************************************
 * 4-wise independent hashing using carryless multiplication.
 * ***************************************************/

class carrylesseight_64 {
    uint64_t m_a;
    uint64_t m_b;
    uint64_t m_c;
    uint64_t m_d;
    uint64_t m_e;
    uint64_t m_f;
    uint64_t m_g;
    uint64_t m_h;

public:
    void init();
    uint64_t operator()(uint64_t x);
};

void carrylesseight_64::init() {
    m_a = getRandomUInt64();
    m_b = getRandomUInt64();
    m_c = getRandomUInt64();
    m_d = getRandomUInt64();
    m_e = getRandomUInt64();
    m_f = getRandomUInt64();
    m_g = getRandomUInt64();
    m_h = getRandomUInt64();
}

uint64_t carrylesseight_64::operator()(uint64_t x) {
    uint64_t h = gf64_mult(m_a, x) ^ m_b;
    h = gf64_mult(h, x) ^ m_c;
    h = gf64_mult(h, x) ^ m_d;
    h = gf64_mult(h, x) ^ m_e;
    h = gf64_mult(h, x) ^ m_f;
    h = gf64_mult(h, x) ^ m_g;
    h = gf64_mult(h, x) ^ m_h;
    return h;
}


#endif // _FAST_HASHING_H_
