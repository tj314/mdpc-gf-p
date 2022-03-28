#ifndef MCELIECE_QCMDPC_GF_P_CODE_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_HPP

#include <flintxx.h>
#include <fmpqxx.h>
#include <fmpzxx.h>
#include <fmpz_mod_polyxx.h>
#include <fmpq_polyxx.h>
#include <iostream>
#include <vector>
#include <optional>
#include <cmath>
#include "random.hpp"

using std::vector;
using std::optional;
using flint::fmpzxx;
using flint::fmpq_polyxx;
using flint::fmpz_polyxx;
using flint::fmpz_modxx_ctx;
using flint::fmpz_mod_polyxx;


struct DecodeBounds {
    unsigned q, q_thirds;
    double q_2, q_6, q_18;
    unsigned b1, b2, b3, b4, b5, b6, b7, b8;

    DecodeBounds() = default;

    auto set(unsigned q) -> void;
};


class Code {
private:
    unsigned q_value, k_value;
    fmpzxx q, k;
    fmpz_modxx_ctx context;
    fmpz_mod_polyxx h0, h1, h1_inv;
    fmpz_mod_polyxx mod;

    // the second block should be calculated as follows:
    // -(h1_inv*h0)^T
    // here we omit the transposition and encode accordingly
    fmpz_mod_polyxx second_block_G;

    DecodeBounds bounds;

    auto calculate_syndrome(const vector<fmpzxx>& ciphertext) -> vector<fmpzxx>;
    auto decide(vector<fmpzxx>& error_vector, const vector<fmpzxx>& syndrome) -> void;
    auto transform(vector<fmpzxx>& error_vetor) -> void;
public:
    Code(unsigned q, unsigned k);
    auto init_keys() -> void;
    auto encode(const vector<fmpzxx>& plaintext) -> vector<fmpzxx>;
    auto decode(const vector<fmpzxx>& ciphertext, unsigned num_iterations) -> optional<vector<fmpzxx>>;
};

#endif
