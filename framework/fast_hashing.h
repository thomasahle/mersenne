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

/* ***************************************************
 * 2-wise independent hashing using polyhash using exact mod.
 * ***************************************************/
class polytwo_64_exact {
    __uint128_t m_a;
    __uint128_t m_b;

    constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
public:
    void init();
    uint64_t operator()(uint64_t x);
};

void polytwo_64_exact::init() {
    m_a = getRandomUInt128() >> 39;
    m_b = getRandomUInt128() >> 39;
}

uint64_t polytwo_64_exact::operator()(uint64_t x) {
    __uint128_t h = fast_large_mult_mod_exact(m_a, m_b, x);

    return (uint64_t)h;
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
    // return (uint64_t)h;
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


#endif // _FAST_HASHING_H_
