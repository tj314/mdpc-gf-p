#include "code.hpp"
#include <iomanip>

Code::Code(unsigned q, unsigned k):
    q_value(q),
    k_value(k),
    q(q),
    k(k),
    context(this->q),
    h0(fmpz_mod_polyxx(this->context, k_value)),
    h1(fmpz_mod_polyxx(this->context, k_value)),
    h1_inv(fmpz_mod_polyxx(this->context, k_value)),
    mod(fmpz_mod_polyxx(this->context, k_value)),
    second_block_G(fmpz_mod_polyxx(this->context, k_value)),
    bounds(DecodeBounds{q_value}) {
    // http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    if ((q & (q-1)) != 0 || q == 0) {
        // TODO: compile-time options for throwing exceptions vs printing errors
        std::cout << "Code params: Parameter q is not a power of 2!" << std::endl;
    }

    if (!(this->k).is_probabprime()) {
        // TODO: k is probably small enough to test deterministically
        std::cout << "Code params: Parameter k is probably not prime!" << std::endl;
    }
    mod.set_coeff(0, -1);
    mod.set_coeff(k, 1);
    init_keys();
}

auto Code::init_keys() -> void {
    unsigned h0_prime = round(q_value / 3.0);
    unsigned h1_prime = round(q_value / 9.0);
    Random::poly(h0, k_value, h0_prime);  // coeffs from {-1, 0, 1} in Z_q

    fmpq_polyxx h1_tmp, mod, f;
    fmpz_polyxx numerator;
    fmpz_mod_polyxx g{context};
    fmpzxx m, r;
    mod.set_coeff(0, -1);
    mod.set_coeff(k_value, 1);



    while (true) {
        // coeffs from {-1, 0, 1} in Q + h1_prime added to the first coeff
        Random::poly(h1_tmp, k_value, h1_prime);
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
        auto xgcd_result2 = xgcd(m, q);
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
    /*
    std::cout << "h0 poly:      ";
    for (slong i = 0; i <= h0.degree(); ++i) {
        std::cout << std::setw(2) << h0.get_coeff(i);
        std::cout << " ";
    }
    std::cout << std::endl;

    std::cout << "h0 poly nice: ";
    h0.print_pretty("x");
    std::cout << std::endl;
    
    std::cout << "h1 poly:      ";
    for (slong i = 0; i <= h1.degree(); ++i) {
        std::cout << std::setw(2) << h1.get_coeff(i);
        std::cout << " ";
    }
    std::cout << std::endl;
    
    std::cout << "h1 poly nice: ";
    h1.print_pretty("x");
    std::cout << std::endl;
    
    std::cout << "h1 inv poly:  ";
    for (slong i = 0; i <= h1_inv.degree(); ++i) {
        std::cout << std::setw(2) << h1_inv.get_coeff(i);
        std::cout << " ";
    }
    std::cout << std::endl;
    
    std::cout << "h1 inv nice:  ";
    h1_inv.print_pretty("x");
    std::cout << std::endl;
    */
    fmpz_mod_polyxx pol{this->context, k_value};
    pol.set((h1*h1_inv) % this->mod);
    // std::cout << "h1_inv is OK:  " << (pol.is_one() ? "true" : "false") << std::endl;

    second_block_G.set((h1_inv * h0) % this->mod);
    fmpzxx scalar{-1};
    second_block_G = scalar * second_block_G;
    /*
    std::cout << "2nd block G:  ";
    for (slong i = 0; i <= second_block_G.degree(); ++i) {
        std::cout << std::setw(2) << second_block_G.get_coeff(i);
        std::cout << " ";
    }
    std::cout << std::endl;
    */
};

auto Code::encode(const vector<fmpzxx>& plaintext) -> vector<fmpzxx> {
    vector<fmpzxx> ciphertext;
    fmpzxx tmp{0};
    // std::cout << "before encode" << std::endl;
    for (unsigned i = 0; i < k_value; ++i) {
        tmp = plaintext.at(i) % q;
        // std::cout << tmp << " ";
        ciphertext.push_back(tmp);
    }
    // std::cout << std::endl;
    for (unsigned i = k_value; i > 0; --i) {
        tmp = 0;
        for (unsigned j = 0; j < k_value; ++j) {
            tmp += (plaintext.at(j) * second_block_G.get_coeff((i+j) % k_value));
        }
        tmp = tmp % q;
        ciphertext.push_back(tmp);
    }

    /*
    std::cout << "encode inside: ";
    for (unsigned i = 0; i < 2*k_value; ++i) {
        std::cout << ciphertext.at(i) << " ";
    }
    std::cout << std::endl;
    // std::cout << "after encode" << std::endl;
    */
    return ciphertext;
};

/*
template<typename T>
auto print_vec(const char * msg, vector<T> vec) -> void {
    std::cout << msg;
    for (const T& val : vec) {
        std::cout << std::setw(2) << val << " ";
    }
    std::cout << std::endl;
}
*/

auto Code::decode(const vector<fmpzxx>& ciphertext, unsigned num_iterations) -> optional<vector<fmpzxx>> {
    vector<fmpzxx> error_vector{2*k_value, fmpzxx{0}};
    vector<fmpzxx> syndrome = calculate_syndrome(ciphertext);
    bool syndrome_is_zero = true;
    for (unsigned i = 0; i < k_value; ++i) {
        syndrome_is_zero = syndrome_is_zero && syndrome.at(i).is_zero();
    }
    if (syndrome_is_zero) {
        return error_vector;
    }
    vector<fmpzxx> p{syndrome};
    vector<fmpzxx> esyn;
    fmpzxx tmp;
    // std::cout << "DECODING:" << std::endl;
    // print_vec("\tsyndrome before: ", p);
    for (unsigned iter = 0; iter < num_iterations; ++iter) {
        // std::cout << "Iteration " << iter << std::endl;
        decide(error_vector, p);
        transform(error_vector);
        esyn = calculate_syndrome(error_vector);
        // print_vec("\terror vector:    ", error_vector);

        // p = s - He^T
        syndrome_is_zero = true;
        for (unsigned i = 0; i < k_value; ++i) {
            tmp = syndrome.at(i) - esyn.at(i);
            tmp %= q;
            p.at(i) = tmp;
            syndrome_is_zero = p.at(i).is_zero() && syndrome_is_zero;
        }
        // print_vec("\tsyndrome after:  ", p);
        
        if (syndrome_is_zero) {
            return error_vector;
        }
    }

    return {};
}

auto Code::calculate_syndrome(const vector<fmpzxx>& ciphertext) -> vector<fmpzxx> {
    vector<fmpzxx> syndrome;
    fmpzxx tmp;
    for (unsigned i = k_value; i > 0; --i) {
        tmp = 0;
        for (unsigned j = 0; j < k_value; ++j) {
            tmp += (h0.get_coeff((i + j) % k_value) * ciphertext.at(j));
        }
        tmp %= q;
        for (unsigned j = 0; j < k_value; ++j) {
            tmp += (h1.get_coeff((i + j) % k_value) * ciphertext.at(k_value + j));
        }
        tmp = tmp % q;
        syndrome.push_back(tmp);
    }
    return syndrome;
}

auto Code::decide(vector<fmpzxx>& error_vector, const vector<fmpzxx>& syndrome) -> void {
    // std::cout << "err_len, syn_len: " << error_vector.size() << " " << syndrome.size() << std::endl;

    for (unsigned i = 0; i < k_value; ++i) {
        unsigned p_i = syndrome.at(i).to<ulong>();

        if (p_i >= bounds.b1 && p_i <= bounds.b2) {
            // e'_i = 1
            error_vector.at(i) += 1;
        } else if (p_i >= bounds.b3 && p_i <= bounds.b4) {
            // e'_i = -1
            error_vector.at(i) -= 1;
        }

        p_i %= bounds.q_thirds;
        if (p_i >= bounds.b5 && p_i <= bounds.b6) {
            // e'_ki = 1
            error_vector.at(k_value + i) += 1;
        } else if (p_i >= bounds.b7 && p_i <= bounds.b8) {
            // e'_ki = -1
            error_vector.at(k_value + i) -= 1;
        }
    }
}

auto Code::transform(vector<fmpzxx>& error_vetor) -> void {
    for (unsigned i = 0; i < 2*k_value; ++i) {
        fmpzxx& val = error_vetor.at(i);
        if (val == 2) {
            val = -1;
        } else if (val == -2) {
            val = 1;
        }
    }
}

