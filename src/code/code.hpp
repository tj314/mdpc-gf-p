#ifndef MCELIECE_QCMDPC_GF_P_CODE_HPP
#define MCELIECE_QCMDPC_GF_P_CODE_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/tools.h>
#include <NTL/vec_ZZ_p.h>
#include <NTL/ZZ_pEX.h>
#include <NTL/mat_ZZ_p.h>

NTL_CLIENT
using namespace std;

/**
 * This class represents a QC-MDPC [n,k]-linear code over Zq. It is expected that the following is true:
 * - q=2^x for some integer x>2
 * - k is an odd prime
 */
class Code {
private:
	const long q; // power of 2
	const long k; // length of cyclic block (H)
	ZZ_pX modulus; // x^k - 1

	// matrices represented by polynomials
	ZZ_pX h0, h0_inverse, h1;
	ZZ_pX g;
	vec_ZZ_p g_coeffs; // TODO: consider this approach vs matrix, remove if appropriate

	// matrix
	mat_ZZ_p G;

	/**
	 * @brief Generate a polynomial mod (x^k - 1) with coefficients drawn from I1 and sum equal to 0.
	 * 
	 * @return vec_ZZ_p 
	 */
	[[nodiscard]] auto generate_random_poly() const -> ZZ_pX;

	/**
	 * @brief Set h0 and h0_inverse polynomials.
	 *
	 * h0 polynomial represents the left block of parity-check matrix. It is calculated as follows:
	 * 1) generate a random polynomial using @see Code::generate_random_poly()
	 * 2) calculate significant entry as round(q/3)
	 * 3) add significant entry to the first coefficient of h0
	 * 4) if h0 is not invertible go to 1)
	 * 5) set h0_inverse to h0^-1
	 */
	auto generate_h0() -> void;

	/**
	 * @brief Set h1 polynomial.
	 *
	 * h0 polynomial represents the left block of parity-check matrix. It is calculated as follows:
	 * 1) generate a random polynomial using @see Code::generate_random_poly()
	 * 2) calculate significant entry as round(q/1)
	 * 3) add significant entry to the first coefficient of h1
	 */
	auto generate_h1() -> void;


	// TODO: this
	// auto generate_private_key() -> void;

	/**
	 * @brief Set polynomial g and matrix G.
	 *
	 * polynomial g = h0^-1 * h1 represents the right block of generating matrix.
	 * Matrix G in this class represents the right block of generating matrix. It is a cyclic matrix whose first block is determined by the coefficients of polynomial g.
	 *
	 * @todo right now, the g_coeffs is also set here. Update the documentation accordingly.
	 */
	auto generate_G() -> void;


	// TODO: implement
	auto calculate_syndrome(const vec_ZZ_p& message) -> vec_ZZ_p;

public:
	/**
	 * @brief Construct an instance of Code.
	 *
	 * @param q unsigned, ideally a power of 2
	 * @param k unsigned, ideally an odd prime
	 */
	Code(unsigned q, unsigned k);

	/**
	 * @brief Encode message.
	 *
	 * encoded = message * generating matrix. Message must be of length k.
	 *
	 * @throw invalid_argument if message is not of length k
	 * @param message vector of length k containing the message
	 * @return vector containing the encoded message
	 */
	[[nodiscard]] auto encode(const vec_ZZ_p& message) const -> vec_ZZ_p;

	// coeffs version
	// TODO: decide if we're keeping both versions
	// TODO: if we are keeping both versions, we should consider compile-time flag to switch between them
	[[nodiscard]] auto encode_with_coeffs(const vec_ZZ_p& message) const -> vec_ZZ_p;

	// decodes message using p-ary bit-flipping
	// returns e (error vector) or decode error
	[[nodiscard]] auto decode(const vec_ZZ_p& message, size_t max_iter) const -> vec_ZZ_p;
		// uses ciphertext message, parity check matrix H, MAX_ITER
		// 1. implement decoding algorithm
		// 2. return e or decode error

	/**
	 * @brief Set h0, h0_inverse, h1, g and G.
	 *
	 * @todo in the current implementation, this also sets g_coeffs
	 */
	auto init_keys() -> void;

// GETTERS

	[[nodiscard]] auto get_g() const -> const ZZ_pX&;
	[[nodiscard]] auto get_G() const -> const mat_ZZ_p&;
	[[nodiscard]] auto get_k() const -> long;
	[[nodiscard]] auto get_q() const -> long;
};


#endif