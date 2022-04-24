#include <vector>
#include <iostream>
#include <string_view>
#include <charconv>
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


auto main(int argc, char * argv[]) -> int {

    auto params = parse_args(argc, argv);
    if (params.error) {
        std::cout << "Invalid arguments!" << std::endl;
        print_help();
        return -1;
    } else if (params.help) {
        print_help();
        return 0;
    }
    std::cout << "Settings:" << std::endl;
    std::cout << "q = " << params.q << std::endl;
    std::cout << "k = " << params.k << std::endl;
    std::cout << "iters = " << params.num_iters << std::endl;
    std::cout << "instances = " << params.num_instances << std::endl;
    std::cout << "runs = " << params.num_runs << std::endl;
    std::cout << std::endl;

    unsigned num_failures = 0;
    vector<long> ciphertext(2*params.k, 0);

    for (unsigned instance = 0; instance < params.num_instances; ++instance) {
        Code c{params.q, params.k};
        std::cerr << "Instance " << instance + 1 << " of " << params.num_instances << std::endl;

        for (unsigned run = 0; run < params.num_runs; ++run) {
            for (long & it : ciphertext) {
                it = floor_mod(((int)Random::integer(3)) - 1, params.q);
            }

            auto maybe_decoded = c.decode(ciphertext, params.num_iters);
            if (!maybe_decoded) {
                std::cout << "Decryption failed!" << std::endl;
                num_failures += 1;
                continue;
            }

            auto decoded = maybe_decoded.value();
            if (decoded.size() != ciphertext.size()) {
                std::cout << "Decryption failed! invalid length of decrypted vector!" << std::endl;
                num_failures += 1;
            } else {
                bool different = false;
                for (unsigned i = 0; i < ciphertext.size(); ++i) {
                    if (ciphertext.at(i) != floor_mod(decoded.at(i), params.q)) {
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
