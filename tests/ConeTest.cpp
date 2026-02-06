#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Cone.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestableCone : public Cone {
public:
    using Cone::Cone;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Cone.Minimal.Valid") {
    ShapeConfig config{};
    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    const auto& v = cone.getVertices();
    const auto& i = cone.getIndices();

    REQUIRE_FALSE(v.empty());
    REQUIRE_FALSE(i.empty());

    for (unsigned idx : i) {
        REQUIRE(idx < v.size());
    }
}

TEST_CASE("ShapesGenerator.Cone.NoTangents") {
    ShapeConfig config{};
    config.genTangents = false;

    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        CHECK(v.Tangent == glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Cone.NoBitangents") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = false;

    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Cone.PositiveHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Cone.NegativeHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = false;

    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Cone.Position.Range.OneToOne") {
    ShapeConfig config{};
    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        CheckInRange(v.Position.x, -1.0f, 1.0f, TEST_EPSILON, "Position.x");
        CheckInRange(v.Position.y, -1.0f, 1.0f, TEST_EPSILON, "Position.y");
        CheckInRange(v.Position.z, -1.0f, 1.0f, TEST_EPSILON, "Position.z");
    }
}

TEST_CASE("ShapesGenerator.Cone.Position.Range.HalfToHalf") {
    ShapeConfig config{};
    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::HALF_TO_HALF);

    for (const auto& v : cone.getVertices()) {
        CheckInRange(v.Position.x, -0.5f, 0.5f, TEST_EPSILON, "Position.x");
        CheckInRange(v.Position.y, -0.5f, 0.5f, TEST_EPSILON, "Position.y");
        CheckInRange(v.Position.z, -0.5f, 0.5f, TEST_EPSILON, "Position.z");
    }
}

TEST_CASE("ShapesGenerator.Cone.Normals.UnitLength") {
    ShapeConfig config{};
    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        REQUIRE(glm::length(v.Normal) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Cone.Tangents.UnitLength") {
    ShapeConfig config{};
    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        REQUIRE(glm::length(v.Tangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Cone.Bitangents.UnitLength") {
    ShapeConfig config{};
    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        REQUIRE(glm::length(v.Bitangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Cone.TexCoord.Range") {
    ShapeConfig config{};
    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    for (const auto& v : cone.getVertices()) {
        CheckInRange(v.TexCoord.x, 0.0f, 1.0f, TEST_EPSILON, "TexCoord.x");
        CheckInRange(v.TexCoord.y, 0.0f, 1.0f, TEST_EPSILON, "TexCoord.y");
    }
}

TEST_CASE("ShapesGenerator.Cone.IndexCount") {
    constexpr unsigned w = 4;

    ShapeConfig config{};
    TestableCone cone(
        config,
        w,
        0.745f,
        1.0f,
        ConeShading::FLAT,
        ValuesRange::ONE_TO_ONE
    );

    // Cone (flat shading):
    // - side surface:  w triangles
    // - base cap:      w triangles
    const unsigned expectedTriangles = w * 2;

    REQUIRE(cone.getIndices().size() == expectedTriangles * 3);
}

TEST_CASE("ShapesGenerator.Cone.Generation(S[3]H[1.0]R[0.5].FLAT.TBP)") {
    static const std::vector<Vertex> expectedVertices = {
           // POSITION                             // TEX COORD          // NORMAL                              // TANGENT                  // BITANGENT
        { {        0.f, -0.707107f,  0.707107f }, {      0.5f,   1.f }, {        0.f,      -1.f,        0.f }, {  1.f,  0.f,        0.f }, {       -0.f,       0.f,        1.f } },
        { {  0.612372f, -0.707107f, -0.353554f }, { 0.933013f, 0.25f }, {        0.f,      -1.f,        0.f }, {  1.f,  0.f,       -0.f }, {        0.f,       0.f,        1.f } },
        { { -0.612373f, -0.707107f, -0.353553f }, { 0.066987f, 0.25f }, {        0.f,      -1.f,        0.f }, {  1.f,  0.f,       -0.f }, {        0.f,       0.f,        1.f } },
        { {        0.f, -0.707107f,        0.f }, {      0.5f,  0.5f }, {        0.f,      -1.f,        0.f }, {  1.f,  0.f,       -0.f }, {        0.f,       0.f,        1.f } },
        { {        0.f, -0.707107f,  0.707107f }, {       0.f,   1.f }, {  0.612372f, 0.707107f,  0.353553f }, { 0.5f, -0.f, -0.866025f }, { -0.612373f, 0.707107f, -0.353553f } },
        { {  0.612372f, -0.707107f, -0.353554f }, {       1.f,   1.f }, {  0.612372f, 0.707107f,  0.353553f }, { 0.5f, -0.f, -0.866025f }, { -0.612373f, 0.707107f, -0.353553f } },
        { {        0.f,        1.f,        0.f }, {      0.5f,   0.f }, {  0.612372f, 0.707107f,  0.353553f }, { 0.5f, -0.f, -0.866025f }, { -0.612373f, 0.707107f, -0.353553f } },
        { {  0.612372f, -0.707107f, -0.353554f }, {       0.f,   1.f }, {       -0.f, 0.707107f, -0.707107f }, { -1.f,  0.f,        0.f }, {        0.f, 0.707107f,  0.707107f } },
        { { -0.612373f, -0.707107f, -0.353553f }, {       1.f,   1.f }, {       -0.f, 0.707107f, -0.707107f }, { -1.f,  0.f,        0.f }, {        0.f, 0.707107f,  0.707107f } },
        { {        0.f,        1.f,        0.f }, {      0.5f,   0.f }, {       -0.f, 0.707107f, -0.707107f }, { -1.f,  0.f,        0.f }, {        0.f, 0.707107f,  0.707107f } },
        { { -0.612373f, -0.707107f, -0.353553f }, {       0.f,   1.f }, { -0.612372f, 0.707107f,  0.353554f }, { 0.5f,  0.f,  0.866025f }, {  0.612372f, 0.707107f, -0.353554f } },
        { {        0.f, -0.707107f,  0.707107f }, {       1.f,   1.f }, { -0.612372f, 0.707107f,  0.353554f }, { 0.5f,  0.f,  0.866025f }, {  0.612372f, 0.707107f, -0.353554f } },
        { {        0.f,        1.f,        0.f }, {      0.5f,   0.f }, { -0.612372f, 0.707107f,  0.353554f }, { 0.5f,  0.f,  0.866025f }, {  0.612372f, 0.707107f, -0.353554f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
         1,  0,  3,
         2,  1,  3,
         0,  2,  3,
         4,  5,  6,
         7,  8,  9,
        10, 11, 12
    };

    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableCone cone(config, 3u, 1.0f, 0.5f, ConeShading::FLAT, ValuesRange::ONE_TO_ONE);

    const auto& v = cone.getVertices();
    const auto& i = cone.getIndices();

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

TEST_CASE("ShapesGenerator.Cone.Generation(S[4]H[0.745]R[1.0].SMOOTH.TBP)") {
    static const std::vector<Vertex> expectedVertices = {
           // POSITION                             // TEX COORD              // NORMAL                              // TANGENT                              // BITANGENT
        { {        0.f, -0.707107f,  0.707107f }, {      0.5f,       1.f }, {        0.f,      -1.f,        0.f }, {        1.f,        0.f,       -0.f }, {        0.f,       0.f,        1.f } },
        { {  0.707107f, -0.707107f,       -0.f }, {       1.f,      0.5f }, {        0.f,      -1.f,        0.f }, {        1.f,        0.f,       -0.f }, {        0.f,       0.f,        1.f } },
        { {       -0.f, -0.707107f, -0.707107f }, {      0.5f,       0.f }, {        0.f,      -1.f,        0.f }, {        1.f,        0.f,       -0.f }, {        0.f,       0.f,        1.f } },
        { { -0.707107f, -0.707107f,        0.f }, {       0.f,      0.5f }, {        0.f,      -1.f,        0.f }, {        1.f,        0.f,       -0.f }, {        0.f,       0.f,        1.f } },
        { {        0.f, -0.707107f,        0.f }, {      0.5f,      0.5f }, {        0.f,      -1.f,        0.f }, {        1.f,        0.f,       -0.f }, {        0.f,       0.f,        1.f } },
        { {        0.f, -0.707107f,  0.707107f }, {       0.f, 0.866025f }, {        0.f, 0.447214f,  0.894427f }, {  0.800809f,   0.53569f, -0.267845f }, { -0.598919f, 0.716266f, -0.358133f } },
        { {  0.707107f, -0.707107f,       -0.f }, { 0.241181f, 0.965926f }, {  0.894427f, 0.447214f,       -0.f }, { -0.079552f,  0.159103f, -0.984052f }, { -0.440081f, 0.880163f,  0.177882f } },
        { {       -0.f, -0.707107f, -0.707107f }, {      0.5f,       1.f }, {       -0.f, 0.447214f, -0.894427f }, {       -1.f,        0.f,        0.f }, {        0.f, 0.894427f,  0.447214f } },
        { { -0.707107f, -0.707107f,        0.f }, { 0.758819f, 0.965926f }, { -0.894427f, 0.447214f,        0.f }, { -0.079551f, -0.159103f,  0.984052f }, {  0.440081f, 0.880163f,  0.177882f } },
        { {        0.f, -0.707107f,  0.707107f }, {       1.f, 0.866025f }, {        0.f, 0.447214f,  0.894427f }, {  0.800809f,  -0.53569f,  0.267845f }, {  0.598919f, 0.716266f, -0.358133f } },
        { {        0.f,        1.f,        0.f }, {      0.5f,       0.f }, {        0.f,       1.f,        0.f }, {       -1.f,        0.f,        0.f }, {        0.f,      -0.f,        1.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
         1,  0,  4,
         2,  1,  4,
         3,  2,  4,
         0,  3,  4,
         5,  6, 10,
         6,  7, 10,
         7,  8, 10,
         8,  9, 10
    };

    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableCone cone(config, 4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    const auto& v = cone.getVertices();
    const auto& i = cone.getIndices();

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