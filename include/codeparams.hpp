#ifndef MCELIECE_QCMDPC_GF_P_CODE_PARAMS_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_PARAMS_HPP

#include <flint.h>
#include <fmpq.h>
#include <fmpz.h>
#include <fmpz_mod_poly.h>

struct CodeParams {
    unsigned q_value, k_value;
    fmpz_t q, k;
    fmpz_mod_ctx_t ctx;
    fmpz_mod_poly_t modulus;

    CodeParams() = default;
    ~CodeParams();
    auto set(unsigned q, unsigned k) -> void;
};

#endif