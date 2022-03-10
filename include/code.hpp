#ifndef MCELIECE_QCMDPC_GF_P_CODE_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_HPP

#include <flintxx.h>
#include <fmpqxx.h>
#include <fmpzxx.h>
#include <fmpz_mod_polyxx.h>
#include <fmpq_polyxx.h>
#include <iostream>
#include <cmath>
#include "random.hpp"

using namespace flint;

class Code {
private:
    CodeParams params;  // DO NOT MOVE THIS
                        // params MUST be the first member in this class
    fmpz_mod_polyxx h0, h1, h1_inv;
    // fmpq_polyxx h1_tmp;

public:
    Code(unsigned q, unsigned k, Random& rnd);
    auto init_keys(Random& rnd) -> void;
    auto encode() -> void;
    auto decode(void) -> void;
};

#endif
