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

#ifdef DEBUG
#include <cassert>
#endif

#include "randomgen.h"

using namespace std;

/**
 * Calculate the product between two numbers in the finite field
 * GF(2^64)
 **/
inline const uint64_t gf64_mult(const uint64_t a, const uint64_t b) {
    __m128i a_128 = _mm_setr_epi32((uint32_t) a, (uint32_t) (a >> 32), 0, 0);
    __m128i b_128 = _mm_setr_epi32((uint32_t) b, (uint32_t) (b >> 32), 0, 0);

    __m128i c_128 = _mm_clmulepi64_si128(a_128, b_128, 0);
    uint64_t res[2];
    memcpy(res, &c_128, sizeof(res));

    // Calculate the result modulo x^64 + x^4 + x^3 + x + 1
    res[1] ^= (res[1] >> 60) ^ (res[1] >> 61) ^ (res[1] >> 63);
    return res[0] ^ (res[1]) ^ (res[1] << 1) ^ (res[1] << 3) ^ (res[1] << 4);
}


// /**
//  * Calculate the product between two numbers in the finite field
//  * GF(2^32)
//  **/
// inline const uint32_t gf64_multiplication(const uint32_t a, const uint32_t b) {
//     __m128i a_128 = _mm_setr_epi32(a, 0, 0, 0);
//     __m128i b_128 = _mm_setr_epi32(b, 0, 0, 0);

//     __m128i c_128 = _mm_clmulepi64_si128(a_128, b_128, 0);
//     uint32_t res[4];
//     memcpy(res, &c_128, sizeof(res));

//     // Calculate the result modulo x^32 + x^22 + x^2 + x^1 + 1
//     res[1] ^= (res[1] >> 10) ^ (res[1] >> 30) ^ (res[1] >> 31);
//     return res[0] ^ (res[1]) ^ (res[1] << 1) ^ (res[1] << 2) ^ (res[1] << 22);
// }

/**
 * Takes three integers a, b, x and calculates approximately 
 * (ax + b) mod p where p = 2^89 - 1. It will return a number,
 * y, that satisfies 0 <= y < 2p.
 **/
inline const __uint128_t fast_large_mult_mod(const __uint128_t a, const __uint128_t b, const uint64_t x) {
    __uint128_t fst_a = a >> 64;
    __uint128_t scd_a = (uint64_t)a;

    __uint128_t fst_b = b >> 64;    
    __uint128_t scd_b = (uint64_t)b;

    __uint128_t c = scd_a * x + scd_b;
    __uint128_t d = fst_a * x + fst_b + (c >> 64);

    return ((d & (((__uint128_t) 1 << 25) - 1)) << 64) + (__uint128_t)(uint64_t)c
        + (d >> 25);
}

/**
 * Takes three integers a, b, x and calculates (ax + b) mod p where p = 2^89 - 1.
 **/
inline const __uint128_t fast_large_mult_mod_exact( const __uint128_t  a,  const __uint128_t  b,  const uint64_t  x) {
    __uint128_t fst_a = a >> 64;
    __uint128_t scd_a = (uint64_t)a;

    __uint128_t low = scd_a * x;
    __uint128_t high = fst_a * x;

    __uint128_t c_div = low + (uint64_t)(b + 1);
    __uint128_t d_div = high + ((b + 1) >> 64) + (c_div >> 64);

    __uint128_t e_div = d_div >> 25;
    c_div = (__uint128_t)(uint64_t)c_div + (__uint128_t)(uint64_t)e_div;
    d_div += (c_div >> 64) + (e_div >> 64);

    __uint128_t e = b + d_div >> 25;

    __uint128_t c_mod = low + (uint64_t)e;
    __uint128_t d_mod = high + (e >> 64) + (c_mod >> 64);

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
