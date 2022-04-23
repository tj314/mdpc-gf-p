#include <vector>
#include <iostream>
#include "code.hpp"

auto main() -> int {
    unsigned q = 345, k = 491;
    unsigned num_instances = 1000;
    unsigned num_runs = 1000;
    unsigned num_iters = 25;

    vector<long> ciphertext(k*2, 0);

    for (unsigned i = 0; i < num_instances; ++i) {
        Code code{q, k};
        for (unsigned j = 0; j < num_runs; ++j) {
            for (unsigned it = 0; it < k; ++it) {
                ciphertext.at(it) = floor_mod(((int)Random::integer(3)) - 1, q);
            }

            auto maybe_decrypted = code.decode(ciphertext, num_iters);
            if (!maybe_decrypted) {
                std::cout << "Decryption failure!" << std::endl;
            } else {
                auto err_vec = maybe_decrypted.value();
                if (err_vec.size() != ciphertext.size()) {
                    std::cout << "Decryption failure! err_vec is of different length" << std::endl;
                    continue;
                }
                for (unsigned it = 0; it < err_vec.size(); ++it) {
                    if (floor_mod(err_vec.at(it), q) != ciphertext.at(it)) {
                        std::cout << "Decryption failure! err_vec differs" << std::endl;
                        continue;
                    }
                }
                std::cout << "Decryption success!" << std::endl;
            }
        }
    }
    return 0;
}
