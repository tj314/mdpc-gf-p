#include <NTL/ZZ.h>
#include <NTL/vec_ZZ_p.h>
#include <iostream>
#include "code.hpp"


NTL_CLIENT

auto main() -> int {
    const unsigned q = 16;
    const unsigned k = 7;
    Code code{q, k};
    vec_ZZ_p m = random_vec_ZZ_p((long)k);
    cout << "Message: " << m << endl;
    try {
        cout << "Encoded1:" << code.encode(m) << endl;
        cout << "Encoded2:" << code.encode_with_transpose(m) << endl;
    } catch(const invalid_argument& ex) {
        cerr << ex.what() << endl;
    }

    return 0;
}