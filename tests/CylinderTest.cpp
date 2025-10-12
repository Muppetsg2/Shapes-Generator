#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Cylinder.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>

class TestableCylinder : public Cylinder {
public:
    using Cylinder::Cylinder;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Cylinder.Generation(V3H1FLAT)") {
    static const std::vector<Vertex> expectedVertices = {
        //POSITION						//TEX COORD		//NORMAL					//TANGENT					//BITANGENT
        { { 0.f, 1.f, 1.f }, { 0.5f, 1.f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.866025f, 1.f, -0.5f }, { 0.933013f, 0.25f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { -0.866025f, 1.f, -0.5f }, { 0.066987f, 0.25f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.f, 1.f, 0.f }, { 0.5f, 0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.f, 1.f, 1.f }, { 0.f, 0.f }, { 0.866026f, 0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, 1.f, -0.5f }, { 1.f, 0.f }, { 0.866026f, 0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, 1.f, -0.5f }, { 0.f, 0.f }, { -0.f, 0.f, -1.f }, { -1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, 1.f, -0.5f }, { 1.f, 0.f }, { -0.f, 0.f, -1.f }, { -1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, 1.f, -0.5f }, { 0.f, 0.f }, { -0.866025f, 0.f, 0.5f }, { 0.5f, 0.f, 0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.f, 1.f, 1.f }, { 1.f, 0.f }, { -0.866025f, 0.f, 0.5f }, { 0.5f, 0.f, 0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.f, -1.f, 1.f }, { 0.f, 1.f }, { 0.866026f, 0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, -1.f, -0.5f }, { 1.f, 1.f }, { 0.866026f, 0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, -1.f, -0.5f }, { 0.f, 1.f }, { -0.f, 0.f, -1.f }, { -1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, -1.f, -0.5f }, { 1.f, 1.f }, { -0.f, 0.f, -1.f }, { -1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, -1.f, -0.5f }, { 0.f, 1.f }, { -0.866025f, 0.f, 0.5f }, { 0.5f, 0.f, 0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.f, -1.f, 1.f }, { 1.f, 1.f }, { -0.866025f, 0.f, 0.5f }, { 0.5f, 0.f, 0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.f, -1.f, 1.f }, { 0.5f, 1.f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.866025f, -1.f, -0.5f }, { 0.933013f, 0.25f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { -0.866025f, -1.f, -0.5f }, { 0.066987f, 0.25f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.f, -1.f, 0.f }, { 0.5f, 0.5f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
        0, 1, 3,
        1, 2, 3,
        2, 0, 3,
        4, 10, 5,
        5, 10, 11,
        6, 12, 7,
        7, 12, 13,
        8, 14, 9,
        9, 14, 15,
        17, 16, 19,
        18, 17, 19,
        16, 18, 19
    };

    TestableCylinder* cylinder = new TestableCylinder(1u, 3u, CylinderShading::FLAT, ValuesRange::ONE_TO_ONE);

    const auto& v = cylinder->getVertices();
    const auto& i = cylinder->getIndices();

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

TEST_CASE("ShapesGenerator.Cylinder.Generation(V6H2SMOOTH)") {
    static const std::vector<Vertex> expectedVertices = {
        //POSITION						//TEX COORD		//NORMAL					//TANGENT					//BITANGENT
        { { 0.f, 1.f, 1.f }, { 0.5f, 1.f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.866025f, 1.f, 0.5f }, { 0.933013f, 0.75f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { 0.866025f, 1.f, -0.5f }, { 0.933013f, 0.25f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { -0.f, 1.f, -1.f }, { 0.5f, 0.f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { -0.866025f, 1.f, -0.5f }, { 0.066987f, 0.25f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { -0.866025f, 1.f, 0.5f }, { 0.066987f, 0.75f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.f, 1.f, 0.f }, { 0.5f, 0.5f }, { 0.f, 1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { 0.f, 1.f, 1.f }, { 0.f, 0.f }, { 0.f, 0.f, 1.f }, { 0.866025f, 0.f, -0.5f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, 1.f, 0.5f }, { 0.166667f, 0.f }, { 0.866025f, 0.f, 0.5f }, { 0.654654f, 0.f, -0.755929f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, 1.f, -0.5f }, { 0.333333f, 0.f }, { 0.866025f, 0.f, -0.5f }, { -0.327327f, 0.f, -0.944911f }, { 0.f, -1.f, 0.f } },
        { { -0.f, 1.f, -1.f }, { 0.5f, 0.f }, { -0.f, 0.f, -1.f }, { -0.98198f, 0.f, -0.188982f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, 1.f, -0.5f }, { 0.666667f, 0.f }, { -0.866025f, 0.f, -0.5f }, { -0.654654f, 0.f, 0.755929f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, 1.f, 0.5f }, { 0.833333f, 0.f }, { -0.866025f, 0.f, 0.5f }, { 0.327327f, 0.f, 0.944911f }, { 0.f, -1.f, 0.f } },
        { { -0.f, 1.f, 1.f }, { 1.f, 0.f }, { -0.f, 0.f, 1.f }, { 0.866025f, 0.f, 0.5f }, { 0.f, -1.f, 0.f } },
        { { 0.f, 0.f, 1.f }, { 0.f, 0.5f }, { 0.f, 0.f, 1.f }, { 0.866025f, 0.f, -0.5f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, 0.f, 0.5f }, { 0.166667f, 0.5f }, { 0.866025f, 0.f, 0.5f }, { 0.5f, 0.f, -0.866025f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, 0.f, -0.5f }, { 0.333333f, 0.5f }, { 0.866025f, 0.f, -0.5f }, { -0.5f, 0.f, -0.866025f }, { 0.f, -1.f, 0.f } },
        { { -0.f, 0.f, -1.f }, { 0.5f, 0.5f }, { -0.f, 0.f, -1.f }, { -1.f, 0.f, 0.f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, 0.f, -0.5f }, { 0.666667f, 0.5f }, { -0.866025f, 0.f, -0.5f }, { -0.5f, 0.f, 0.866025f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, 0.f, 0.5f }, { 0.833333f, 0.5f }, { -0.866025f, 0.f, 0.5f }, { 0.5f, 0.f, 0.866025f }, { 0.f, -1.f, 0.f } },
        { { -0.f, 0.f, 1.f }, { 1.f, 0.5f }, { -0.f, 0.f, 1.f }, { 0.866025f, 0.f, 0.5f }, { 0.f, -1.f, 0.f } },
        { { 0.f, -1.f, 1.f }, { 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.866025f, 0.f, -0.5f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, -1.f, 0.5f }, { 0.166667f, 1.f }, { 0.866025f, 0.f, 0.5f }, { 0.327327f, 0.f, -0.944911f }, { 0.f, -1.f, 0.f } },
        { { 0.866025f, -1.f, -0.5f }, { 0.333333f, 1.f }, { 0.866025f, 0.f, -0.5f }, { -0.654654f, 0.f, -0.755929f }, { 0.f, -1.f, 0.f } },
        { { -0.f, -1.f, -1.f }, { 0.5f, 1.f }, { -0.f, 0.f, -1.f }, { -0.981981f, 0.f, 0.188982f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, -1.f, -0.5f }, { 0.666667f, 1.f }, { -0.866025f, 0.f, -0.5f }, { -0.327327f, 0.f, 0.944911f }, { 0.f, -1.f, 0.f } },
        { { -0.866025f, -1.f, 0.5f }, { 0.833333f, 1.f }, { -0.866025f, 0.f, 0.5f }, { 0.654653f, 0.f, 0.755929f }, { 0.f, -1.f, 0.f } },
        { { -0.f, -1.f, 1.f }, { 1.f, 1.f }, { -0.f, 0.f, 1.f }, { 0.866025f, 0.f, 0.5f }, { 0.f, -1.f, 0.f } },
        { { 0.f, -1.f, 1.f }, { 0.5f, 1.f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { 0.866025f, -1.f, 0.5f }, { 0.933013f, 0.75f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { 0.866025f, -1.f, -0.5f }, { 0.933013f, 0.25f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { -0.f, -1.f, -1.f }, { 0.5f, 0.f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { -0.866025f, -1.f, -0.5f }, { 0.066987f, 0.25f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { -0.866025f, -1.f, 0.5f }, { 0.066987f, 0.75f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.f, -1.f, 0.f }, { 0.5f, 0.5f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
        0, 1, 6,
        1, 2, 6,
        2, 3, 6,
        3, 4, 6,
        4, 5, 6,
        5, 0, 6,
        7, 14, 8,
        8, 14, 15,
        8, 15, 9,
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

    TestableCylinder* cylinder = new TestableCylinder(2u, 6u, CylinderShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    const auto& v = cylinder->getVertices();
    const auto& i = cylinder->getIndices();

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