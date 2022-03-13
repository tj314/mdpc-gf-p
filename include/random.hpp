#ifndef MCELIECE_QCMDPC_GF_P_RANDOM_HPP
#define MCELIECE_QCMDPC_GF_P_RANDOM_HPP

#include <time.h>
#include <stdlib.h>
#include <flintxx.h>
#include <fmpzxx.h>
#include <fmpqxx.h>
#include <fmpz_mod_polyxx.h>
#include <fmpq_polyxx.h>
#include <iostream>
#include <vector>
#include "codeparams.hpp"

using flint::fmpq_polyxx;
using flint::fmpzxx;
using flint::fmpqxx;
using flint::fmpz_mod_polyxx;
using std::vector;

class Random {
public:
    Random();

    auto integer(unsigned bound) -> unsigned;

    auto poly(fmpq_polyxx& output, const CodeParams& params, unsigned add_to_first = 0) -> void;
    auto poly(fmpz_mod_polyxx& output, const CodeParams& params, unsigned add_to_first = 0) -> void;

    auto error_vector(const CodeParams& params) -> vector<fmpzxx>;
};
#endif
