#include <vector>
#include <iostream>
#include "code.hpp"


auto print_help() -> void {
    std::cout << "====================================================================================================================" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "Run tests:    ./mdpc-gf-p -q q_value -k k_value -i/--iters num_iters -n/--instances num_instances -r/--runs num_runs" << std::endl;
    std::cout << "Warning! A lot of output will be produced. It is highly recommended to redirect it." << std::endl;
    std::cout << "Example usage:" << std::endl;
    std::cout << "./mdpc-gf-p -q 512 -k 491 -i 25 -n 1000 -r 1000 > out.txt" << std::endl;
    std::cout << "====================================================================================================================" << std::endl;
}


auto parse_args(int argc, char * argv[]) {
    struct params {
        unsigned q;
        unsigned k;
        unsigned num_iters;
        unsigned num_instances;
        unsigned num_runs;
        bool error;
        bool help;
    } p {0, 0, 0, 0, 0, false, false};

    if (argc == 1) {
        p.error = true;
        return p;
    }

    bool q_set = false, k_set = false, iters_set = false, instances_set = false, runs_set = false;

    std::vector<std::string_view> args(argv + 1, argv + argc);
    auto it = args.begin(), end = args.end();
    while(it != end) {
        if (*it == "-h" || *it == "--help") {
            p.help = true;
            return p;
        } else if (*it == "-q") {
            ++it;
            if (it != end) {
                auto res = std::from_chars((*it).data(), (*it).data() + (*it).size(), p.q);
                if (res.ec == std::errc::invalid_argument) {
                    p.error = true;
                    return p;
                }
                q_set = true;
            } else {
                p.error = true;
                return p;
            }
        } else if (*it == "-k") {
            ++it;
            if (it != end) {
                auto res = std::from_chars((*it).data(), (*it).data() + (*it).size(), p.k);
                if (res.ec == std::errc::invalid_argument) {
                    p.error = true;
                    return p;
                }
                k_set = true;
            } else {
                p.error = true;
                return p;
            }
        } else if (*it == "-i" || *it == "--iters") {
            ++it;
            if (it != end) {
                auto res = std::from_chars((*it).data(), (*it).data() + (*it).size(), p.num_iters);
                if (res.ec == std::errc::invalid_argument) {
                    p.error = true;
                    return p;
                }
                iters_set = true;
            } else {
                p.error = true;
                return p;
            }
        } else if (*it == "-n" || *it == "--instances") {
            ++it;
            if (it != end) {
                auto res = std::from_chars((*it).data(), (*it).data() + (*it).size(), p.num_instances);
                if (res.ec == std::errc::invalid_argument) {
                    p.error = true;
                    return p;
                }
                instances_set = true;
            } else {
                p.error = true;
                return p;
            }
        } else if (*it == "-r" || *it == "--runs") {
            ++it;
            if (it != end) {
                auto res = std::from_chars((*it).data(), (*it).data() + (*it).size(), p.num_runs);
                if (res.ec == std::errc::invalid_argument) {
                    p.error = true;
                    return p;
                }
                runs_set = true;
            } else {
                p.error = true;
                return p;
            }
        } else {
            // unknown param
            p.error = true;
            return p;
        }
        ++it;
    }

    if (!(q_set && k_set && runs_set && instances_set && iters_set)) {
        p.error = true;
    }

    return p;
}


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
