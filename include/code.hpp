#ifndef MCELIECE_QCMDPC_GF_P_CODE_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_HPP

#include <flintxx.h>
#include <fmpqxx.h>
#include <fmpzxx.h>
#include <fmpz_mod_polyxx.h>
#include <fmpq_polyxx.h>
#include <iostream>
#include <vector>
#include <optional>
#include <cmath>
#include "random.hpp"
#include "utils.hpp"

using std::vector;
using std::optional;
using flint::fmpzxx;
using flint::fmpq_polyxx;
using flint::fmpz_polyxx;
using flint::fmpz_modxx_ctx;
using flint::fmpz_mod_polyxx;

/**
 * @brief DecodeBounds struct holds values used in decoding.
 *
 * These values are needed in each iteration of decoding.
 * They can be calculated from the parameter q and do not change until this parameter changes.
 * The values are calculated using the set() method. The values are uninitialized until it is called.
 */
struct DecodeBounds {
    unsigned q_thirds;
    double q_2, q_6, q_18;
    unsigned b1, b2, b3, b4, b5, b6, b7, b8;

    DecodeBounds() = default;

    /**
     * @brief Calculate the values.
     * @param q Parameter q of the MDPC GF(p) instance.
     */
    auto set(unsigned q) -> void;
};

/**
 * @brief Code class is an instance of MDPC GF(p), holds matrices H and G and performs encoding and decoding.
 */
class Code {
private:
    /**
     * @brief Parameters q and k of the MDPC GF(p) instance.
     */
    unsigned q_value, k_value;

    /**
     * @brief Vectors representing polynomials h0 and h1.
     *
     * These represent H0 and H1 blocks of the matrix H.
     */
    vector<unsigned> h0, h1;

    /**
     * @brief Vector representing the first row of the second block of matrix G.
     *
     * The second block should be calculated as -(h1_inv*h0)^T.
     * Here we omit the transposition and encode accordingly.
     * The first block of matrix G is the identity matrix and therefore does not need to be explicitly stored in a vector.
     * Once again, encoding is modified accordingly.
     */
    vector<unsigned> second_block_G;

    /**
     * @brief An instance of DecodeBounds.
     * @see DecodeBounds
     */
    DecodeBounds bounds{};

    /**
     * @brief Calculate the syndrome of vector vec.
     *
     * The syndrome of a vector is calculated as H*c^T or c*H^T. These produce a row or column vector.
     * In this implementation a row vector is produced. If the vec is a codeword in the current MDPC GF(p) instance,
     * its syndrome is a zero vector. It is nonzero otherwise. In this implementation, this method is used to calculate
     * the syndrome of ciphertext (vector<unsigned>) and of error vector (vector<int>).
     *
     * @tparam T Expected to be int or unsigned.
     * @param vec A vector of length 2*k.
     * @return A vector of length k representing the syndrome of vec.
     */
    template<typename T>
    auto calculate_syndrome(const vector<T>& vec) -> vector<unsigned> {
        vector<unsigned> syndrome;
        long tmp;
        for (unsigned i = k_value; i > 0; --i) {
            tmp = 0;
            for (unsigned j = 0; j < k_value; ++j) {
                tmp += (h0.at(floor_mod(i + j, k_value)) * ((long)vec.at(j)));
            }
            tmp = floor_mod(tmp, q_value);
            for (unsigned j = 0; j < k_value; ++j) {
                tmp += (h1.at(floor_mod(i + j, k_value)) * ((long)vec.at(k_value + j)));
            }
            tmp = floor_mod(tmp, q_value);
            syndrome.push_back((unsigned)tmp);
        }
        return syndrome;
    }

    /**
     * @brief Evaluate the syndrome and update the error_vector accordingly.
     *
     * Based on the value of syndrome, each position in the error vector may be incremented, decremented or remain
     * unchanged. The detailed explanation of the logic behind this method may be found in the original paper at
     * https://www.lebesgue.fr/sites/default/files/proceedings_WCC/WCC_2019_paper_32.pdf
     *
     * @param error_vector Error vector of length 2*k.
     * @param syndrome Syndrome vector of length k.
     */
    auto decide(vector<int>& error_vector, const vector<unsigned>& syndrome) const -> void;

    /**
     * @brief Update the error vector so that it only contains values -1, 0 or 1.
     *
     * The call to the decide method may have caused the error vector to contain values -2 and 2.
     * If this is the case, each occurrence of -2 is replaced by 1 and each occurrence of 2 is replaced by -1.
     *
     * @see Code::decide(vector<int>& error_vector, const vector<unsigned>& syndrome)
     * @param error_vector Error vector of length 2*k.
     */
    auto transform(vector<int>& error_vector) const -> void;
public:
    /**
     * @brief Create a new instance of MDPC GF(p) and generate keys.
     *
     * If the parameter q is not a power of 2, one may expect suboptimal performance of the decoder. In such scenario
     * the decoding will fail with higher frequency. This may be used to test the decoder's performance, however. Hence
     * such setting of the q is not disallowed.
     *
     * @param q Parameter q, ideally a power of 2.
     * @param k Parameter k, ideally a prime.
     */
    Code(unsigned q, unsigned k);

    /**
     * @brief Generate keys.
     *
     * The private key is the matrix H. The public kes is the matrix G. Matrix H consists of circulant blocks H0 and H1.
     * These are represented by polynomials h0 and h1 respectively.
     * Matrix G consists of one identity matrix block and a one circulant block.
     * The keys are generated as follows:
     * -# Randomly generate h0.
     * -# Randomly generate h1 such that it is invertible:
     *  -# Randomly generate h1 in Z_q/(x^k -1).
     *  -# Convert it to Q.
     *  -# Calculate the inverse in Q.
     *  -# Convert the inverse to Z_q/(x^k -1).
     * -# Calculate second block of matrix G as -(h1_inv*h0).
     *
     * @see Random::poly_internal(unsigned k, unsigned add_to_first = 0)
     */
    auto init_keys() -> void;

    /**
     * @brief Encode a vector.
     *
     * The vector vec is encoded as vec*G. The resulting vector is of length 2*k
     * and its first k positions are equal to vec. This is because the first block of G is identity matrix.
     *
     * @param vec A vector in Z_q of length k.
     * @return An encoded vectpr in Z_q of length 2*k.
     */
    auto encode(const vector<unsigned>& vec) -> vector<unsigned>;

    /**
     * @brief Decode a vector.
     *
     * Decoding the vector means finding the used error vector.
     * In the beginning, current error vector value is set to 0 and the vec's syndrome is calculated.
     * If it is zero, decoding successfully ends and zero initialized error vector is returned.
     * Otherwise, the vector is decoded iteratively up to num_iterations iterations. In each:
     * -# The current error vector is updated using the decide method and transformed using the transform method.
     * -# The syndrome of the current error vector is calculated.
     * -# This syndrome is subtracted from vec's syndrome. If the result is a zero vector, the decoding successfully ends and the current error vector is returned.
     * In case all iterations finnish and the error vector is not returned, the decoding is considered a failure.
     *
     * @param vec A vector in Z_q of length 2*k.
     * @param num_iterations Number of iterations.
     * @return On success, return found error vector.
     */
    auto decode(const vector<unsigned>& vec, unsigned num_iterations) -> optional<vector<int>>;
};

#endif
