#include <vector>
#include <iostream>
#include "protocol.hpp"

auto main() -> int {
    unsigned q = 512, k = 491;
    unsigned num_instances = 1000;
    unsigned num_runs = 1000;
    unsigned num_iters = 25;

    unsigned num_failures = 0;

    vector<unsigned> plaintext(k, 0);
    for (unsigned instance = 0; instance < num_instances; ++instance) {
        Protocol p{q, k};
        std::cerr << "Instance " << instance + 1 << " of " << num_instances << std::endl;

        for (unsigned run = 0; run < num_runs; ++run) {
            for (unsigned i = 0; i < k; ++i) {
                plaintext.at(i) = Random::integer(q);
            }

            auto ciphertext = p.encrypt(plaintext, false).value();

            auto maybe_decrypted = p.decrypt(ciphertext, num_iters, false);
            if (!maybe_decrypted) {
                std::cout << "Decryption failed!" << std::endl;
                num_failures += 1;
                continue;
            }
            auto decrypted = maybe_decrypted.value();

            if (decrypted.size() != plaintext.size()) {
                std::cout << "Decryption failed! invalid length of decrypted vector!" << std::endl;
                num_failures += 1;
            } else {
                bool different = false;
                for (unsigned i = 0; i < plaintext.size(); ++i) {
                    if (plaintext.at(i) != decrypted.at(i)) {
                        std::cout << "Decryption failed! decrypted vector differs from the original plaintext!" << std::endl;
                        num_failures += 1;
                        different = true;
                        break;
                    }
                }
                if (!different) {
                    std::cout << "Encryption and decryption OK!" << std::endl;
                }
            }
        }
    }

    std::cout << "Failures: " << num_failures << std::endl;
    std::cerr << "Failures: " << num_failures << std::endl;
    return 0;
}
