#include <vector>
#include <iostream>
#include "protocol.hpp"

auto main() -> int {
    unsigned q = 512, k = 509;
    // unsigned q = 8, k = 11;
    // unsigned q = 512, k = 41;
    Protocol p{q, k};
    std::cout << "keys generated!" << std::endl;

    vector<unsigned> plaintext;
    for (unsigned i = 0; i < k; ++i) {
        plaintext.push_back(Random::integer(q));
    }

    auto maybe_encrypted = p.encrypt(plaintext, true);
    if (!maybe_encrypted) {
        std::cerr << "Encryption failed!" << std::endl;
        return -1;
    }
    auto ciphertext = maybe_encrypted.value();

    auto maybe_decrypted = p.decrypt(ciphertext, 100, true);
    if (!maybe_decrypted) {
        std::cerr << "Decryption failed!" << std::endl;
        return -1;
    }
    auto decrypted = maybe_decrypted.value();

    if (decrypted.size() != plaintext.size()) {
        std::cerr << "Decryption failed: invalid length of decrypted vector!" << std::endl;
    } else {
        bool different = false;
        for (int i = 0; i < plaintext.size(); ++i) {
            if (plaintext.at(i) != decrypted.at(i)) {
                std::cerr << "Decryption failed: decrypted vector differs from the original plaintext!" << std::endl;
                different = true;
                break;
            }
        }
        if (!different) {
            std::cout << "Encryption and decryption OK!" << std::endl;
        }
    }
    return 0;
}
