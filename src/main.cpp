#include <vector>
#include <iostream>
#include "protocol.hpp"
#include "random.hpp"

using std::vector;
using flint::fmpzxx;

auto main() -> int {
    unsigned q = 8, k = 11;
    Protocol p{q, k};

    std::cout << "Plaintext:  ";
    vector<unsigned> plaintext;
    for (unsigned i = 0; i < k; ++i) {
        plaintext.push_back(Random::integer(q));
        std::cout << plaintext.at(i) << " ";
    }
    std::cout << std::endl;

    auto maybe_encrypted = p.encrypt(plaintext, true);
    if (!maybe_encrypted) {
        std::cerr << "Encryption failed!" << std::endl;
        return -1;
    }

    auto ciphertext = maybe_encrypted.value();
    std::cout << "Ciphertext: ";
    for (unsigned val : ciphertext) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    auto maybe_decrypted = p.decrypt(ciphertext, 10, true);
    if (!maybe_decrypted) {
        std::cerr << "Decryption failed!" << std::endl;
        return -1;
    }
    auto decrypted = maybe_decrypted.value();
    std::cout << "Decrypted:  ";
    for (unsigned val : decrypted) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    return 0;
}
