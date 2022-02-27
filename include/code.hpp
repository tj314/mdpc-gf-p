#ifndef MCELIECE_QCMDPC_GF_P_CODE_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_HPP

#include <flint.h>
#include <fmpq.h>
#include <fmpz.h>
#include <fmpz_mod_poly.h>
#include <iostream>
#include "random.hpp"

class Code {
private:
        fmpz_t q, k;
        fmpz_mod_ctx_t ctx;
        fmpz_mod_poly_t modulus;
        
        auto generate_random_poly(fmpz_mod_poly_t output, Random& rnd) const -> void;

    public:
        Code(unsigned q, unsigned k, Random& rnd);
        ~Code();
        auto init_keys(Random& rnd) -> void;
        auto encode() -> void;
        auto decode(void) -> void;
};

#endif