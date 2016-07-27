#include "gmp.h"

void generate_prime(mpz_t rop, mpz_t max);
void find_phi(mpz_t rop, mpz_t p, mpz_t q);
void generate_e(mpz_t rop, mpz_t n);
void find_d(mpz_t rop, mpz_t e, mpz_t phi_n);
void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n);
void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n);
