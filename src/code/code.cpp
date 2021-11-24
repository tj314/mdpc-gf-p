#include "code.hpp"
#include <cstddef>

/*-----------*\
 CONSTRUCTORS
\*-----------*/

Code::Code(unsigned q, unsigned k) : q((long)q), k((long)k) {
	// http://www.graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
	if ((q & (q-1)) != 0 || q == 0) {
		// TODO compile-time options for throwing exceptions vs printing errors
		cout << "Code constructor: Parameter q is not a power of 2!" << endl;
	}
	// ProbPrime returns 1 if k is prime
	if (!ProbPrime((long)k, 3)) { // TODO: k is probably small enough to test deterministically
		// TODO same as above
		cout << "Code constructor: Parameter k is probably not a prime!" << endl;
	}

	ZZ_p::init(ZZ(q));

	SetCoeff(modulus, 0, -1);
	SetCoeff(modulus, k);

	init_keys();
}


/*-----------*\
 PRIVATE
\*-----------*/


auto Code::generate_random_poly() const -> ZZ_pX {
	vector<long> vec;

	long num_ones = RandomBnd(k/2); // given sum has to be 0, there can be at most k/2 ones
	for (long i = 0; i < num_ones; ++i) {
		vec.push_back(1);
		vec.push_back(-1);
	}
	for (long i = 0; i < k - 2 * num_ones; ++i)
		vec.push_back(0);
	vec.resize(k,0);

	// now, sum of vec is 0, but it is 1,-1,1,-1,...,0,0,0,0 which is not very random
	// therefore, we have to shuffle it

	long num_swaps = RandomBnd(k);
	for (long i = 0; i < num_swaps; ++i) {
		long idx1 = RandomBnd(k);
		long idx2 = RandomBnd(k);
		swap(vec[idx1], vec[idx2]);
	}

	/*
	long sum = 0;
	for (size_t i = 0; i < (size_t)k; ++i) {
		long tmp = RandomBnd(3L) - 1;
		sum += tmp;
		vec.push_back(tmp);
	}
	
	while (sum != 0) {
		long idx = RandomBnd(k);
		long tmp = RandomBnd(3L) - 1;
		sum -= vec[idx];
		sum += tmp;
		vec[idx] = tmp;
	}
	*/

	ZZ_pX out;
	for (size_t i = 0; i < (size_t)k; ++i)
		SetCoeff(out, i, vec.at(i));
	return out;
}

auto Code::generate_h0() -> void {
	double tmp = q / 3.0;
	ZZ_pX h0_significant;
	SetCoeff(h0_significant, 0, lround(tmp));
	while (true) {
		try {
			h0 = generate_random_poly() + h0_significant;
			h0_inverse = InvMod(h0, modulus);
			break;
		} catch(const InvModErrorObject& ex) {
			continue;
		}
	}
}

auto Code::generate_h1() -> void {
	double tmp = q / 9.0;
	ZZ_pX h1_significant;
	SetCoeff(h1_significant, 0, lround(tmp));
	h1 = generate_random_poly() + h1_significant;
}

auto Code::generate_G() -> void {
	g = MulMod(h0_inverse, h1, modulus);
	G.SetDims(k, k);
	for (long i = 0; i < k; ++i) {
		for (long j = 0; j < k; ++j) {
			G.put(i, j, ZZ_p{coeff(g, j)});
		}
		g = MulByXMod(g, modulus);
	}

	// for encoding w/0 matrix
	g_coeffs = VectorCopy(g, k);
}


/*-----------*\
 PUBLIC
\*-----------*/


auto Code::calculate_syndrome(const vec_ZZ_p& message) -> vec_ZZ_p {
	throw "not yet implemented!";
	return message;
}

auto Code::encode_with_coeffs(const vec_ZZ_p& message) const -> vec_ZZ_p {
	if (message.length() != k)
		throw invalid_argument("Code::encode_with_transpose: Given message is not of length k!");
	vec_ZZ_p encoded = VectorCopy(message, k);
	for (long i = 0; i < k; ++i) {
		long curr_offset = i;
		ZZ_p sum{0};
		for (long j = 0; j < k; ++j) {
			sum += message[j] * g_coeffs[curr_offset];
			--curr_offset;
			if (curr_offset < 0)
				curr_offset += k;
		}
		encoded.append(sum);
	}
	return encoded;
}

auto Code::encode(const vec_ZZ_p& message) const -> vec_ZZ_p {
	if (message.length() != k)
		throw invalid_argument("Code::encode: Given message is not of length k!");
	vec_ZZ_p encoded = VectorCopy(message, k);
	vec_ZZ_p encoded_right;
	mul(encoded_right, message, G);
	append(encoded, encoded_right);
	return encoded;
}

auto Code::decode(const vec_ZZ_p& message, const size_t max_iter) const -> vec_ZZ_p {
	if (message.length() != 2*k)
		throw invalid_argument("Code::decode: Given message is not of length 2*k!");
	// dummy code to suppress warnings
	size_t i = max_iter;
	++i;
	throw "Not yet implemented!";
}

auto Code::init_keys() -> void {
	generate_h0();
	generate_h1();
	generate_G();

	/*
	cout << "KEY INIT" << endl;
	cout << "\th0    : " << h0 << endl;
	cout << "\th0 inv: " << h0_inverse << endl;
	cout << "\th1    : " << h1 << endl;
	cout << "\tg     : " << g_coeffs << endl;
	*/
}

auto Code::get_g() const -> const ZZ_pX& {
	return g;
}

auto Code::get_G() const -> const mat_ZZ_p& {
	return G;
}

auto Code::get_k() const -> long {
	return k;
}

auto Code::get_q() const -> long {
	return q;
}