#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Tetrahedron.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>

class TestableTetrahedron : public Tetrahedron {
public:
    using Tetrahedron::Tetrahedron;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Tetrahedron.Generation") {
    static const std::vector<Vertex> expectedVertices = {
        //POSITION						 //TEX COORD	//NORMAL				             //TANGENT					//BITANGENT
        { { 0.f, -0.5f, 0.866025f }, { 0.5f, 1.f }, { 0.f, -1.f, 0.f }, { -1.f, -0.f, 0.f }, { 0.f, -0.f, 1.f } },
        { { 0.75f, -0.5f, -0.433013f }, { 0.f, 0.f }, { 0.f, -1.f, 0.f }, { -1.f, -0.f, 0.f }, { 0.f, -0.f, 1.f } },
        { { -0.75f, -0.5f, -0.433013f }, { 1.f, 0.f }, { 0.f, -1.f, 0.f }, { -1.f, -0.f, 0.f }, { 0.f, -0.f, 1.f } },
        { { 0.f, -0.5f, 0.866025f }, { 0.f, 1.f }, { 0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, -0.866025f }, { 0.288675f, -0.942809f, 0.166667f } },
        { { 0.75f, -0.5f, -0.433013f }, { 1.f, 1.f }, { 0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, -0.866025f }, { 0.288675f, -0.942809f, 0.166667f } },
        { { 0.f, 0.724745f, 0.f }, { 0.5f, 0.f }, { 0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, -0.866025f }, { 0.288675f, -0.942809f, 0.166667f } },
        { { 0.75f, -0.5f, -0.433013f }, { 0.f, 1.f }, { -0.f, 0.917663f, -0.39736f }, { -1.f, -0.f, 0.f }, { -0.f, -0.942809f, -0.333333f } },
        { { -0.75f, -0.5f, -0.433013f }, { 1.f, 1.f }, { -0.f, 0.917663f, -0.39736f }, { -1.f, -0.f, 0.f }, { -0.f, -0.942809f, -0.333333f } },
        { { 0.f, 0.724745f, 0.f }, { 0.5f, 0.f }, { -0.f, 0.917663f, -0.39736f }, { -1.f, -0.f, 0.f }, { -0.f, -0.942809f, -0.333333f } },
        { { -0.75f, -0.5f, -0.433013f }, { 0.f, 1.f }, { -0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, 0.866025f }, { -0.288675f, -0.942809f, 0.166667f } },
        { { 0.f, -0.5f, 0.866025f }, { 1.f, 1.f }, { -0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, 0.866025f }, { -0.288675f, -0.942809f, 0.166667f } },
        { { 0.f, 0.724745f, 0.f }, { 0.5f, 0.f }, { -0.344124f, 0.917663f, 0.19868f }, { 0.5f, 0.f, 0.866025f }, { -0.288675f, -0.942809f, 0.166667f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
        0, 2, 1,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11
    };

    TestableTetrahedron* tetrahedron = new TestableTetrahedron(ValuesRange::ONE_TO_ONE);

    const auto& v = tetrahedron->getVertices();
    const auto& i = tetrahedron->getIndices();

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