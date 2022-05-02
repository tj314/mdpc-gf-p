#include "protocol.hpp"

auto Protocol::encrypt(const vector<unsigned>& plaintext, bool verbose) -> optional<vector<unsigned>> {
    if (plaintext.size() != k) {
        if (verbose) {
           cerr << "Encrypt: Plaintext is of size " << plaintext.size() << ". It must be of size k = " << k << "!" << endl;
        }
        return {};
    }

    vector<unsigned> encoded = c.encode(plaintext);
    vector<int> error_vector = Random::error_vector(k);
    vector<unsigned> encrypted;
    long tmp;
    for (unsigned i = 0; i < 2*k; ++i) {
        tmp = ((long)encoded.at(i)) + error_vector.at(i);
        tmp = floor_mod(tmp, q);
        encrypted.push_back(tmp);
    }
    return encrypted;
}

auto Protocol::decrypt(const vector<unsigned>& ciphertext, unsigned num_iterations, bool verbose) -> optional<vector<unsigned>> {
    if (ciphertext.size() != 2*k) {
        if (verbose) {
           cerr << "Decrypt: Ciphertext is of size " << ciphertext.size() << ". It must be of size 2*k = " << 2*k << "!" << endl;
        }
        return {};
    }

    auto maybe_error_vector = c.decode(ciphertext, num_iterations);
    if (!maybe_error_vector) {
        if (verbose) {
           cerr << "Decrypt: Decoding failure!" << endl;
        }
        return {};
    }

    vector<unsigned> plaintext;
    long tmp;
    auto for_sure_error_vector = maybe_error_vector.value();
    for (unsigned i = 0; i < k; ++i) {
        tmp = ((long)ciphertext.at(i)) - ((long)for_sure_error_vector.at(i));
        tmp = floor_mod(tmp, q);
        plaintext.push_back((unsigned)tmp);
    }
    return plaintext;
}
