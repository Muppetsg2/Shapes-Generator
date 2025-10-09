#include "pch.hpp"
#include "BitMathOperators.hpp"

TEST_CASE("ShapesGenerator.ICOSphere.Calculations") {

    unsigned int as[] = { 0, 0, 0, 0, 0, 1, 5, 11, 10, 7, 3, 3, 3, 3, 3, 4, 2, 6, 8, 9 };
    unsigned int bs[] = { 11, 5, 1, 7, 10, 5, 11, 10, 7, 1, 9, 4, 2, 6, 8, 9, 4, 2, 6, 8 };
    unsigned int cs[] = { 5, 1, 7, 10, 11, 9, 4, 2, 6, 8, 4, 2, 6, 8, 9, 5, 11, 10, 7, 1 };

    for (unsigned int i = 0u; i < 4u; ++i) {
        for (unsigned int j = 0u; j < 5u; ++j) {
            unsigned int ia, ib, ic;
            switch (i) {
                case 0u: {
                    ia = 0u;
                    ib = (11u + 3u * mul_2(i - div_4(i)) + mul_2(div_2(i)) + div_4(i)) % 12u;
                    ic = (5u + mul_8(div_4(i + 3u)) + 3u * mod_4(i) * div_4(i + 2u) + mul_2(mod_2(div_2(i))) + div_4(i + 2u)) % 12u;
                    break;
                }
                case 1u: {
                    ia = (1u + mul_2(2u + 3u * mod_4(i + 3u) * div_4(i + 2)) * div_4(i + 3u) + 3u * ((i / 3u) + div_4(i)) + mul_2(i / 3u)) % 12u;
                    ib = (5u + 3u * mul_2(i) + 3u * ((i / 3u) + 1u) * div_4(i + 2u) + mul_2(div_4(i + 2u))) % 12u;
                    ic = (9u + 7u * div_4(i + 3u) + mul_2(mul_2(i) + (mod_4(i) / 3u) + mod_2((i + 2u) % 3)) * (div_4(i + 2u))) % 12u;
                    break;
                }
                case 2u: {
                    ia = 3u;
                    ib = (9u + 7u * div_4(i + 3u) + mul_2(mul_2(i) + (mod_4(i) / 3u) + mod_2((i + 2u) % 3)) * div_4(i + 2u)) % 12u;
                    ic = (4u + mul_2(5u + i * div_4(i + 2u)) * div_4(i + 3u) - div_4(i)) % 12u;
                    break;
                }
                case 3u: {
                    ia = (4u + mul_2(5u + i * div_4(i + 2u)) * div_4(i + 3u) - div_4(i)) % 12u;
                    ib = (9u + 7u * div_4(i + 3u) + mul_2(mul_2(i) + (mod_4(i) / 3u) + mod_2((i + 2u) % 3u)) * div_4(i + 2u)) % 12u;
                    ic = (5u + 3u * mul_2(i) + 3u * ((i / 3u) + 1u) * div_4(i + 2u) + mul_2(div_4(i + 2u))) % 12u;
                    break;
                }
            }

            CHECK(ia == as[i * 5 + j]);
            CHECK(ib == bs[i * 5 + j]);
            CHECK(ic == cs[i * 5 + j]);
        }
    }
}