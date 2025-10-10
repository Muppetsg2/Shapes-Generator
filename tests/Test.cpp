#include "pch.hpp"
#include "BitMathOperators.hpp"

TEST_CASE("ShapesGenerator.ICOSphere.Calculations") {

    unsigned int as[] = { 0, 0, 0, 0, 0, 1, 5, 11, 10, 7, 3, 3, 3, 3, 3, 4, 2, 6, 8, 9 };
    unsigned int bs[] = { 11, 5, 1, 7, 10, 5, 11, 10, 7, 1, 9, 4, 2, 6, 8, 9, 4, 2, 6, 8 };
    unsigned int cs[] = { 5, 1, 7, 10, 11, 9, 4, 2, 6, 8, 4, 2, 6, 8, 9, 5, 11, 10, 7, 1 };

    for (unsigned int j = 0u; j < 4u; ++j) {
        for (unsigned int i = 0u; i < 5u; ++i) {
            unsigned int ia, ib, ic;
            switch (j) {
                case 0u: {
                    ia = 0u;
                    ib = (11u + 3u * mul_2(i - div_4(i)) + mul_2(div_2(i)) + div_4(i)) % 12u;
                    ic = (5u + mul_8(div_4(i + 3u)) + 3u * mod_4(i + 3u) * div_4(i + 2u) + mul_2(mod_2(div_2(i))) + div_4(i + 2u)) % 12u;
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

            CHECK(ia == as[j * 5 + i]);
            CHECK(ib == bs[j * 5 + i]);
            CHECK(ic == cs[j * 5 + i]);
        }
    }
}

TEST_CASE("ShapesGenerator.Cube.Calculations") {

    int xs[] = { -1, 1, 1, -1, -1, 1, 1, -1 };
    int ys[] = { 1, 1, 1, 1, -1, -1, -1, -1 };
    int zs[] = { 1, 1, -1, -1, 1, 1, -1, -1 };

    int us[] = { 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0 };
    int vs[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1 };

    int nxs[] = { -1, 1, 1, -1, -1, 1, 1, -1 };
    int nys[] = { 1, 1, 1, 1, -1, -1, -1, -1 };
    int nzs[] = { 1, 1, -1, -1, 1, 1, -1, -1 };

    unsigned int triss[] = { 2, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 1 };

    for (unsigned int p = 0u; p < 3u; ++p) {
        for (unsigned int i = 0u; i < 8u; ++i) {
            
            int x = ((int)mod_2(div_2(i + 1u)) - (int)mod_2(div_2(i + 1u) + 1u));
            int y = ((int)mod_2(div_4(i + 4u)) - (int)div_4(i));
            int z = ((int)mod_2(div_2(i) + 1u) - (int)mod_2(div_2(i)));

            CHECK(x == xs[i]);
            CHECK(y == ys[i]);
            CHECK(z == zs[i]);
            
            int u = 0;
            int v = 0;

            int nx = 0;
            int ny = 0;
            int nz = 0;
            
            unsigned int tris = 1;
            switch (p) {
                case 0u: {
                    u = mod_2(i);
                    v = div_4(i);

                    nz = ((int)mod_2(div_2(i) + 1u) - (int)mod_2(div_2(i)));
    
                    tris = mod_2(i + mod_2(div_4(i + 4u))) + 1u;
                    break;
                }
                case 1u: {
                    u = mod_2(i + 1u);
                    v = div_4(i);

                    nx = ((int)mod_2(div_2(i + 1u)) - (int)mod_2(div_2(i + 1u) + 1u));
    
                    tris = mod_2(i + div_4(i)) + 1u;
                    break;
                }
                case 2u: {
                    u = mod_2(div_2(i + 1u));
                    v = mod_2(div_2(i + mul_2(mod_2(div_4(i + 4u)))));

                    ny = ((int)mod_2(div_4(i + 4u)) - (int)div_4(i));
    
                    tris = mod_2(i + div_4(i)) + 1u;
                    break;
                }
            }

            CHECK(u == us[p * 8 + i]);
            CHECK(v == vs[p * 8 + i]);

            CHECK(nx == (p == 1 ? nxs[i] : 0));
            CHECK(ny == (p == 2 ? nys[i] : 0));
            CHECK(nz == (p == 0 ? nzs[i] : 0));

            CHECK(tris == triss[p * 8 + i]);
        }
    }
    
    unsigned int fs[] = { 0, 0, 2, 2, 9, 9, 11, 11, 19, 19, 20, 20 };
    unsigned int ss[] = { 4, 5, 6, 7, 13, 14, 15, 12, 16, 17, 23, 22 };
    unsigned int ts[] = { 5, 1, 7, 3, 14, 10, 12, 8, 17, 18, 22, 21 };

    for (unsigned int p = 0u; p < 3u; ++p) {
        const unsigned int t1 = p + mul_8(p) + div_2(p);
        const unsigned int t2 = p + 2u + mul_8(p);
    
        unsigned int f, s, t; // First, Second, Third
        if (p == 0u) {
            for (unsigned int i = 0u; i < 2u; ++i) {
                const unsigned int v = i == 0u ? t1 : t2;
    
                // First Triangle
                f = v;
                s = v + 4u;
                t = v + 5u;

                CHECK(f == fs[i * 2]);
                CHECK(s == ss[i * 2]);
                CHECK(t == ts[i * 2]);
    
                // Second Triangle
                s = v + 5u;
                t = v + 1u;

                CHECK(f == fs[i * 2 + 1]);
                CHECK(s == ss[i * 2 + 1]);
                CHECK(t == ts[i * 2 + 1]);
            }
        }
        else if (p == 1u) {
            // RIGHT
            // First Triangle
            f = t1;
            s = t1 + 4u;
            t = t1 + 5u;

            CHECK(f == fs[4]);
            CHECK(s == ss[4]);
            CHECK(t == ts[4]);
    
            // Second Triangle
            s = t1 + 5u;
            t = t1 + 1u;

            CHECK(f == fs[5]);
            CHECK(s == ss[5]);
            CHECK(t == ts[5]);
    
            // LEFT
            // First Triangle
            f = t2;
            s = t2 + 4u;
            t = t2 + 1u;

            CHECK(f == fs[6]);
            CHECK(s == ss[6]);
            CHECK(t == ts[6]);
    
            // Second Triangle
            s = t2 + 1u;
            t = t2 - 3u;

            CHECK(f == fs[7]);
            CHECK(s == ss[7]);
            CHECK(t == ts[7]);
        }
        else if (p == 2u) {
            // TOP
            // First Triangle
            f = t1;
            s = t1 - 3u;
            t = t1 - 2u;

            CHECK(f == fs[8]);
            CHECK(s == ss[8]);
            CHECK(t == ts[8]);
    
            // Second Triangle
            s = t1 - 2u;
            t = t1 - 1u;

            CHECK(f == fs[9]);
            CHECK(s == ss[9]);
            CHECK(t == ts[9]);
    
            // BOTTOM
            // First Triangle
            f = t2;
            s = t2 + 3u;
            t = t2 + 2u;

            CHECK(f == fs[10]);
            CHECK(s == ss[10]);
            CHECK(t == ts[10]);
    
            // Second Triangle
            s = t2 + 2u;
            t = t2 + 1u;

            CHECK(f == fs[11]);
            CHECK(s == ss[11]);
            CHECK(t == ts[11]);
        }
    }
}