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
    // auto poly_internal(fmpq_polyxx& output, const CodeParams& params, unsigned add_to_first = 0) -> void;
    // auto poly_internal(fmpz_mod_polyxx& output, const CodeParams& params, unsigned add_to_first = 0) -> void;

    template<typename Poly, typename PolyElem>
    auto poly_internal(Poly& output, unsigned k, unsigned add_to_first) -> void {
        unsigned third = k / 3;
        unsigned i;

        // set the first third of poly to 1
        // set the second third to -1
        // and the rest to 0
        for (i = 0; i < third; ++i) {
            output.set_coeff(i, 1);
        }
        for (; i < 2*third; ++i) {
            output.set_coeff(i, -1);
        }
        for (; i < k; ++i) {
            output.set_coeff(i, 0);
        }

        PolyElem tmp1, tmp2;

        // shuffle
        for (i = 0; i < k-1; ++i) {
            unsigned j = this->integer_internal(k - i) + i;

            if (i == j) {
                continue;
            } else {
                tmp1 = output.get_coeff(i);
                tmp2 = output.get_coeff(j);
                output.set_coeff(i, tmp2);
                output.set_coeff(j, tmp1);
            }
        }

        if (add_to_first != 0) {
            tmp1 = output.get_coeff(0);
            tmp1 += add_to_first;
            output.set_coeff(0, tmp1);
        }
    }

    auto error_vector_internal(unsigned k) -> vector<fmpzxx>;

public:
    Random(const Random& r) = delete;
    static auto integer(unsigned bound) -> unsigned;
    static auto poly(fmpq_polyxx& output, unsigned k, unsigned add_to_first = 0) -> void;
    static auto poly(fmpz_mod_polyxx& output, unsigned k, unsigned add_to_first = 0) -> void;
    static auto error_vector(unsigned k) -> vector<fmpzxx>;
    static auto reseed() -> void;
};
#endif
