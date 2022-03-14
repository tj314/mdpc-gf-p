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
private:
    Random();

    // this is not needed now, but will be needed later
    // DO NOT remove
    static auto get() -> Random&;

    auto integer_internal(unsigned bound) -> unsigned;
    auto poly_internal(fmpq_polyxx& output, const CodeParams& params, unsigned add_to_first = 0) -> void;
    auto poly_internal(fmpz_mod_polyxx& output, const CodeParams& params, unsigned add_to_first = 0) -> void;
    auto error_vector_internal(const CodeParams& params) -> vector<fmpzxx>;

public:
    Random(const Random& r) = delete;
    static auto integer(unsigned bound) -> unsigned;
    static auto poly(fmpq_polyxx& output, const CodeParams& params, unsigned add_to_first = 0) -> void;
    static auto poly(fmpz_mod_polyxx& output, const CodeParams& params, unsigned add_to_first = 0) -> void;
    static auto error_vector(const CodeParams& params) -> vector<fmpzxx>;

};
#endif
