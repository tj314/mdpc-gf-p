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
#include <algorithm>
#include "utils.hpp"

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
    auto poly_internal(unsigned k, unsigned add_to_first = 0) -> vector<int>;
    auto error_vector_internal(unsigned k) -> vector<int>;

public:
    Random(const Random& r) = delete;
    static auto integer(unsigned bound) -> unsigned;
    static auto poly(fmpq_polyxx& output, unsigned k, unsigned add_to_first = 0) -> void;
    static auto poly(fmpz_mod_polyxx& output, unsigned k, unsigned q, unsigned add_to_first = 0) -> void;
    static auto poly(vector<unsigned>& output, unsigned k, unsigned q, unsigned add_to_first=0) -> void;
    static auto error_vector(unsigned k) -> vector<int>;
    static auto reseed() -> void;
};
#endif
