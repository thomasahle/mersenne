#include <stdio.h>
#include <gmp.h>
#include <assert.h>
#include <time.h>
#include <math.h>

void crandall(mpz_t x, mp_bitcnt_t b, mpz_t q, mpz_t r,
      mpz_t q0, mpz_t r0, mpz_t t, mpz_t p
      ) {
  mpz_tdiv_q_2exp(q0, x, b);
  mpz_tdiv_r_2exp(r0, x, b);
  mpz_set(q, q0);
  mpz_set(r, r0);
  while (mpz_sgn(q0) == 1) {
     mpz_set(t, q0); // t = q0 * c
     mpz_tdiv_q_2exp(q0, t, b);
     mpz_tdiv_r_2exp(r0, t, b);
     mpz_add(q, q, q0);
     mpz_add(r, r, r0);
  }
  while (mpz_cmp(r, p) != -1) {
     mpz_sub(r, r, p);
     mpz_add_ui(q, q, 1);
  }
}

void our_method(mpz_t x, mp_bitcnt_t b, int m, mpz_t z) {
   // Note, this function will replace x with x+1
  mpz_add_ui(x, x, 1); // x = x + 1
  mpz_tdiv_q_2exp(z, x, b);
  for (int i = 0; i < m-1; i++) {
     mpz_add(z, z, x); // z = z + x
     mpz_tdiv_q_2exp(z, z, b); // z = z >> b
  }
}



#define timeit(F, REPS_I, REPS_O) {\
  sum = 0;\
  sqs = 0;\
  mpz_init(s);\
  for (int i = 0; i < REPS_O; i++) {\
     start = clock();\
     for (int i = 0; i < REPS_I; i++) {\
        F; \
        mpz_add(s, s, q);\
        mpz_add_ui(x, x0, i);\
     }\
     diff = clock() - start;\
     msec = diff * 1000 / CLOCKS_PER_SEC;\
     sum += msec;\
     sqs += msec*msec;\
  }\
  mpz_out_str(stderr, 10, s);\
  printf("\n");\
  msec = sum / REPS_O;\
  stdv = sqrt(sqs / REPS_O - msec * msec);\
  printf("Mean time taken %d seconds %d milliseconds +/- %d\n", msec/1000, msec%1000, stdv);\
}\


#define timeit_table(F, REPS) {\
  sum = 0;\
  mpz_init(s);\
  start = clock();\
  for (int i = 0; i < REPS; i++) {\
     F; \
     mpz_add_ui(x, x0, i);\
     mpz_add(s, s, q);\
  }\
  diff = clock() - start;\
  msec = diff * 1000 / CLOCKS_PER_SEC;\
  mpz_out_str(stderr, 10, s);\
  printf("%d ", msec);\
}\

int main2(){
   // For doing a single `b` and with stddev
  mpz_t x;
  mpz_init(x);
  mpz_t r;
  mpz_init(r);
  mpz_t q;
  mpz_init(q);

  mpz_t x0;
  mpz_init(x0);
  gmp_randstate_t state;
  gmp_randinit_default(state);
  mpz_urandomb(x0, state, 512);

  mp_bitcnt_t b = 256;
  int m = 2;

  int rep_inner = 1e7;
  int rep_outer = 1e2;

  mpz_t s;
  clock_t start, diff;
  int msec, sum, sqs, stdv;

  printf("Running NOP\n");
  timeit(
     mpz_add_ui(q, q, 1),
     rep_inner, rep_outer);

  printf("Running Crandall\n");
  mpz_t q0, r0, t, p;
  mpz_init(q0);
  mpz_init(r0);
  mpz_init(t);
  mpz_init_set_ui(p, 1);
  mpz_mul_2exp(p, p, b);
  mpz_sub_ui(p, p, 1);
  timeit(
     crandall(x, b, q, r,  q0, r0, t, p),
     rep_inner, rep_outer);

  printf("Running Ours\n");
  timeit(
     our_method(x, b, m, q),
     rep_inner, rep_outer);

  printf("Running Gnu\n");
  timeit(
     mpz_tdiv_q(q, x, p),
     rep_inner, rep_outer);
}


int main(){
   // For making tables
  mpz_t x, r, q, x0;
  mpz_init(r);
  mpz_init(q);
  mpz_init(x0);
  mpz_init(x);

  gmp_randstate_t state;
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));

  int rep = 1e7;

  printf("ms per %d divisions\n", rep);
  printf("NOP, Crand, Us, Gnu\n");

  for (mp_bitcnt_t b = 32; b <= 1024; b *= 2) {
     //mpz_urandomb(x0, state, 2*b);
     mpz_set_ui(x0, 1); // Set x0 to 2^(2b) - reps, rather than random
     mpz_mul_2exp(x0, x0, 2*b-25); // Should subtract at least log2(rep)
     int m = 2;
     printf("b = %d, ", b);

     mpz_t s;
     clock_t start, diff;
     int msec, sum, sqs, stdv;

     // Time NOP
     timeit_table(
        mpz_add_ui(q, q, 1),
        rep);

     // Time Crandall. The method has some temp vars we init here.
     mpz_t q0, r0, t, p;
     mpz_init(q0);
     mpz_init(r0);
     mpz_init(t);
     mpz_init(p);
     mpz_set_ui(p, 1);
     mpz_mul_2exp(p, p, b);
     mpz_sub_ui(p, p, 1);
     timeit_table(
        crandall(x, b, q, r,  q0, r0, t, p),
        rep);

     timeit_table(
        our_method(x, b, m, q),
        rep);

     timeit_table(
        mpz_tdiv_q(q, x, p),
        rep);

     printf("\n");
   }
}
