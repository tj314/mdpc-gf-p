#ifndef MCELIECE_QCMDPC_GF_P_UTILS_HPP
#define MCELIECE_QCMDPC_GF_P_UTILS_HPP

/**
 * @brief Calculate remainder of integer division num/modulo.
 *
 * This function guarantees the following behavior:
 * - if num >= 0 and modulo > 0 then floor_mod(num, modulo) = num % modulo
 * - if num < 0 and modulo > 0 then floor_mod(num, modulo) = (num % modulo) + modulo
 * For example:
 * - floor_mod(7, 3) = 7 % 3 = 1
 * - floor_mod(-7, 3) = (-7 % 3) + 3 = -1 + 3 = 2
 *
 * @param num A number.
 * @param modulo A positive modulus.
 * @return The positive remainder of integer division num/modulo.
 */
auto floor_mod(long num, long modulo) -> long;
#endif
