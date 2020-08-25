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

class multishift_64 {
#ifdef DEBUG
    bool hasInit;
#endif
    // using uint_large_t = typename conditional<sixtyfour, __uint128_t, uint64_t>::type;
    // using uint_t = typename conditional<sixtyfour, uint64_t, uint32_t>::type;

    __uint128_t m_a, m_b;

public:
    uint64_t operator()(uint64_t x);
    void init();
};


void multishift_64::init() {
#ifdef SEEDED_RANDOM
    m_a = getRandomUInt128();
    m_b = getRandomUInt128();
#endif
#ifdef DEBUG
    hasInit=true;
#endif
}

uint64_t multishift_64::operator()(uint64_t x) {
#ifdef DEBUG
    assert(hasInit);
#endif
    return (m_a * (__uint128_t)x + m_b) >> 64;
}

class multishift_32 {
#ifdef DEBUG
    bool hasInit;
#endif
    // using uint_large_t = typename conditional<sixtyfour, __uint128_t, uint64_t>::type;
    // using uint_t = typename conditional<sixtyfour, uint64_t, uint32_t>::type;

    uint32_t m_a, m_b;

public:
    uint32_t operator()(uint32_t x);
    void init();
};


void multishift_32::init() {
#ifdef SEEDED_RANDOM
    m_a = getRandomUInt64();
    m_b = getRandomUInt64();
#endif
#ifdef DEBUG
    hasInit=true;
#endif
}

uint32_t multishift_32::operator()(uint32_t x) {
#ifdef DEBUG
    assert(hasInit);
#endif
    return (m_a * (uint64_t)x + m_b) >> 32;
}


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
 * 4-wise independent hashing using polyhash.
 * ***************************************************/

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
    return h & -((__int128_t)(h - m_p) >> 89);
    // __uint128_t tmp = h + 1;
    // return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
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
    return h & -((__int128_t)(h - m_p) >> 89);
    // __uint128_t tmp = h + 1;
    // return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
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
    return h & -((__int128_t)(h - m_p) >> 89);
    // __uint128_t tmp = h + 1;
    // return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
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


/* ***************************************************
 * 2-wise independent hashing using polyhash.
 * ***************************************************/

class polytwo_32 {
    uint64_t m_a;
    uint64_t m_b;

    constexpr static uint64_t m_p = ((uint64_t) 1 << 61) - 1;
public:
    void init();
    uint64_t operator()(uint32_t x);
};

void polytwo_32::init() {
    m_a = getRandomUInt64() >> 3;
    m_b = getRandomUInt64() >> 3;
}

uint64_t polytwo_32::operator()(uint32_t x) {
    __uint128_t h = (__uint128_t)m_a * x + m_b;
    h = (h & m_p) + (h >> 61);
    if (h >= m_p) h -= m_p;

    return (uint64_t)h;
}

/* ***************************************************
 * 4-wise independent hashing using polyhash.
 * ***************************************************/

class polyfour_32 {
    uint64_t m_a;
    uint64_t m_b;
    uint64_t m_c;
    uint64_t m_d;

    constexpr static uint64_t m_p = ((__uint128_t) 1 << 61) - 1;
public:
    void init();
    uint64_t operator()(uint32_t x);
};

void polyfour_32::init() {
    m_a = getRandomUInt64() >> 3;
    m_b = getRandomUInt64() >> 3;
    m_c = getRandomUInt64() >> 3;
    m_d = getRandomUInt64() >> 3;
}

uint64_t polyfour_32::operator()(uint32_t x) {
    uint64_t y = x;
    __uint128_t h = (__uint128_t)m_a * y + m_b;
    h = (h & m_p) + (h >> 61);
    h = (__uint128_t)h * y + m_c;
    h = (h & m_p) + (h >> 61);
    h = (__uint128_t)h * y + m_d;
    h = (h & m_p) + (h >> 61);
    if (h >= m_p) h -= m_p;

    return (uint64_t)h;
}

/* ***************************************************
 * 4-wise independent hashing using polyhash.
 * ***************************************************/

class polyeight_32 {
    uint64_t m_a;
    uint64_t m_b;
    uint64_t m_c;
    uint64_t m_d;
    uint64_t m_e;
    uint64_t m_f;
    uint64_t m_g;
    uint64_t m_h;

    constexpr static uint64_t m_p = ((uint64_t) 1 << 61) - 1;
public:
    void init();
    uint64_t operator()(uint32_t x);
};

void polyeight_32::init() {
    m_a = getRandomUInt64() >> 3;
    m_b = getRandomUInt64() >> 3;
    m_c = getRandomUInt64() >> 3;
    m_d = getRandomUInt64() >> 3;
    m_d = getRandomUInt64() >> 3;
    m_e = getRandomUInt64() >> 3;
    m_f = getRandomUInt64() >> 3;
    m_g = getRandomUInt64() >> 3;
    m_h = getRandomUInt64() >> 3;
}

uint64_t polyeight_32::operator()(uint32_t x) {
    __uint128_t h = (__uint128_t)m_a * x + m_b;
    h = (uint64_t)(h & m_p + (h >> 61));
    h = (__uint128_t)h * x + m_c;
    h = (uint64_t)(h & m_p + (h >> 61));
    h = (__uint128_t)h * x + m_d;
    h = (uint64_t)(h & m_p + (h >> 61));
    h = (__uint128_t)h * x + m_e;
    h = (uint64_t)(h & m_p + (h >> 61));
    h = (__uint128_t)h * x + m_f;
    h = (uint64_t)(h & m_p + (h >> 61));
    h = (__uint128_t)h * x + m_g;
    h = (uint64_t)(h & m_p + (h >> 61));
    h = (__uint128_t)h * x + m_h;
    h = (uint64_t)(h & m_p + (h >> 61));
    if (h >= m_p) h -= m_p;

    return h;
}

// /* ***************************************************
//  * 2-wise independent hashing using polyhash using exact mod.
//  * ***************************************************/
// class polytwo_64_exact {
//     __uint128_t m_a;
//     __uint128_t m_b;

//     constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
// public:
//     void init();
//     __uint128_t operator()(uint64_t x);
// };

// void polytwo_64_exact::init() {
//     m_a = getRandomUInt128() >> 39;
//     m_b = getRandomUInt128() >> 39;
// }

// __uint128_t polytwo_64_exact::operator()(uint64_t x) {
//     __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
//     return h & -((__int128_t)(h - m_p) >> 89);
//     // __uint128_t tmp = h + 1;
//     // return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
// }

// /* ***************************************************
//  * 4-wise independent hashing using polyhash using exact mod.
//  * ***************************************************/

// class polyfour_64_exact {
//     __uint128_t m_a;
//     __uint128_t m_b;
//     __uint128_t m_c;
//     __uint128_t m_d;

//     constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
// public:
//     void init();
//     __uint128_t operator()(uint64_t x);
// };

// void polyfour_64_exact::init() {
//     m_a = getRandomUInt128() >> 39;
//     m_b = getRandomUInt128() >> 39;
//     m_c = getRandomUInt128() >> 39;
//     m_d = getRandomUInt128() >> 39;
// }

// __uint128_t polyfour_64_exact::operator()(uint64_t x) {
//     __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
//     h = fast_large_mult_mod(h, m_c, x);
//     h = fast_large_mult_mod(h, m_d, x);
//     return h & -((__int128_t)(h - m_p) >> 89);
//     // __uint128_t tmp = h + 1;
//     // return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
// }

// /* ***************************************************
//  * 8-wise independent hashing using polyhash using exact mod.
//  * ***************************************************/

// class polyeight_64_exact {
//     __uint128_t m_a;
//     __uint128_t m_b;
//     __uint128_t m_c;
//     __uint128_t m_d;
//     __uint128_t m_e;
//     __uint128_t m_f;
//     __uint128_t m_g;
//     __uint128_t m_h;

//     constexpr static __uint128_t m_p = ((__uint128_t) 1 << 89) - 1;
// public:
//     void init();
//     __uint128_t operator()(uint64_t x);
// };

// void polyeight_64_exact::init() {
//     m_a = getRandomUInt128() >> 39;
//     m_b = getRandomUInt128() >> 39;
//     m_c = getRandomUInt128() >> 39;
//     m_d = getRandomUInt128() >> 39;
//     m_e = getRandomUInt128() >> 39;
//     m_f = getRandomUInt128() >> 39;
//     m_g = getRandomUInt128() >> 39;
//     m_h = getRandomUInt128() >> 39;
// }

// __uint128_t polyeight_64_exact::operator()(uint64_t x) {
//     __uint128_t h = fast_large_mult_mod(m_a, m_b, x);
//     h = fast_large_mult_mod(h, m_c, x);
//     h = fast_large_mult_mod(h, m_d, x);
//     h = fast_large_mult_mod(h, m_e, x);
//     h = fast_large_mult_mod(h, m_f, x);
//     h = fast_large_mult_mod(h, m_g, x);
//     h = fast_large_mult_mod(h, m_h, x);
//     return h & -((__int128_t)(h - m_p) >> 89);
//     // __uint128_t tmp = h + 1;
//     // return (h + (((tmp >> 89) + tmp) >> 89)) & m_p;
// }

/* ***************************************************
 * 2-wise independent hashing using carryless multiplication.
 * ***************************************************/

class carrylesstwo_32 {
    uint32_t m_a;
    uint32_t m_b;

public:
    void init();
    uint32_t operator()(uint32_t x);
};

void carrylesstwo_32::init() {
    m_a = getRandomUInt32();
    m_b = getRandomUInt32();
}

uint32_t carrylesstwo_32::operator()(uint32_t x) {
    return gf32_mult(m_a, x) ^ m_b;
}


/* ***************************************************
 * 4-wise independent hashing using carryless multiplication.
 * ***************************************************/

class carrylessfour_32 {
    uint32_t m_a;
    uint32_t m_b;
    uint32_t m_c;
    uint32_t m_d;

public:
    void init();
    uint32_t operator()(uint32_t x);
};

void carrylessfour_32::init() {
    m_a = getRandomUInt32();
    m_b = getRandomUInt32();
    m_c = getRandomUInt32();
    m_d = getRandomUInt32();
}

uint32_t carrylessfour_32::operator()(uint32_t x) {
    uint32_t h = gf32_mult(m_a, x) ^ m_b;
    h = gf32_mult(h, x) ^ m_c;
    h = gf32_mult(h, x) ^ m_d;
    return h;
}

/* ***************************************************
 * 4-wise independent hashing using carryless multiplication.
 * ***************************************************/

class carrylesseight_32 {
    uint32_t m_a;
    uint32_t m_b;
    uint32_t m_c;
    uint32_t m_d;
    uint32_t m_e;
    uint32_t m_f;
    uint32_t m_g;
    uint32_t m_h;

public:
    void init();
    uint32_t operator()(uint32_t x);
};

void carrylesseight_32::init() {
    m_a = getRandomUInt32();
    m_b = getRandomUInt32();
    m_c = getRandomUInt32();
    m_d = getRandomUInt32();
    m_e = getRandomUInt32();
    m_f = getRandomUInt32();
    m_g = getRandomUInt32();
    m_h = getRandomUInt32();
}

uint32_t carrylesseight_32::operator()(uint32_t x) {
    uint32_t h = gf32_mult(m_a, x) ^ m_b;
    h = gf32_mult(h, x) ^ m_c;
    h = gf32_mult(h, x) ^ m_d;
    h = gf32_mult(h, x) ^ m_e;
    h = gf32_mult(h, x) ^ m_f;
    h = gf32_mult(h, x) ^ m_g;
    h = gf32_mult(h, x) ^ m_h;
    return h;
}


#endif // _FAST_HASHING_H_
