#include "pch.hpp"
#include "BitMathOperators.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Cube.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestableCube : public Cube {
public:
    using Cube::Cube;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Cube.Minimal.Valid") {
    ShapeConfig config{};
    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    const auto& v = cube.getVertices();
    const auto& i = cube.getIndices();

    REQUIRE_FALSE(v.empty());
    REQUIRE_FALSE(i.empty());

    for (unsigned idx : i) {
        REQUIRE(idx < v.size());
    }
}

TEST_CASE("ShapesGenerator.Cube.NoTangents") {
    ShapeConfig config{};
    config.genTangents = false;

    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        CHECK(v.Tangent == glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Cube.NoBitangents") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = false;

    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Cube.PositiveHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Cube.NegativeHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = false;

    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Cube.Position.Range.OneToOne") {
    ShapeConfig config{};
    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        CheckInRange(v.Position.x, -1.0f, 1.0f, TEST_EPSILON, "Position.x");
        CheckInRange(v.Position.y, -1.0f, 1.0f, TEST_EPSILON, "Position.y");
        CheckInRange(v.Position.z, -1.0f, 1.0f, TEST_EPSILON, "Position.z");
    }
}

TEST_CASE("ShapesGenerator.Cube.Position.Range.HalfToHalf") {
    ShapeConfig config{};
    TestableCube cube(config, ValuesRange::HALF_TO_HALF);

    for (const auto& v : cube.getVertices()) {
        CheckInRange(v.Position.x, -0.5f, 0.5f, TEST_EPSILON, "Position.x");
        CheckInRange(v.Position.y, -0.5f, 0.5f, TEST_EPSILON, "Position.y");
        CheckInRange(v.Position.z, -0.5f, 0.5f, TEST_EPSILON, "Position.z");
    }
}

TEST_CASE("ShapesGenerator.Cube.Normals.UnitLength") {
    ShapeConfig config{};
    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        REQUIRE(glm::length(v.Normal) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Cube.Tangents.UnitLength") {
    ShapeConfig config{};
    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        REQUIRE(glm::length(v.Tangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Cube.Bitangents.UnitLength") {
    ShapeConfig config{};
    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        REQUIRE(glm::length(v.Bitangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Cube.TexCoord.Range") {
    ShapeConfig config{};
    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cube.getVertices()) {
        CheckInRange(v.TexCoord.x, 0.0f, 1.0f, TEST_EPSILON, "TexCoord.x");
        CheckInRange(v.TexCoord.y, 0.0f, 1.0f, TEST_EPSILON, "TexCoord.y");
    }
}

TEST_CASE("ShapesGenerator.Cube.IndexCount") {
    ShapeConfig config{};
    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    // Cube:
    // 6 faces * 2 triangles * 3 indices
    REQUIRE(cube.getIndices().size() == 36);
}

TEST_CASE("ShapesGenerator.Cube.Generation(TBP)") {
    static const std::vector<Vertex> expectedVertices = {
           // POSITION           // TEX COORD  // NORMAL             // TANGENT           // BITANGENT
        { { -1.f,  1.f,  1.f }, { 0.f, 0.f }, {  0.f,  0.f,  1.f }, {  1.f, 0.f,  0.f }, { 0.f, 1.f,  0.f } },
        { {  1.f,  1.f,  1.f }, { 1.f, 0.f }, {  0.f,  0.f,  1.f }, {  1.f, 0.f,  0.f }, { 0.f, 1.f,  0.f } },
        { {  1.f,  1.f, -1.f }, { 0.f, 0.f }, {  0.f,  0.f, -1.f }, { -1.f, 0.f,  0.f }, { 0.f, 1.f,  0.f } },
        { { -1.f,  1.f, -1.f }, { 1.f, 0.f }, {  0.f,  0.f, -1.f }, { -1.f, 0.f,  0.f }, { 0.f, 1.f,  0.f } },
        { { -1.f, -1.f,  1.f }, { 0.f, 1.f }, {  0.f,  0.f,  1.f }, {  1.f, 0.f,  0.f }, { 0.f, 1.f,  0.f } },
        { {  1.f, -1.f,  1.f }, { 1.f, 1.f }, {  0.f,  0.f,  1.f }, {  1.f, 0.f,  0.f }, { 0.f, 1.f,  0.f } },
        { {  1.f, -1.f, -1.f }, { 0.f, 1.f }, {  0.f,  0.f, -1.f }, { -1.f, 0.f,  0.f }, { 0.f, 1.f,  0.f } },
        { { -1.f, -1.f, -1.f }, { 1.f, 1.f }, {  0.f,  0.f, -1.f }, { -1.f, 0.f,  0.f }, { 0.f, 1.f,  0.f } },
        { { -1.f,  1.f,  1.f }, { 1.f, 0.f }, { -1.f,  0.f,  0.f }, {  0.f, 0.f,  1.f }, { 0.f, 1.f,  0.f } },
        { {  1.f,  1.f,  1.f }, { 0.f, 0.f }, {  1.f,  0.f,  0.f }, {  0.f, 0.f, -1.f }, { 0.f, 1.f,  0.f } },
        { {  1.f,  1.f, -1.f }, { 1.f, 0.f }, {  1.f,  0.f,  0.f }, {  0.f, 0.f, -1.f }, { 0.f, 1.f,  0.f } },
        { { -1.f,  1.f, -1.f }, { 0.f, 0.f }, { -1.f,  0.f,  0.f }, {  0.f, 0.f,  1.f }, { 0.f, 1.f,  0.f } },
        { { -1.f, -1.f,  1.f }, { 1.f, 1.f }, { -1.f,  0.f,  0.f }, {  0.f, 0.f,  1.f }, { 0.f, 1.f,  0.f } },
        { {  1.f, -1.f,  1.f }, { 0.f, 1.f }, {  1.f,  0.f,  0.f }, {  0.f, 0.f, -1.f }, { 0.f, 1.f,  0.f } },
        { {  1.f, -1.f, -1.f }, { 1.f, 1.f }, {  1.f,  0.f,  0.f }, {  0.f, 0.f, -1.f }, { 0.f, 1.f,  0.f } },
        { { -1.f, -1.f, -1.f }, { 0.f, 1.f }, { -1.f,  0.f,  0.f }, {  0.f, 0.f,  1.f }, { 0.f, 1.f,  0.f } },
        { { -1.f,  1.f,  1.f }, { 0.f, 1.f }, {  0.f,  1.f,  0.f }, {  1.f, 0.f,  0.f }, { 0.f, 0.f, -1.f } },
        { {  1.f,  1.f,  1.f }, { 1.f, 1.f }, {  0.f,  1.f,  0.f }, {  1.f, 0.f,  0.f }, { 0.f, 0.f, -1.f } },
        { {  1.f,  1.f, -1.f }, { 1.f, 0.f }, {  0.f,  1.f,  0.f }, {  1.f, 0.f,  0.f }, { 0.f, 0.f, -1.f } },
        { { -1.f,  1.f, -1.f }, { 0.f, 0.f }, {  0.f,  1.f,  0.f }, {  1.f, 0.f,  0.f }, { 0.f, 0.f, -1.f } },
        { { -1.f, -1.f,  1.f }, { 0.f, 0.f }, {  0.f, -1.f,  0.f }, {  1.f, 0.f,  0.f }, { 0.f, 0.f,  1.f } },
        { {  1.f, -1.f,  1.f }, { 1.f, 0.f }, {  0.f, -1.f,  0.f }, {  1.f, 0.f,  0.f }, { 0.f, 0.f,  1.f } },
        { {  1.f, -1.f, -1.f }, { 1.f, 1.f }, {  0.f, -1.f,  0.f }, {  1.f, 0.f,  0.f }, { 0.f, 0.f,  1.f } },
        { { -1.f, -1.f, -1.f }, { 0.f, 1.f }, {  0.f, -1.f,  0.f }, {  1.f, 0.f,  0.f }, { 0.f, 0.f,  1.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
         0,  4,  5,
         0,  5,  1,
         2,  6,  7,
         2,  7,  3,
         9, 13, 14,
         9, 14, 10,
        11, 15, 12,
        11, 12,  8,
        19, 16, 17,
        19, 17, 18,
        20, 23, 22,
        20, 22, 21
    };

    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableCube cube(config, ValuesRange::ONE_TO_ONE);

    const auto& v = cube.getVertices();
    const auto& i = cube.getIndices();

    REQUIRE(v.size() == expectedVertices.size());
    REQUIRE(i.size() == expectedIndices.size());

    for (size_t idx = 0; idx < v.size(); ++idx) {
        CheckVec3Equal(v[idx].Position, expectedVertices[idx].Position, TEST_EPSILON, "Position");
        CheckVec2Equal(v[idx].TexCoord, expectedVertices[idx].TexCoord, TEST_EPSILON, "TexCoord");
        CheckVec3Equal(v[idx].Normal, expectedVertices[idx].Normal, TEST_EPSILON, "Normal");
        CheckVec3Equal(v[idx].Tangent, expectedVertices[idx].Tangent, TEST_EPSILON, "Tangent");
        CheckVec3Equal(v[idx].Bitangent, expectedVertices[idx].Bitangent, TEST_EPSILON, "Bitangent");
    }

    for (size_t idx = 0; idx < i.size(); ++idx) {
        REQUIRE(i[idx] == expectedIndices[idx]);
    }
}

TEST_CASE("Calculations.Cube") {

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