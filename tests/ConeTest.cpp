#include "pch.hpp"
#include "BitMathOperators.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Cone.hpp"

#include "Helpers.hpp"

#include <vector>

class TestableCone : public Cone {
public:
    using Cone::Cone;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Cone.Generation(S3H1.0R0.5FLAT)") {
    static const std::vector<Vertex> expectedVertices = {
        //POSITION						//TEX COORD		//NORMAL					//TANGENT					//BITANGENT
        { { 0.f, -0.707107f, 0.707107f }, { 0.5f, 1.f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 0.f, 1.f } },
        { { 0.612372f, -0.707107f, -0.353554f }, { 0.933013f, 0.25f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { -0.f, 0.f, 1.f } },
        { { -0.612373f, -0.707107f, -0.353553f }, { 0.066987f, 0.25f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { -0.f, 0.f, 1.f } },
        { { 0.f, -0.707107f, 0.f }, { 0.5f, 0.5f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { -0.f, 0.f, 1.f } },
        { { 0.f, -0.707107f, 0.707107f }, { 0.f, 1.f }, { 0.612372f, 0.707107f, 0.353553f }, { 0.5f, 0.f, -0.866025f }, { 0.175633f, -0.97922f, 0.101401f } },
        { { 0.612372f, -0.707107f, -0.353554f }, { 1.f, 1.f }, { 0.612372f, 0.707107f, 0.353553f }, { 0.5f, 0.f, -0.866025f }, { 0.175633f, -0.97922f, 0.101401f } },
        { { 0.f, 1.f, 0.f }, { 0.5f, 0.f }, { 0.612372f, 0.707107f, 0.353553f }, { 0.5f, 0.f, -0.866025f }, { 0.175633f, -0.97922f, 0.101401f } },
        { { 0.612372f, -0.707107f, -0.353554f }, { 0.f, 1.f }, { -0.f, 0.707107f, -0.707107f }, { -1.f, 0.f, 0.f }, { -0.f, -0.97922f, -0.202803f } },
        { { -0.612373f, -0.707107f, -0.353553f }, { 1.f, 1.f }, { -0.f, 0.707107f, -0.707107f }, { -1.f, 0.f, 0.f }, { -0.f, -0.97922f, -0.202803f } },
        { { 0.f, 1.f, 0.f }, { 0.5f, 0.f }, { -0.f, 0.707107f, -0.707107f }, { -1.f, 0.f, 0.f }, { -0.f, -0.97922f, -0.202803f } },
        { { -0.612373f, -0.707107f, -0.353553f }, { 0.f, 1.f }, { -0.612372f, 0.707107f, 0.353554f }, { 0.5f, 0.f, 0.866025f }, { -0.175633f, -0.97922f, 0.101402f } },
        { { 0.f, -0.707107f, 0.707107f }, { 1.f, 1.f }, { -0.612372f, 0.707107f, 0.353554f }, { 0.5f, 0.f, 0.866025f }, { -0.175633f, -0.97922f, 0.101402f } },
        { { 0.f, 1.f, 0.f }, { 0.5f, 0.f }, { -0.612372f, 0.707107f, 0.353554f }, { 0.5f, 0.f, 0.866025f }, { -0.175633f, -0.97922f, 0.101402f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
        1, 0, 3,
        2, 1, 3,
        0, 2, 3,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12
    };

    TestableCone* cone = new TestableCone(3u, 1.0f, 0.5f, ConeShading::FLAT, ValuesRange::ONE_TO_ONE);

    const auto& v = cone->getVertices();
    const auto& i = cone->getIndices();

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

TEST_CASE("ShapesGenerator.Cone.Generation(S4H0.745R1.0SMOOTH)") {
    static const std::vector<Vertex> expectedVertices = {
        //POSITION						//TEX COORD		//NORMAL					//TANGENT					//BITANGENT
        { { 0.f, -0.707107f, 0.707107f }, { 0.5f, 1.f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { 0.707107f, -0.707107f, -0.f }, { 1.f, 0.5f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { -0.f, -0.707107f, -0.707107f }, { 0.5f, 0.f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { -0.707107f, -0.707107f, 0.f }, { 0.f, 0.5f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { 0.f, -0.707107f, 0.f }, { 0.5f, 0.5f }, { 0.f, -1.f, 0.f }, { 1.f, 0.f, -0.f }, { 0.f, 0.f, 1.f } },
        { { 0.f, -0.707107f, 0.707107f }, { 0.f, 0.866025f }, { 0.f, 0.447214f, 0.894427f }, { 0.656309f, 0.182777f, -0.732018f }, { 0.617323f, -0.718888f, -0.31955f } },
        { { 0.707107f, -0.707107f, -0.f }, { 0.241181f, 0.965926f }, { 0.894427f, 0.447214f, -0.f }, { -0.042626f, 0.167106f, -0.985017f }, { 0.326589f, -0.869091f, -0.37151f } },
        { { -0.f, -0.707107f, -0.707107f }, { 0.5f, 1.f }, { -0.f, 0.447214f, -0.894427f }, { -1.f, 0.f, 0.f }, { -0.f, -0.92388f, -0.382683f } },
        { { -0.707107f, -0.707107f, 0.f }, { 0.758819f, 0.965926f }, { -0.894427f, 0.447214f, 0.f }, { -0.042626f, -0.167106f, 0.985017f }, { -0.326589f, -0.869091f, -0.37151f } },
        { { 0.f, -0.707107f, 0.707107f }, { 1.f, 0.866025f }, { 0.f, 0.447214f, 0.894427f }, { 0.65631f, -0.182777f, 0.732018f }, { -0.617323f, -0.718888f, -0.31955f } },
        { { 0.f, 1.f, 0.f }, { 0.5f, 0.f }, { 0.f, 1.f, 0.f }, { -1.f, -0.000001f, 0.000001f }, { -0.f, -0.919511f, -0.393063f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
        1, 0, 4,
        2, 1, 4,
        3, 2, 4,
        0, 3, 4,
        5, 6, 10,
        6, 7, 10,
        7, 8, 10,
        8, 9, 10
    };

    TestableCone* cone = new TestableCone(4u, 0.745f, 1.0f, ConeShading::SMOOTH, ValuesRange::ONE_TO_ONE);

    const auto& v = cone->getVertices();
    const auto& i = cone->getIndices();

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