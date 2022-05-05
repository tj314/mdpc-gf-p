#include <vector>
#include <iostream>
#include <chrono>
#include "protocol.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::time_point;
using std::cout;
using std::cerr;
using std::endl;

auto main() -> int {
    unsigned q = 345, k = 491;
    unsigned instances = 1000;

    cout << "Settings:" << endl;
    cout << "q = " << q << endl;
    cout << "k = " << k << endl;
    cout << "instances = " << instances << endl;
    cout << "units = milliseconds" << endl;

    vector<unsigned> plaintext(k, 0);
    for (unsigned instance = 0; instance < instances; ++instance) {
        std::cerr << "Instance " << instance + 1 << " of " << instances << std::endl;
        auto start_gen = high_resolution_clock::now();
        Protocol p{q, k};
        auto stop_gen = high_resolution_clock::now();

        for (unsigned i = 0; i < k; ++i) {
            plaintext.at(i) = Random::integer(q);
        }

        auto start_enc = high_resolution_clock::now();
        auto ciphertext = p.encrypt(plaintext, false).value();
        auto stop_enc = high_resolution_clock::now();

        auto start_dec_25 = high_resolution_clock::now();
        auto maybe_decrypted_25 = p.decrypt(ciphertext, 25, false);
        auto stop_dec_25 = high_resolution_clock::now();

        auto start_dec_50 = high_resolution_clock::now();
        auto maybe_decrypted_50 = p.decrypt(ciphertext, 50, false);
        auto stop_dec_50 = high_resolution_clock::now();

        auto start_dec_75 = high_resolution_clock::now();
        auto maybe_decrypted_75 = p.decrypt(ciphertext, 75, false);
        auto stop_dec_75 = high_resolution_clock::now();


        auto duration_gen = duration_cast<milliseconds>(stop_gen - start_gen);
        auto duration_enc = duration_cast<milliseconds>(stop_enc - start_enc);
        auto duration_dec_25 = duration_cast<milliseconds>(stop_dec_25 - start_dec_25);
        auto duration_dec_50 = duration_cast<milliseconds>(stop_dec_50 - start_dec_50);
        auto duration_dec_75 = duration_cast<milliseconds>(stop_dec_75 - start_dec_75);
        cout << duration_gen.count() << " " << duration_enc.count() << " " << duration_dec_25.count() << " " << duration_dec_50.count() << " " << duration_dec_75.count() << endl;
    }

    return 0;
}
