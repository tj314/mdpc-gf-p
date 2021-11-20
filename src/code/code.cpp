#include "code.hpp"

Code::Code(unsigned q, unsigned k) : q((long)q), k((long)k) {
    ZZ_pX modulus;
    SetCoeff(modulus, 0, -1);
    SetCoeff(modulus, k);
    ZZ_pE::init(modulus);
}

auto Code::generate_random_poly() -> ZZ_pEX {
    vector<long> vec;
    long sum = 0;
    for (size_t i = 0; i < (size_t)k; ++i) {
        long tmp = RandomBnd(3L) - 1;
        sum += tmp;
        vec.push_back(tmp);
    }
    
    while (sum != 0) {
        long idx = RandomBnd(k);
        long tmp = RandomBnd(3L) - 1;
        sum -= vec[idx];
        sum += tmp;
        vec[idx] = tmp;
    }

    ZZ_pEX out;
    for (size_t i = 0; i < (size_t)k; ++i)
        SetCoeff(out, i, vec.at(i));
    return out;
}

auto Code::generate_h0() -> void {
    double tmp = q / 3.0;
    ZZ_pEX h0_significant;
    SetCoeff(h0_significant, 0, (long)(tmp + 0.5));
    h0 = generate_random_poly() + h0_significant;
    ZZ_pEX one;
    SetCoeff(one, 0);
    while (true) {
        try {
            h0_inverse = one / h0;
            break;
        } catch(const InvModErrorObject& ex) {
            continue;
        }
    }
}

auto Code::generate_h1() -> void {
    double tmp = q / 9.0;
    ZZ_pEX h1_significant;
    SetCoeff(h1_significant, 0, (long)(tmp + 0.5));
    h1 = generate_random_poly() + h1_significant;
}
