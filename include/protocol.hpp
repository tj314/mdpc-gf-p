#ifndef MCELIECE_QCMDPC_GF_P_PROTOCOL_HPP
#define MCELIECE_QCMDPC_GF_P_PROTOCOL_HPP

#include <fmpzxx.h>
#include <vector>
#include <optional>
#include <iostream>
#include "code.hpp"

using flint::fmpzxx;
using std::vector;
using std::optional;
using std::cerr;
using std::endl;

/**
 * @brief Protocol class holds an instance of MDPC GF(p) cryptosystem and allows for encryption and decryption using this instance.
 */
class Protocol {
private:
    /**
     * @brief Parameters q and k of the MDPC GF(p) cryptosystem.
     * @see Code
     */
    unsigned q, k;

    /**
     * @brief Instance of MDPC GF(p) used for encoding and decoding.
     * @see Code
     */
    Code c;

public:
    /**
     * @brief Generate a new MDPC GF(p) instance with the given q and k settings.
     *
     * To achieve the best possible DFR value, q should be a power of 2 and k should be a prime. It is not required, however.
     *
     * @see Code
     *
     * @param q Parameter q, ideally a power of 2.
     * @param k Parameter k, ideally a prime.
     */
    Protocol(unsigned q, unsigned k): q(q), k(k), c(Code{q, k}) {}

    /**
     * @brief Encrypt a plaintext.
     *
     * Encryption is achieved as follows:
     * -# Plaintext is encoded using the generated Code instance.
     * -# An error vector is generated.
     * -# Encoded plaintext is summed with the error vector.
     * Encryption will fail if and only if the plaintext length is not k.
     * Ciphertext is of length 2*k.
     *
     * @see Code::encode(const vector<unsigned>& plaintext)
     * @param plaintext A vector of length k with values from Z_q.
     * @param verbose Whether to print error messages.
     * @return On success, return ciphertext.
     */
    auto encrypt(const vector<unsigned>& plaintext, bool verbose=false) -> optional<vector<unsigned>>;

    /**
     * @brief Decrypt a ciphertext.
     *
     * Decryption is achieved as follows:
     * -# Ciphertext is decoded using the generated Code instance to obtain the error vector used in encryption.
     * -# The error vector is subtracted from the ciphertext.
     * -# Plaintext is read as the first k positions from ciphertext.
     * Decryption will fail if:
     * - Ciphertext is not of length 2*k.
     * - Decoding fails.
     *
     * @see Code::decode(const vector<unsigned>& ciphertext, unsigned num_iterations)
     * @param ciphertext A vector of length 2*k with values from Z_q.
     * @param num_iterations Number of iterations used in the decoding step.
     * @param verbose Whether to print error messages.
     * @return On success, return plaintext.
     */
    auto decrypt(const vector<unsigned>& ciphertext, unsigned num_iterations=25, bool verbose=false) -> optional<vector<unsigned>>;
};
#endif
