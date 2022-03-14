#include <vector>
#include <iostream>
#include <fmpzxx.h>
#include "code.hpp"
#include "random.hpp"

using std::vector;
using flint::fmpzxx;

auto main() -> int {
    unsigned q = 8, k = 11;
    Code c{q, k};
    vector<fmpzxx> plaintext;
    std::cout << "Plaintext: ";
    for (unsigned i = 0; i < k; ++i) {
        // fmpzxx tmp{Random::integer(q)};
        unsigned tmp = Random::integer(q);
        plaintext.push_back(fmpzxx{tmp});
        std::cout << tmp << " ";
    }
    std::cout << std::endl;
    vector<fmpzxx> encoded = c.encode(plaintext);
    std::cout << "Encoded:  ";
    for (unsigned i = 0; i < 2*k; ++i) {
        std::cout << encoded.at(i) << " ";
    }
    std::cout << std::endl;
    
    auto maybe_error_vector = c.decode(encoded, 10);
    if (maybe_error_vector) {
        auto error_vector = maybe_error_vector.value();
        std::cout << "Err vect: ";
        for (unsigned i = 0; i < 2*k; ++i) {
            std::cout << error_vector.at(i) << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Decoding error!" << std::endl;
    }
    
    return 0;
}
