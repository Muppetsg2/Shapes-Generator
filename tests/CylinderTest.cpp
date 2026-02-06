#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Cylinder.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestableCylinder : public Cylinder {
public:
    using Cylinder::Cylinder;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Cylinder.Minimal.Valid") {
    ShapeConfig config{};
    TestableCylinder cylinder(
        config,
        1u,  // height segments
        3u,  // radial segments (minimum)
        CylinderShading::SMOOTH,
        ValuesRange::ONE_TO_ONE
    );

    const auto& v = cylinder.getVertices();
    const auto& i = cylinder.getIndices();

    REQUIRE_FALSE(v.empty());
    REQUIRE_FALSE(i.empty());

    for (unsigned idx : i) {
        REQUIRE(idx < v.size());
    }
}

TEST_CASE("ShapesGenerator.Cylinder.NoTangents") {
    ShapeConfig config{};
    config.genTangents = false;

    TestableCylinder cylinder(
        config,
        2u,
        8u,
        CylinderShading::SMOOTH,
        ValuesRange::ONE_TO_ONE
    );

    for (const auto& v : cylinder.getVertices()) {
        CHECK(v.Tangent == glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Cylinder.NoBitangents") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = false;

    TestableCylinder cylinder(
        config,
        2u,
        8u,
        CylinderShading::SMOOTH,
        ValuesRange::ONE_TO_ONE
    );

    for (const auto& v : cylinder.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Cylinder.PositiveHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableCylinder cylinder(
        config,
        2u,
        8u,
        CylinderShading::SMOOTH,
        ValuesRange::ONE_TO_ONE
    );

    for (const auto& v : cylinder.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Cylinder.NegativeHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = false;

    TestableCylinder cylinder(
        config,
        2u,
        8u,
        CylinderShading::SMOOTH,
        ValuesRange::ONE_TO_ONE
    );

    for (const auto& v : cylinder.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Cylinder.Position.Range.OneToOne") {
    ShapeConfig config{};
    TestableCylinder cylinder(config, 5u, 16u, CylinderShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cylinder.getVertices()) {
        REQUIRE(v.Position.x >= -1.f);
        REQUIRE(v.Position.x <= 1.f);
        REQUIRE(v.Position.y >= -1.f);
        REQUIRE(v.Position.y <= 1.f);
        REQUIRE(v.Position.z >= -1.f);
        REQUIRE(v.Position.z <= 1.f);
    }
}

TEST_CASE("ShapesGenerator.Cylinder.Position.Range.HalfToHalf") {
    ShapeConfig config{};
    TestableCylinder cylinder(config, 5u, 16u, CylinderShading::SMOOTH, ValuesRange::HALF_TO_HALF);

    for (const auto& v : cylinder.getVertices()) {
        REQUIRE(v.Position.x >= -0.5f);
        REQUIRE(v.Position.x <= 0.5f);
        REQUIRE(v.Position.y >= -0.5f);
        REQUIRE(v.Position.y <= 0.5f);
        REQUIRE(v.Position.z >= -0.5f);
        REQUIRE(v.Position.z <= 0.5f);
    }
}

TEST_CASE("ShapesGenerator.Cylinder.Normals.UnitLength") {
    ShapeConfig config{};
    TestableCylinder cylinder(
        config,
        3u,
        16u,
        CylinderShading::SMOOTH,
        ValuesRange::ONE_TO_ONE
    );

    for (const auto& v : cylinder.getVertices()) {
        float len = glm::length(v.Normal);
        REQUIRE(len == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Cylinder.TexCoord.Range") {
    ShapeConfig config{};
    TestableCylinder cylinder(config,
        1u, 8u, CylinderShading::SMOOTH, ValuesRange::ONE_TO_ONE
    );

    for (const auto& v : cylinder.getVertices()) {
        REQUIRE(v.TexCoord.x >= 0.f);
        REQUIRE(v.TexCoord.x <= 1.f);
        REQUIRE(v.TexCoord.y >= 0.f);
        REQUIRE(v.TexCoord.y <= 1.f);
    }
}

TEST_CASE("ShapesGenerator.Cylinder.IndexCount") {
    constexpr unsigned h = 4;
    constexpr unsigned r = 12;

    ShapeConfig config{};
    TestableCylinder cylinder(
        config,
        h,
        r,
        CylinderShading::SMOOTH,
        ValuesRange::ONE_TO_ONE
    );

    // Cylinder:
    // - top cap:    r triangles
    // - bottom cap: r triangles
    // - side:       h * r * 2 triangles
    const unsigned expectedTriangles = r * 2 + h * r * 2;

    REQUIRE(cylinder.getIndices().size() == expectedTriangles * 3);
}

TEST_CASE("ShapesGenerator.Cylinder.Generation(V[3]H[1].FLAT.TBP)") {
    static const std::vector<Vertex> expectedVertices = {
           // POSITION                  // TEX COORD          // NORMAL                   // TANGENT                 // BITANGENT
        { {        0.f,  1.f,   1.f }, {      0.5f,   1.f }, {        0.f,  1.f,  0.f }, {  1.f, 0.f,        0.f }, { 0.f,  0.f, -1.f } },
        { {  0.866025f,  1.f, -0.5f }, { 0.933013f, 0.25f }, {        0.f,  1.f,  0.f }, {  1.f, 0.f,        0.f }, { 0.f,  0.f, -1.f } },
        { { -0.866025f,  1.f, -0.5f }, { 0.066987f, 0.25f }, {        0.f,  1.f,  0.f }, {  1.f, 0.f,        0.f }, { 0.f,  0.f, -1.f } },
        { {        0.f,  1.f,   0.f }, {      0.5f,  0.5f }, {        0.f,  1.f,  0.f }, {  1.f, 0.f,        0.f }, { 0.f,  0.f, -1.f } },
        { {        0.f,  1.f,   1.f }, {       0.f,   0.f }, {  0.866026f,  0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f,  1.f,  0.f } },
        { {  0.866025f,  1.f, -0.5f }, {       1.f,   0.f }, {  0.866026f,  0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f,  1.f,  0.f } },
        { {  0.866025f,  1.f, -0.5f }, {       0.f,   0.f }, {        0.f,  0.f, -1.f }, { -1.f, 0.f,        0.f }, { 0.f,  1.f,  0.f } },
        { { -0.866025f,  1.f, -0.5f }, {       1.f,   0.f }, {       -0.f,  0.f, -1.f }, { -1.f, 0.f,        0.f }, { 0.f,  1.f,  0.f } },
        { { -0.866025f,  1.f, -0.5f }, {       0.f,   0.f }, { -0.866025f,  0.f, 0.5f }, { 0.5f, 0.f,  0.866025f }, { 0.f,  1.f,  0.f } },
        { {        0.f,  1.f,   1.f }, {       1.f,   0.f }, { -0.866025f,  0.f, 0.5f }, { 0.5f, 0.f,  0.866025f }, { 0.f,  1.f,  0.f } },
        { {        0.f, -1.f,   1.f }, {       0.f,   1.f }, {  0.866026f,  0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f,  1.f,  0.f } },
        { {  0.866025f, -1.f, -0.5f }, {       1.f,   1.f }, {  0.866026f,  0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f,  1.f,  0.f } },
        { {  0.866025f, -1.f, -0.5f }, {       0.f,   1.f }, {       -0.f,  0.f, -1.f }, { -1.f, 0.f,        0.f }, { 0.f,  1.f,  0.f } },
        { { -0.866025f, -1.f, -0.5f }, {       1.f,   1.f }, {       -0.f,  0.f, -1.f }, { -1.f, 0.f,        0.f }, { 0.f,  1.f,  0.f } },
        { { -0.866025f, -1.f, -0.5f }, {       0.f,   1.f }, { -0.866025f,  0.f, 0.5f }, { 0.5f, 0.f,  0.866025f }, { 0.f,  1.f,  0.f } },
        { {        0.f, -1.f,   1.f }, {       1.f,   1.f }, { -0.866025f,  0.f, 0.5f }, { 0.5f, 0.f,  0.866025f }, { 0.f,  1.f,  0.f } },
        { {        0.f, -1.f,   1.f }, {      0.5f,   1.f }, {        0.f, -1.f,  0.f }, {  1.f, 0.f,        0.f }, { 0.f,  0.f,  1.f } },
        { {  0.866025f, -1.f, -0.5f }, { 0.933013f, 0.25f }, {        0.f, -1.f,  0.f }, {  1.f, 0.f,        0.f }, { 0.f,  0.f,  1.f } },
        { { -0.866025f, -1.f, -0.5f }, { 0.066987f, 0.25f }, {        0.f, -1.f,  0.f }, {  1.f, 0.f,        0.f }, { 0.f,  0.f,  1.f } },
        { {        0.f, -1.f,   0.f }, {      0.5f,  0.5f }, {        0.f, -1.f,  0.f }, {  1.f, 0.f,        0.f }, { 0.f,  0.f,  1.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
         0,  1,  3,
         1,  2,  3,
         2,  0,  3,
         4, 10,  5,
         5, 10, 11,
         6, 12,  7,
         7, 12, 13,
         8, 14,  9,
         9, 14, 15,
        17, 16, 19,
        18, 17, 19,
        16, 18, 19
    };

    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableCylinder cylinder(config, 1u, 3u, CylinderShading::FLAT, ValuesRange::ONE_TO_ONE);

    const auto& v = cylinder.getVertices();
    const auto& i = cylinder.getIndices();

    REQUIRE(v.size() == expectedVertices.size());
    REQUIRE(i.size() == expectedIndices.size());

    for (size_t idx = 0; idx < v.size(); ++idx) {
        CheckVec3Equal(v[idx].Position, expectedVertices[idx].Position, TEST_EPSILON, "Position", idx);
        CheckVec2Equal(v[idx].TexCoord, expectedVertices[idx].TexCoord, TEST_EPSILON, "TexCoord", idx);
        CheckVec3Equal(v[idx].Normal, expectedVertices[idx].Normal, TEST_EPSILON, "Normal", idx);
        CheckVec3Equal(v[idx].Tangent, expectedVertices[idx].Tangent, TEST_EPSILON, "Tangent", idx);
        CheckVec3Equal(v[idx].Bitangent, expectedVertices[idx].Bitangent, TEST_EPSILON, "Bitangent", idx);
    }

    for (size_t idx = 0; idx < i.size(); ++idx) {
        REQUIRE(i[idx] == expectedIndices[idx]);
    }
}

TEST_CASE("ShapesGenerator.Cylinder.Generation(V[6]H[2].SMOOTH.TBP)") {
    static const std::vector<Vertex> expectedVertices = {
           // POSITION                  // TEX COORD          // NORMAL                    // TANGENT                       // BITANGENT
        { {        0.f,  1.f,   1.f }, {      0.5f,   1.f }, {        0.f,  1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f, -1.f } },
        { {  0.866025f,  1.f,  0.5f }, { 0.933013f, 0.75f }, {        0.f,  1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f, -1.f } },
        { {  0.866025f,  1.f, -0.5f }, { 0.933013f, 0.25f }, {        0.f,  1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f, -1.f } },
        { {       -0.f,  1.f,  -1.f }, {      0.5f,   0.f }, {        0.f,  1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f, -1.f } },
        { { -0.866025f,  1.f, -0.5f }, { 0.066987f, 0.25f }, {        0.f,  1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f, -1.f } },
        { { -0.866025f,  1.f,  0.5f }, { 0.066987f, 0.75f }, {        0.f,  1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f, -1.f } },
        { {        0.f,  1.f,   0.f }, {      0.5f,  0.5f }, {        0.f,  1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f, -1.f } },
        { {        0.f,  1.f,   1.f }, {       0.f,   0.f }, {        0.f,  0.f,   1.f }, {  0.866025f, 0.f,      -0.5f }, { 0.f, 1.f,  0.f } },
        { {  0.866025f,  1.f,  0.5f }, { 0.166667f,   0.f }, {  0.866025f,  0.f,  0.5f }, {  0.654654f, 0.f, -0.755929f }, { 0.f, 1.f,  0.f } },
        { {  0.866025f,  1.f, -0.5f }, { 0.333333f,   0.f }, {  0.866025f,  0.f, -0.5f }, { -0.327327f, 0.f, -0.944911f }, { 0.f, 1.f,  0.f } },
        { {       -0.f,  1.f,  -1.f }, {      0.5f,   0.f }, {       -0.f,  0.f,  -1.f }, {  -0.98198f, 0.f, -0.188982f }, { 0.f, 1.f,  0.f } },
        { { -0.866025f,  1.f, -0.5f }, { 0.666667f,   0.f }, { -0.866025f,  0.f, -0.5f }, { -0.654654f, 0.f,  0.755929f }, { 0.f, 1.f,  0.f } },
        { { -0.866025f,  1.f,  0.5f }, { 0.833333f,   0.f }, { -0.866025f,  0.f,  0.5f }, {  0.327327f, 0.f,  0.944911f }, { 0.f, 1.f,  0.f } },
        { {       -0.f,  1.f,   1.f }, {       1.f,   0.f }, {       -0.f,  0.f,   1.f }, {  0.866025f, 0.f,       0.5f }, { 0.f, 1.f,  0.f } },
        { {        0.f,  0.f,   1.f }, {       0.f,  0.5f }, {        0.f,  0.f,   1.f }, {  0.866025f, 0.f,      -0.5f }, { 0.f, 1.f,  0.f } },
        { {  0.866025f,  0.f,  0.5f }, { 0.166667f,  0.5f }, {  0.866025f,  0.f,  0.5f }, {       0.5f, 0.f, -0.866025f }, { 0.f, 1.f,  0.f } },
        { {  0.866025f,  0.f, -0.5f }, { 0.333333f,  0.5f }, {  0.866025f,  0.f, -0.5f }, {      -0.5f, 0.f, -0.866025f }, { 0.f, 1.f,  0.f } },
        { {       -0.f,  0.f,  -1.f }, {      0.5f,  0.5f }, {       -0.f,  0.f,  -1.f }, {       -1.f, 0.f,        0.f }, { 0.f, 1.f,  0.f } },
        { { -0.866025f,  0.f, -0.5f }, { 0.666667f,  0.5f }, { -0.866025f,  0.f, -0.5f }, {      -0.5f, 0.f,  0.866025f }, { 0.f, 1.f,  0.f } },
        { { -0.866025f,  0.f,  0.5f }, { 0.833333f,  0.5f }, { -0.866025f,  0.f,  0.5f }, {       0.5f, 0.f,  0.866025f }, { 0.f, 1.f,  0.f } },
        { {       -0.f,  0.f,   1.f }, {       1.f,  0.5f }, {       -0.f,  0.f,   1.f }, {  0.866025f, 0.f,       0.5f }, { 0.f, 1.f,  0.f } },
        { {        0.f, -1.f,   1.f }, {       0.f,   1.f }, {        0.f,  0.f,   1.f }, {  0.866025f, 0.f,      -0.5f }, { 0.f, 1.f,  0.f } },
        { {  0.866025f, -1.f,  0.5f }, { 0.166667f,   1.f }, {  0.866025f,  0.f,  0.5f }, {  0.327327f, 0.f, -0.944911f }, { 0.f, 1.f,  0.f } },
        { {  0.866025f, -1.f, -0.5f }, { 0.333333f,   1.f }, {  0.866025f,  0.f, -0.5f }, { -0.654654f, 0.f, -0.755929f }, { 0.f, 1.f,  0.f } },
        { {       -0.f, -1.f,  -1.f }, {      0.5f,   1.f }, {       -0.f,  0.f,  -1.f }, { -0.981981f, 0.f,  0.188982f }, { 0.f, 1.f,  0.f } },
        { { -0.866025f, -1.f, -0.5f }, { 0.666667f,   1.f }, { -0.866025f,  0.f, -0.5f }, { -0.327327f, 0.f,  0.944911f }, { 0.f, 1.f,  0.f } },
        { { -0.866025f, -1.f,  0.5f }, { 0.833333f,   1.f }, { -0.866025f,  0.f,  0.5f }, {  0.654653f, 0.f,  0.755929f }, { 0.f, 1.f,  0.f } },
        { {       -0.f, -1.f,   1.f }, {       1.f,   1.f }, {       -0.f,  0.f,   1.f }, {  0.866025f, 0.f,       0.5f }, { 0.f, 1.f,  0.f } },
        { {        0.f, -1.f,   1.f }, {      0.5f,   1.f }, {        0.f, -1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f,  1.f } },
        { {  0.866025f, -1.f,  0.5f }, { 0.933013f, 0.75f }, {        0.f, -1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f,  1.f } },
        { {  0.866025f, -1.f, -0.5f }, { 0.933013f, 0.25f }, {        0.f, -1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f,  1.f } },
        { {       -0.f, -1.f,  -1.f }, {      0.5f,   0.f }, {        0.f, -1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f,  1.f } },
        { { -0.866025f, -1.f, -0.5f }, { 0.066987f, 0.25f }, {        0.f, -1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f,  1.f } },
        { { -0.866025f, -1.f,  0.5f }, { 0.066987f, 0.75f }, {        0.f, -1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f,  1.f } },
        { {        0.f, -1.f,   0.f }, {      0.5f,  0.5f }, {        0.f, -1.f,   0.f }, {        1.f, 0.f,        0.f }, { 0.f, 0.f,  1.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
         0,  1,  6,
         1,  2,  6,
         2,  3,  6,
         3,  4,  6,
         4,  5,  6,
         5,  0,  6,
         7, 14,  8,
         8, 14, 15,
         8, 15,  9,
         9, 15, 16,
         9, 16, 10,
        10, 16, 17,
        10, 17, 11,
        11, 17, 18,
        11, 18, 12,
        12, 18, 19,
        12, 19, 13,
        13, 19, 20,
        14, 21, 15,
        15, 21, 22,
        15, 22, 16,
        16, 22, 23,
        16, 23, 17,
        17, 23, 24,
        17, 24, 18,
        18, 24, 25,
        18, 25, 19,
        19, 25, 26,
        19, 26, 20,
        20, 26, 27,
        29, 28, 34,
        30, 29, 34,
        31, 30, 34,
        32, 31, 34,
        33, 32, 34,
        28, 33, 34
    };

    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableCylinder cylinder(config, 2u, 6u, CylinderShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    const auto& v = cylinder.getVertices();
    const auto& i = cylinder.getIndices();

    REQUIRE(v.size() == expectedVertices.size());
    REQUIRE(i.size() == expectedIndices.size());

    for (size_t idx = 0; idx < v.size(); ++idx) {
        CheckVec3Equal(v[idx].Position, expectedVertices[idx].Position, TEST_EPSILON, "Position", idx);
        CheckVec2Equal(v[idx].TexCoord, expectedVertices[idx].TexCoord, TEST_EPSILON, "TexCoord", idx);
        CheckVec3Equal(v[idx].Normal, expectedVertices[idx].Normal, TEST_EPSILON, "Normal", idx);
        CheckVec3Equal(v[idx].Tangent, expectedVertices[idx].Tangent, TEST_EPSILON, "Tangent", idx);
        CheckVec3Equal(v[idx].Bitangent, expectedVertices[idx].Bitangent, TEST_EPSILON, "Bitangent", idx);
    }

    for (size_t idx = 0; idx < i.size(); ++idx) {
        REQUIRE(i[idx] == expectedIndices[idx]);
    }
}