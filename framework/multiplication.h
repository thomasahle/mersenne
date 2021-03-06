/* ***********************************************
 * Hash functions:
 * This file define several hash functions as
 * classes.
 *
 * If DEBUG is defined it asserts that initialization
 * is done properly.
 * ***********************************************/

#ifndef _MULTIPLICATION_H_
#define _MULTIPLICATION_H_

#include <x86intrin.h>
#include <cstring>

#ifdef DEBUG
#include <cassert>
#endif

#include "randomgen.h"

using namespace std;

/**
 * Calculate the product between two numbers in the finite field
 * GF(2^64)
 **/
const __m128i P64 = _mm_set_epi64x(0, (uint64_t)1 + ((uint64_t)1<<1) + ((uint64_t)1<<3) + ((uint64_t)1<<4));
static inline const uint64_t gf64_mult(const uint64_t a, const uint64_t b) {
    __m128i a_128 = _mm_set_epi64x(0, a);
    __m128i b_128 = _mm_set_epi64x(0, b);

    __m128i c_128 = _mm_clmulepi64_si128(a_128, b_128, 0);

    __m128i q1 = _mm_srli_si128(c_128, 8);
    __m128i q2 = _mm_clmulepi64_si128(q1, P64, 0);
    __m128i q3 = _mm_srli_si128(q2, 8);
    __m128i q4 = _mm_clmulepi64_si128(q3, P64, 0);

    return _mm_cvtsi128_si64(_mm_xor_si128(c_128, _mm_xor_si128(q2, q4)));
}



// /**
//  * Calculate the product between two numbers in the finite field
//  * GF(2^32)
//  **/
const __m128i P32 = _mm_set_epi64x(0, (uint64_t)1 + ((uint64_t)1<<2) + ((uint64_t)1<<6) + ((uint64_t)1<<7) + ((uint64_t)1<<32));
static inline const uint64_t gf32_mult(const uint64_t a, const uint64_t b) {
    __m128i a_128 = _mm_set_epi64x(0, a);
    __m128i b_128 = _mm_set_epi64x(0, b);

    __m128i c_128 = _mm_clmulepi64_si128(a_128, b_128, 0);

    __m128i q1 = _mm_srli_epi64(c_128, 32);
    __m128i q2 = _mm_clmulepi64_si128(q1, P32, 0);
    __m128i q3 = _mm_srli_epi64(q2, 32);
    __m128i q4 = _mm_clmulepi64_si128(q3, P32, 0);

    return _mm_cvtsi128_si64(_mm_xor_si128(c_128, q4));
}

/**
 * Takes three integers a, b, x and calculates approximately 
 * (ax + b) mod p where p = 2^89 - 1. It assumes that a, b <= 2p.
 * It will return a number, y, that satisfies 0 <= y < 2p.
 **/
// static inline const __uint128_t fast_large_mult_mod(const __uint128_t a, const __uint128_t b, const uint64_t x) {
//     __uint128_t fst_a = a >> 64;
//     __uint128_t scd_a = (uint64_t)a;

//     __uint128_t fst_b = b >> 64;    
//     __uint128_t scd_b = (uint64_t)b;

//     __uint128_t c = scd_a * x + scd_b;
//     __uint128_t d = fst_a * x + fst_b + (c >> 64);

//     return ((d & (((__uint128_t) 1 << 25) - 1)) << 64) + (__uint128_t)(uint64_t)c
//         + (d >> 25);
// }
static inline __uint128_t fast_large_mult_mod(__uint128_t a, __uint128_t b, uint64_t x) {
    // uint64_t fst_a = (uint64_t)(a >> 64);
    // uint64_t scd_a = (uint64_t)a;

    // uint64_t fst_b = (uint64_t)(b >> 64);    
    // uint64_t scd_b = (uint64_t)b;
    __uint128_t mul_low = (__uint128_t)(uint64_t)a * (__uint128_t)x;
    __uint128_t mul_high = (__uint128_t)(uint64_t)(a >> 64) * (__uint128_t)x;


    __uint128_t c = mul_low + (__uint128_t)(uint64_t)b;
    __uint128_t d = mul_high + (b >> 64) + (c >> 64);

    return ((d & (((uint64_t)1 << 25) - 1)) << 64) + (uint64_t)c + (d >> 25);
}

/**
 * Takes three integers a, b, x and calculates (ax + b) mod p where p = 2^89 - 1.
 **/
inline const __uint128_t fast_large_mult_mod_exact( const __uint128_t  a,  const __uint128_t  b,  const uint64_t  x) {
    __uint128_t fst_a = (uint64_t)(a >> 64);
    __uint128_t scd_a = (uint64_t)a;

    // __uint128_t fst_b = (uint64_t)(b >> 64);    
    // __uint128_t scd_b = (uint64_t)b;

    __uint128_t low = scd_a * x;
    __uint128_t high = fst_a * x;

    __uint128_t c_div = low + (uint64_t)(b + 1);
    __uint128_t d_div = high + ((b + 1) >> 64) + (c_div >> 64);

    __uint128_t e_div = d_div >> 25;
    c_div = (uint64_t)c_div + e_div;
    d_div = d_div + (c_div >> 64);

    __uint128_t e = b + (d_div >> 25);
    __uint128_t c_mod = low + (uint64_t)e;
    __uint128_t d_mod = high + (e >> 64);

    // // (2^89 - 1)(2^64 - 1) + (2^89 - 1) = 2^64(2^89 - 1)

    // // c_div = (uint64_t)c_div + (d_div >> 25);
    // // // d_div += (c_div >> 64);

    // // __uint128_t c_mod = low + (uint64_t)b + ((d_div + (c_div >> 64)) >> 25);
    // // __uint128_t d_mod = high + (b >> 64) + (c_mod >> 64);

    // // return ((d_mod & (((__uint128_t) 1 << 25) - 1)) << 64) + (uint64_t)c_mod;

    // // __uint128_t fst_a = a >> 64;
    // // __uint128_t scd_a = (uint64_t)a;

    // // __uint128_t low = scd_a * x;
    // // __uint128_t high = fst_a * x;

    // // __uint128_t c_div = low + (uint64_t)(b + 1);
    // // __uint128_t d_div = high + ((b + 1) >> 64) + (c_div >> 64);

    // // __uint128_t div = (d_div + (((uint64_t)c_div + (d_div >> 25)) >> 64)) >> 25;

    // // __uint128_t e = (b + div) >> 25;
    // // __uint128_t c_mod = low + (uint64_t)e;
    // // __uint128_t d_mod = high + (e >> 64) + (c_mod >> 64);
    // // return (__uint128_t)(uint64_t)c_mod + ((d_mod & (((__uint128_t) 1 << 25) - 1)) << 64);


    // __uint128_t e_div = d_div >> 25;
    // c_div = (__uint128_t)(uint64_t)c_div + (__uint128_t)(uint64_t)e_div;
    // d_div += (c_div >> 64) + (e_div >> 64);

    // __uint128_t e = b + d_div >> 25;

    // __uint128_t c_mod = low + (uint64_t)e;
    // __uint128_t d_mod = high + (e >> 64) + (c_mod >> 64);

    return (__uint128_t)(uint64_t)c_mod + ((d_mod & (((__uint128_t) 1 << 25) - 1)) << 64);
}


// inline const __uint128_t fast_large_mult_mod_exact(const __uint128_t a, const __uint128_t b, const __uint128_t x) {
//     __uint128_t fst_a = a >> 64;
//     __uint128_t scd_a = (uint64_t)a;

//     __uint128_t fst_b = (b + 1) >> 64;    
//     __uint128_t scd_b = (uint64_t)(b + 1);

//     __uint128_t fst_x = x >> 64;    
//     __uint128_t scd_x = (uint64_t)x;

//     __uint128_t low  = scd_a * scd_x;
//     __uint128_t mid  = fst_a * scd_x + scd_a * fst_x;
//     __uint128_t high = fst_a * fst_x;

//     __uint128_t low_div = 


//     __uint128_t c = scd_a * scd_x + scd_b;
//     __uint128_t d = fst_a * scd_x + scd_a * fst_x + fst_b + (c >> 64);
//     __uint128_t e = fst_a * fst_x + (d >> 64);

//     c = (uint64_t)c;
//     d = (uint64_t)d;

//     __uint128_t f = (d >> 25) + (e << 39);

//     __uint128_t c_nxt = c + (uint64_t)f;
//     __uint128_t d_nxt = d + (f >> 64) + (c >> 64);
//     __uint128_t e_nxt = e + (d >> 64);

//     f = ((uint64_t)d_nxt >> 25) + (e_nxt << 39);


//     return ((d & (((__uint128_t) 1 << 25) - 1)) << 64) + (__uint128_t)(c & ((uint64_t) - 1))
//         + (d >> 25) + (e << 39);
// }

#endif // _MULTIPLICATION_H_
