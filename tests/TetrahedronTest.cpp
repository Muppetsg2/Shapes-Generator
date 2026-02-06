#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Tetrahedron.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestableTetrahedron : public Tetrahedron {
public:
    using Tetrahedron::Tetrahedron;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Tetrahedron.Minimal.Valid") {
    ShapeConfig config{};
    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    const auto& v = tetrahedron.getVertices();
    const auto& i = tetrahedron.getIndices();

    REQUIRE_FALSE(v.empty());
    REQUIRE_FALSE(i.empty());

    for (unsigned idx : i) {
        REQUIRE(idx < v.size());
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.NoTangents") {
    ShapeConfig config{};
    config.genTangents = false;

    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : tetrahedron.getVertices()) {
        CHECK(v.Tangent == glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.NoBitangents") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = false;

    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : tetrahedron.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.PositiveHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : tetrahedron.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.NegativeHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = false;

    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : tetrahedron.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.Position.Range.OneToOne") {
    ShapeConfig config{};
    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : tetrahedron.getVertices()) {
        REQUIRE(v.Position.x >= -1.f);
        REQUIRE(v.Position.x <= 1.f);
        REQUIRE(v.Position.y >= -1.f);
        REQUIRE(v.Position.y <= 1.f);
        REQUIRE(v.Position.z >= -1.f);
        REQUIRE(v.Position.z <= 1.f);
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.Position.Range.HalfToHalf") {
    ShapeConfig config{};
    TestableTetrahedron tetrahedron(config, ValuesRange::HALF_TO_HALF);

    for (const auto& v : tetrahedron.getVertices()) {
        REQUIRE(v.Position.x >= -0.5f);
        REQUIRE(v.Position.x <= 0.5f);
        REQUIRE(v.Position.y >= -0.5f);
        REQUIRE(v.Position.y <= 0.5f);
        REQUIRE(v.Position.z >= -0.5f);
        REQUIRE(v.Position.z <= 0.5f);
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.Normals.UnitLength") {
    ShapeConfig config{};
    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : tetrahedron.getVertices()) {
        REQUIRE(glm::length(v.Normal) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.TexCoord.Range") {
    ShapeConfig config{};
    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    for (const auto& v : tetrahedron.getVertices()) {
        REQUIRE(v.TexCoord.x >= 0.f);
        REQUIRE(v.TexCoord.x <= 1.f);
        REQUIRE(v.TexCoord.y >= 0.f);
        REQUIRE(v.TexCoord.y <= 1.f);
    }
}

TEST_CASE("ShapesGenerator.Tetrahedron.IndexCount") {
    ShapeConfig config{};
    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    // Tetrahedron:
    // - 4 triangular faces
    // - 1 triangle per face
    REQUIRE(tetrahedron.getIndices().size() == 12);
}

TEST_CASE("ShapesGenerator.Tetrahedron.Generation(TBP)") {
    static const std::vector<Vertex> expectedVertices = {
           // POSITION                        // TEX COORD   // NORMAL                             // TANGENT                  // BITANGENT
        { {    0.f,     -0.5f,  0.866025f }, { 0.5f, 1.f }, {        0.f,      -1.f,       0.f }, { -1.f,  0.f,        0.f }, {       -0.f,     -0.f,       -1.f } },
        { {  0.75f,     -0.5f, -0.433013f }, {  0.f, 0.f }, {        0.f,      -1.f,       0.f }, { -1.f,  0.f,        0.f }, {       -0.f,     -0.f,       -1.f } },
        { { -0.75f,     -0.5f, -0.433013f }, {  1.f, 0.f }, {        0.f,      -1.f,       0.f }, { -1.f,  0.f,        0.f }, {       -0.f,     -0.f,       -1.f } },
        { {    0.f,     -0.5f,  0.866025f }, {  0.f, 1.f }, {  0.344124f, 0.917663f,  0.19868f }, { 0.5f, -0.f, -0.866025f }, { -0.794719f, 0.39736f, -0.458831f } },
        { {  0.75f,     -0.5f, -0.433013f }, {  1.f, 1.f }, {  0.344124f, 0.917663f,  0.19868f }, { 0.5f, -0.f, -0.866025f }, { -0.794719f, 0.39736f, -0.458831f } },
        { {    0.f, 0.724745f,        0.f }, { 0.5f, 0.f }, {  0.344124f, 0.917663f,  0.19868f }, { 0.5f, -0.f, -0.866025f }, { -0.794719f, 0.39736f, -0.458831f } },
        { {  0.75f,     -0.5f, -0.433013f }, {  0.f, 1.f }, {       -0.f, 0.917663f, -0.39736f }, { -1.f,  0.f,        0.f }, {        0.f, 0.39736f,  0.917663f } },
        { { -0.75f,     -0.5f, -0.433013f }, {  1.f, 1.f }, {       -0.f, 0.917663f, -0.39736f }, { -1.f,  0.f,        0.f }, {        0.f, 0.39736f,  0.917663f } },
        { {    0.f, 0.724745f,        0.f }, { 0.5f, 0.f }, {       -0.f, 0.917663f, -0.39736f }, { -1.f,  0.f,        0.f }, {        0.f, 0.39736f,  0.917663f } },
        { { -0.75f,     -0.5f, -0.433013f }, {  0.f, 1.f }, { -0.344124f, 0.917663f,  0.19868f }, { 0.5f,  0.f,  0.866025f }, {  0.794719f, 0.39736f, -0.458832f } },
        { {    0.f,     -0.5f,  0.866025f }, {  1.f, 1.f }, { -0.344124f, 0.917663f,  0.19868f }, { 0.5f,  0.f,  0.866025f }, {  0.794719f, 0.39736f, -0.458832f } },
        { {    0.f, 0.724745f,        0.f }, { 0.5f, 0.f }, { -0.344124f, 0.917663f,  0.19868f }, { 0.5f,  0.f,  0.866025f }, {  0.794719f, 0.39736f, -0.458832f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
         0,  2,  1,
         3,  4,  5,
         6,  7,  8,
         9, 10, 11
    };

    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestableTetrahedron tetrahedron(config, ValuesRange::ONE_TO_ONE);

    const auto& v = tetrahedron.getVertices();
    const auto& i = tetrahedron.getIndices();

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