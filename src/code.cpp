#include "code.hpp"
#include "codeparams.hpp"
#include "flint.h"

// ==================
// CODE PARAMS CLASS
// ==================

CodeParams::CodeParams(unsigned q, unsigned k):
    q_value(q),
    k_value(k),
    q(q),
    k(k),
    context(this->q) {
    // http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    if ((q & (q-1)) != 0 || q == 0) {
        // TODO: compile-time options for throwing exceptions vs printing errors
        std::cout << "Code params: Parameter q is not a power of 2!" << std::endl;
    }

    if (!(this->k).is_probabprime()) {
        // TODO: k is probably small enough to test deterministically
        std::cout << "Code params: Parameter k is probably not prime!" << std::endl;
    }
}


// ==================
// CODE CLASS
// ==================

Code::Code(unsigned q, unsigned k):
    params(CodeParams{q, k}),
    h0(fmpz_mod_polyxx((this->params).context, k)),
    h1(fmpz_mod_polyxx((this->params).context, k)),
    h1_inv(fmpz_mod_polyxx((this->params).context, k)),
    mod(fmpz_mod_polyxx((this->params).context, k)),
    second_block_G(fmpz_mod_polyxx((this->params).context)) {
    mod.set_coeff(0, -1);
    mod.set_coeff(k, 1);
    init_keys();
}

auto Code::init_keys() -> void {
    unsigned h0_prime = round(params.q_value / 3.0);
    unsigned h1_prime = round(params.q_value / 9.0);
    Random::poly(h0, params, h0_prime);  // coeffs from {-1, 0, 1} in Z_q

    fmpq_polyxx h1_tmp, mod, f;
    fmpz_polyxx numerator;
    fmpz_mod_polyxx g{(this->params).context};
    fmpzxx m, r;
    mod.set_coeff(0, -1);
    mod.set_coeff(params.k_value, 1);



    while (true) {
        // coeffs from {-1, 0, 1} in Q + h1_prime added to the first coeff
        Random::poly(h1_tmp, params, h1_prime);
        // calculate poly f(x) such that
        // f = h_1^{-1} mod (x^k - 1) in Q[x]
        auto xgcd_result = xgcd(h1_tmp, mod);
        if (!xgcd_result.get<0>().is_one()) {
            // gcd(h1_tmp, mod) != 1,
            // therefore f does not exist
            continue;
        }
        f = xgcd_result.get<1>();
        f %= mod;

        // let m be denominator of f
        m = f.den();

        // calculate r such that
        // r = m^-1 mod q
        auto xgcd_result2 = xgcd(m, params.q);
        if (!xgcd_result2.get<0>().is_one()) {
            // r does not exist
            continue;
        }
        r = xgcd_result2.get<1>();

        // calculate poly g(x) such that
        // g(x) = m*f(x) mod q
        // TODO: there must be a better way to access the entire numerator
        for (slong i = 0; i <= f.degree(); ++i) {
            g.set_coeff(i, f.get_coeff(i).num());
        }

        // calculate poly h(x) such that
        // h(x) = r*g(x) mod q
        // then h(x) is h^{-1}(x) in Z_q[x]/(x^k - 1)
        h1_inv = r * g;
        break;
    }

    for (slong i = 0; i <= h1_tmp.degree(); ++i) {
        h1.set_coeff(i, h1_tmp.get_coeff(i).num());
    }

    flint_printf("h0:     ");
    h0.print_pretty("x");
    flint_printf("\n");
    flint_printf("h1:     ");
    h1.print_pretty("x");
    flint_printf("\n");
    flint_printf("h1_inv: ");
    h1_inv.print_pretty("x");
    flint_printf("\n");

    second_block_G.set((h1_inv * h0) % this->mod);
    fmpzxx scalar{-1};
    second_block_G = scalar * second_block_G;

    flint_printf("G seco: ");
    second_block_G.print_pretty("x");
    flint_printf("\n");
};

auto Code::encode(const vector<fmpzxx>& plaintext) -> vector<fmpzxx> {
    vector<fmpzxx> ciphertext;
    fmpzxx tmp{0};
    std::cout << "before encode" << std::endl;
    for (unsigned i = 0; i < params.k_value; ++i) {
        tmp = plaintext.at(i) % params.q;
        std::cout << tmp << " ";
        ciphertext.push_back(tmp);
    }
    std::cout << std::endl;
    for (unsigned i = 0; i < params.k_value; ++i) {
        tmp = 0;
        for (unsigned j = 0; j < params.k_value; ++j) {
            tmp += (plaintext.at(j) * second_block_G.get_coeff((i+j) % params.k_value));
        }
        tmp = tmp % params.q;
        ciphertext.push_back(tmp);
    }


    for (unsigned i = 0; i < 2*params.k_value; ++i) {
        std::cout << ciphertext.at(i) << " ";
    }
    std::cout << std::endl;
    std::cout << "after encode" << std::endl;
    return ciphertext;
};


auto Code::decode(const vector<fmpzxx>& ciphertext, unsigned num_iterations) -> optional<vector<fmpzxx>> {
    vector<fmpzxx> error_vector{2*params.k_value, fmpzxx{0}};
    vector<fmpzxx> syndrome = calculate_syndrome(ciphertext);
    bool syndrome_is_zero = true;
    for (unsigned i = 0; i < params.k_value; ++i) {
        syndrome_is_zero = syndrome_is_zero && syndrome.at(i).is_zero();
    }
    if (syndrome_is_zero) {
        return error_vector;
    }
    vector<fmpzxx> p{syndrome};
    vector<fmpzxx> esyn;
    fmpzxx tmp;
    for (unsigned iter = 0; iter < num_iterations; ++iter) {
        decide(error_vector, p);
        transform(error_vector);
        esyn = calculate_syndrome(error_vector);

        // p = s - He^T
        syndrome_is_zero = true;
        for (unsigned i = 0; i < params.k_value; ++i) {
            tmp = p.at(i) - esyn.at(i);
            tmp %= params.q;
            p.at(i) = tmp;
            syndrome_is_zero = p.at(i).is_zero() && syndrome_is_zero;
        }
        
        if (syndrome_is_zero) {
            return error_vector;
        }
    }

    return {};
}

auto Code::calculate_syndrome(const vector<fmpzxx>& ciphertext) -> vector<fmpzxx> {
    vector<fmpzxx> syndrome;
    fmpzxx tmp;
    for (unsigned i = 0; i < params.k_value; ++i) {
        tmp = 0;
        for (unsigned j = 0; j < params.k_value; ++j) {
            tmp += (h0.get_coeff((params.k_value - i + j) % params.k_value) * ciphertext.at(j));
        }
        for (unsigned j = 0; j < params.k_value; ++j) {
            tmp += (h1.get_coeff((params.k_value - i + j) % params.k_value) * ciphertext.at(params.k_value + j));
        }
        tmp = tmp % params.q;
        syndrome.push_back(tmp);
    }
    return syndrome;
}

auto Code::decide(vector<fmpzxx>& error_vector, const vector<fmpzxx>& syndrome) -> void {
    static double tmp1 = params.q_value / 2.0;
    static double tmp2 = params.q_value / 6.0;
    static double tmp3 = params.q_value / 18.0;
    static unsigned q_thirds = round(params.q_value / 3.0);
    static unsigned b1 = ceil(tmp2);
    static unsigned b2 = floor(tmp1);
    static unsigned b3 = ceil(tmp1);
    static unsigned b4 = floor(5*tmp2);
    static unsigned b5 = ceil(tmp3);
    static unsigned b6 = floor(tmp2);
    static unsigned b7 = ceil(tmp2);
    static unsigned b8 = floor(5*tmp3);

    std::cout << "err_len, syn_len: " << error_vector.size() << " " << syndrome.size() << std::endl;

    for (unsigned i = 0; i < params.k_value; ++i) {
        unsigned p_i = syndrome.at(i).to<ulong>();

        if (p_i >= b1 && p_i <= b2) {
            // e'_i = 1
            error_vector.at(i) += 1;
        } else if (p_i >= b3 && p_i <= b4) {
            // e'_i = -1
            error_vector.at(i) -= 1;
        }

        p_i %= q_thirds;
        if (p_i >= b5 && p_i <= b6) {
            // e'_ki = 1
            error_vector.at(params.k_value + i) += 1;
        } else if (p_i >= b7 && p_i <= b8) {
            // e'_ki = -1
            error_vector.at(params.k_value + i) -= 1;
        }
    }
}

auto Code::transform(vector<fmpzxx>& error_vetor) -> void {
    for (unsigned i = 0; i < 2*params.k_value; ++i) {
        fmpzxx& val = error_vetor.at(i);
        if (val == 2) {
            val = -1;
        } else if (val == -2) {
            val = 1;
        }
    }
}

