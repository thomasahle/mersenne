#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned long long u64;
typedef __uint128_t u128;

// This all works in 64 bits.
 // output bits, and input (m=u)
 // len = 1+k*(k-1)/2;
#ifndef K
   #define K 2
#endif
static const int L = 1+K*(K-1)/2;
// With 32 bits we only need half the L. (Rounded up)
//static const int L = (1+K*(K-1)/2+1)/2;

// Runs of inner hashing loop
#ifndef REPS_I
   #define REPS_I 1e7
#endif

// Runs of outer hashing loop (measures variance of inner loops)
#ifndef REPS_O
   #define REPS_O 1e2
#endif



static //__attribute__((always_inline))
void mult(u64 *xs, u64 y) {
   u128 carry = 0;
   for (int i = 0; i < L; i++) {
      u128 xiy = (u128)xs[i] * y;
      // We have to take care the carry doesn't cause things to overflow
      u128 low = (u128)(u64)xiy + carry;
      xs[i] = (u64) low;
      carry = (xiy >> 64) + (low >> 64);
   }
}

#define mult2(xs, y) { \
   u128 carry = 0; \
   for (int mi = 0; mi < L; mi++) { \
      u128 xiy = (u128)xs[mi] * y; \
      u128 low = (u128)(u64)xiy + carry; \
      xs[mi] = (u64) low; \
      carry = (xiy >> 64) + (low >> 64); \
   } \
}

static //__attribute__((always_inline))
void mult_add(u64 *xs, u64 y, u64 as[L]) {
   u128 carry = 0;
   for (int i = 0; i < L; i++) {
      // We have to take care the carry doesn't cause things to overflow
      u128 xiy = (u128)xs[i] * y;
      u128 low = (u128)(u64)xiy + as[i] + carry;
      xs[i] = (u64) low;
      carry = (xiy >> 64) + (low >> 64);
   }
}


static //__attribute__((always_inline))
void add(u64 *xs, u64 ys[L]) {
   // adds ys to xs
   u64 carry = 0;
   for (int i = 0; i < L; i++) {
      // here we can add the carry with no sweat.
      u128 xiyi = (u128)xs[i] + (u128)ys[i] + carry;
      xs[i] = (u64) xiyi;
      carry = xiyi >> 64;
   }
}

#define add2(xs, ys) { \
   u64 carry = 0; \
   for (int ai = 0; ai < L; ai++) { \
      u128 xiyi = (u128)xs[ai] + (u128)ys[ai] + carry; \
      xs[ai] = (u64) xiyi; \
      carry = xiyi >> 64; \
   } \
}

static //__attribute__((always_inline))
u64 hash(u64 cs[K][L], u64 x) {
   u64 temp[L]; // I tried having this as a global, but it seems the compiler actualy does better when it's local.
   // cs is a `K` by `len` matrix.
   // temp is a `len` vector.

   for (int i = 0; i < L; i++) {
      temp[i] = cs[0][i];
   }
   for (int i = 1; i < K; i++) {
      //mult2(temp, x); // temp = temp * x
      //add2(temp, cs[i]); // temp = temp + cs[i]
      #ifdef MA
         mult_add(temp, x, cs[i]); // temp = temp * x + xs[i]
      #else
         mult(temp, x); // temp = temp * x
         add(temp, cs[i]); // temp = temp + cs[i]
      #endif
   }
   return temp[L-1];
}

static //__attribute__((always_inline))
u64 hash2(u64 cs[K][L], u64 x) {
   u128 c1 = ((u128)cs[0][1] << 64) | cs[0][0];
   u128 c2 = ((u128)cs[1][1] << 64) | cs[1][0];
   return (c1 * (u128)x + c2) >> 64;
}

static //__attribute__((always_inline))
u64 hash3(u128 c1, u128 c2, u64 x) {
   return (c1 * (u128)x + c2) >> 64;
}

static //__attribute__((always_inline))
u64 hash4(u64 cs[K][L], u64 x) {
   u128 low = (u128) x * cs[0][0] + cs[1][0];
   u64 xc2_hi = x * cs[0][1] + cs[1][1] + (low >> 64);
   return xc2_hi;
}





#define timeit(F) {\
  u64 sum = 0;\
  u64 sqs = 0;\
  u64 s = 0;\
  volatile u64 res;\
  for (int i = 0; i < REPS_O; i++) {\
     clock_t start = clock();\
     for (int i = 0; i < REPS_I; i++) {\
        res = F; \
        x += 1;\
     }\
     clock_t diff = clock() - start;\
     u64 musec = diff * 1e6 / CLOCKS_PER_SEC;\
     sum += musec;\
     sqs += musec*musec;\
  }\
  s = x;\
  fprintf(stderr, "%lld\n", s);\
  u64 musec = sum / REPS_O;\
  u64 stdv = sqrt(sqs / REPS_O - musec * musec);\
  printf("Mean time taken %lld.%lld milliseconds +/- %lld.%lld\n", musec/1000, musec%1000, stdv/1000, stdv%1000);\
}\

static void test() {
   u64 cs[K][L];
   for (int i = 0; i < K; i++)
      for (int j = 0; j < L; j++)
         cs[i][j] = (i+1)*(j+1);
   fprintf(stderr, "Test value: %lld\n", hash(cs, 100));
   //for (int i = 0; i < L; i++)
      //printf("%lld\n", temp[i]);
}
/*static void test2() {
   x = x0;
   timeit(hash2(cs, x), ri, ro);
   x = x0;
   u128 c1 = ((u128)cs[0][1] << 64) | cs[0][0];
   u128 c2 = ((u128)cs[1][1] << 64) | cs[1][0];
   //timeit(hash3(c1, c2, vals[i&(1<<10)-1]), ri, ro);
   timeit(hash3(c1, c2, x), ri, ro);
   x = x0;
   timeit(hash4(cs, x), ri, ro);
}
*/
int main() {
   srand(time(0));

   u64 cs[K][L];
   for (int i = 0; i < K; i++)
      for (int j = 0; j < L; j++)
         cs[i][j] = rand();

   u64 x0 = rand();
   u64 x = x0;
   #ifdef NOP
      printf("NOP:\t");
      timeit(x|1); // NOP
      x = x0;
   #endif
   printf("%d-Multiply-shift:\t", K);
   timeit(hash(cs, x));
}
