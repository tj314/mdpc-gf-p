#include "random.hpp"

Random::Random() {
    srand(time(NULL));
}

auto Random::integer(unsigned bound) -> unsigned {
    return rand() % bound;
}


auto Random::poly(fmpq_polyxx& output, const CodeParams& params, unsigned add_to_first) -> void {
    unsigned third = params.k_value / 3;
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
    for (; i < params.k_value; ++i) {
        output.set_coeff(i, 0);
    }

    fmpqxx tmp1, tmp2;

    // shuffle
    for (i = 0; i < params.k_value-1; ++i) {
        unsigned j = this->integer(params.k_value - i) + i;

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


auto Random::poly(fmpz_mod_polyxx& output, const CodeParams& params, unsigned add_to_first) -> void {
    unsigned third = params.k_value / 3;
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
    for (; i < params.k_value; ++i) {
        output.set_coeff(i, 0);
    }

    fmpzxx tmp1, tmp2;

    // shuffle
    for (i = 0; i < params.k_value-1; ++i) {
        unsigned j = this->integer(params.k_value - i) + i;

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

auto Random::error_vector(const CodeParams& params) -> vector<fmpzxx> {
    vector<fmpzxx> error_vector;

    for (unsigned i = 0; i < 2*params.k_value; ++i) {
        error_vector.push_back(fmpzxx{this->integer(params.q_value)});
    }

    return error_vector;
}
