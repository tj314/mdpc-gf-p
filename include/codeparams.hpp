#ifndef MCELIECE_QCMDPC_GF_P_CODE_PARAMS_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_PARAMS_HPP

#include <flintxx.h>
#include <fmpqxx.h>
#include <fmpzxx.h>
#include <fmpz_mod_polyxx.h>

using flint::fmpzxx;
using flint::fmpz_modxx_ctx;
using flint::fmpz_mod_polyxx;

struct CodeParams {
    unsigned q_value, k_value;
    fmpzxx q, k;
    fmpz_modxx_ctx context;

    CodeParams(unsigned q, unsigned k);
};

#endif
