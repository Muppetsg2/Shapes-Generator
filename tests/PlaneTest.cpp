#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Plane.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestablePlane : public Plane {
public:
    using Plane::Plane;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Plane.Minimal.Valid") {
    ShapeConfig config{};
    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    const auto& v = plane.getVertices();
    const auto& i = plane.getIndices();

    REQUIRE_FALSE(v.empty());
    REQUIRE_FALSE(i.empty());

    for (unsigned idx : i) {
        REQUIRE(idx < v.size());
    }
}

TEST_CASE("ShapesGenerator.Plane.NoTangents") {
    ShapeConfig config{};
    config.genTangents = false;

    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        CHECK(v.Tangent == glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Plane.NoBitangents") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = false;

    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent == glm::vec3(0.f));
    }
}

TEST_CASE("ShapesGenerator.Plane.PositiveHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Plane.NegativeHandedness") {
    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = false;

    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        CHECK(v.Tangent != glm::vec3(0.f));
        CHECK(v.Bitangent != glm::vec3(0.f));

        glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
        CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
    }
}

TEST_CASE("ShapesGenerator.Plane.Position.Range.OneToOne") {
    ShapeConfig config{};
    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        CheckInRange(v.Position.x, -1.0f, 1.0f, TEST_EPSILON, "Position.x");
        CheckInRange(v.Position.y, -1.0f, 1.0f, TEST_EPSILON, "Position.y");
        CheckInRange(v.Position.z, -1.0f, 1.0f, TEST_EPSILON, "Position.z");
    }
}

TEST_CASE("ShapesGenerator.Plane.Position.Range.HalfToHalf") {
    ShapeConfig config{};
    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::HALF_TO_HALF);

    for (const auto& v : plane.getVertices()) {
        CheckInRange(v.Position.x, -0.5f, 0.5f, TEST_EPSILON, "Position.x");
        CheckInRange(v.Position.y, -0.5f, 0.5f, TEST_EPSILON, "Position.y");
        CheckInRange(v.Position.z, -0.5f, 0.5f, TEST_EPSILON, "Position.z");
    }
}

TEST_CASE("ShapesGenerator.Plane.Normals.UnitLength") {
    ShapeConfig config{};
    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        REQUIRE(glm::length(v.Normal) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Plane.Tangents.UnitLength") {
    ShapeConfig config{};
    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        REQUIRE(glm::length(v.Tangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Plane.Bitangents.UnitLength") {
    ShapeConfig config{};
    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        REQUIRE(glm::length(v.Bitangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
    }
}

TEST_CASE("ShapesGenerator.Plane.TexCoord.Range") {
    ShapeConfig config{};
    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    for (const auto& v : plane.getVertices()) {
        CheckInRange(v.TexCoord.x, 0.0f, 1.0f, TEST_EPSILON, "TexCoord.x");
        CheckInRange(v.TexCoord.y, 0.0f, 1.0f, TEST_EPSILON, "TexCoord.y");
    }
}

TEST_CASE("ShapesGenerator.Plane.IndexCount") {
    constexpr unsigned r = 4;
    constexpr unsigned c = 6;

    ShapeConfig config{};
    TestablePlane plane(
        config,
        r,
        c,
        PlaneNormalDir::UP,
        ValuesRange::ONE_TO_ONE
    );

    // Plane (grid):
    // - (r - 1) * (c - 1) quads
    // - each quad -> 2 triangles
    const unsigned expectedTriangles = (r - 1) * (c - 1) * 2;

    REQUIRE(plane.getIndices().size() == expectedTriangles * 3);
}

TEST_CASE("ShapesGenerator.Plane.Generation(R[2]C[2].UP.TBP)") {
    static const std::vector<Vertex> expectedVertices = {
           // POSITION          // TEX COORD  // NORMAL          // TANGENT          // BITANGENT
        { { -1.f, 0.f, -1.f }, { 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 1.f, -0.f, 0.f }, { 0.f, 0.f, -1.f } },
        { {  1.f, 0.f, -1.f }, { 1.f, 0.f }, { 0.f, 1.f, 0.f }, { 1.f, -0.f, 0.f }, { 0.f, 0.f, -1.f } },
        { { -1.f, 0.f,  1.f }, { 0.f, 1.f }, { 0.f, 1.f, 0.f }, { 1.f, -0.f, 0.f }, { 0.f, 0.f, -1.f } },
        { {  1.f, 0.f,  1.f }, { 1.f, 1.f }, { 0.f, 1.f, 0.f }, { 1.f, -0.f, 0.f }, { 0.f, 0.f, -1.f } },
    };

    static const std::vector<unsigned int> expectedIndices = {
        2, 1, 0,
        2, 3, 1
    };

    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestablePlane plane(config, 2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    const auto& v = plane.getVertices();
    const auto& i = plane.getIndices();

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

TEST_CASE("ShapesGenerator.Plane.Generation(R[3]C[3].FRONT.TBP)") {
    static const std::vector<Vertex> expectedVertices = {
           // POSITION          // TEX COORD    // NORMAL           // TANGENT         // BITANGENT
        { { -1.f, -1.f, 0.f }, {  0.f,  0.f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { {  0.f, -1.f, 0.f }, { 0.5f,  0.f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { {  1.f, -1.f, 0.f }, {  1.f,  0.f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { { -1.f,  0.f, 0.f }, {  0.f, 0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { {  0.f,  0.f, 0.f }, { 0.5f, 0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { {  1.f,  0.f, 0.f }, {  1.f, 0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { { -1.f,  1.f, 0.f }, {  0.f,  1.f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { {  0.f,  1.f, 0.f }, { 0.5f,  1.f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { {  1.f,  1.f, 0.f }, {  1.f,  1.f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
        3, 1, 0,
        3, 4, 1,
        4, 2, 1,
        4, 5, 2,
        6, 4, 3,
        6, 7, 4,
        7, 5, 4,
        7, 8, 5
    };

    ShapeConfig config{};
    config.genTangents = true;
    config.calcBitangents = true;
    config.tangentHandednessPositive = true;

    TestablePlane plane(config, 3u, 3u, PlaneNormalDir::FRONT, ValuesRange::ONE_TO_ONE);

    const auto& v = plane.getVertices();
    const auto& i = plane.getIndices();

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