#ifndef MCELIECE_QCMDPC_GF_P_RANDOM_HPP
#define MCELIECE_QCMDPC_GF_P_RANDOM_HPP

#include <flintxx.h>
#include <fmpzxx.h>
#include <fmpqxx.h>
#include <fmpz_mod_polyxx.h>
#include <fmpq_polyxx.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "utils.hpp"

using flint::fmpq_polyxx;
using flint::fmpzxx;
using flint::fmpqxx;
using flint::fmpz_mod_polyxx;
using std::vector;
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

class Random {
private:
    mt19937 engine{random_device{}()};

    Random() = default;
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
};
#endif
