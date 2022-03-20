#include "protocol.hpp"
#include <iomanip>

auto Protocol::encrypt(const vector<unsigned>& plaintext, bool verbose) -> optional<vector<unsigned>> {
    if (plaintext.size() != k) {
        if (verbose) {
           cerr << "Encrypt: Plaintext is of size " << plaintext.size() << ". It must be of size k = " << k << "!" << endl;
        }
        return {};
    }
    vector<fmpzxx> vec;
    for (unsigned i = 0; i < k; ++i) {
        vec.push_back(fmpzxx{plaintext.at(i)});
    }
    vector<fmpzxx> encoded = c.encode(vec);
    vector<int> error_vector = Random::error_vector(k);
    std::cout << "Error_vector: ";
    for (int val : error_vector) {
        std::cout << std::setw(2) << val << " ";
    }
    std::cout << std::endl;
    std::cout << "Encoded:      ";
    for (const fmpzxx& val : encoded) {
        std::cout << std::setw(2) << val << " ";
    }
    std::cout << std::endl;
    vector<unsigned> encrypted;
    long tmp;
    for (unsigned i = 0; i < 2*k; ++i) {
        tmp = (long)encoded.at(i).to<slong>();
        tmp += error_vector.at(i);
        if (tmp == -1l) {
            tmp += q;
        }
        encrypted.push_back(tmp % q);
    }
    std::cout << "Encrypted:    ";
    for (unsigned val : encrypted) {
        std::cout << std::setw(2) << val << " ";
    }
    std::cout << std::endl;
    return encrypted;
}

auto Protocol::decrypt(const vector<unsigned>& ciphertext, unsigned num_iterations, bool verbose) -> optional<vector<unsigned>> {
    if (ciphertext.size() != 2*k) {
        if (verbose) {
           cerr << "Decrypt: Ciphertext is of size " << ciphertext.size() << ". It must be of size 2*k = " << 2*k << "!" << endl;
        }
        return {};
    }

    vector<fmpzxx> ctext;
    for (unsigned val : ciphertext) {
        ctext.push_back(fmpzxx{val});
    }

    auto maybe_error_vector = c.decode(ctext, num_iterations);
    if (!maybe_error_vector) {
        if (verbose) {
           cerr << "Decrypt: Decryption failure!" << endl;
        }
        return {};
    }

    vector<unsigned> plaintext;
    fmpzxx tmp;
    auto for_sure_error_vector = maybe_error_vector.value();
    for (unsigned i = 0; i < k; ++i) {
        tmp = ctext.at(i) - for_sure_error_vector.at(i);
        plaintext.push_back((unsigned)(tmp.to<slong>() % q));
    }
    return plaintext;
}
