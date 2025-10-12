#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Pyramid.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>

class TestablePyramid : public Pyramid {
public:
    using Pyramid::Pyramid;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Pyramid.Generation") {
    static const std::vector<Vertex> expectedVertices = {
        //POSITION						//TEX COORD		//NORMAL					     //TANGENT			    //BITANGENT
        { { -1.f, -0.707107f, 1.f },    { 0.f, 0.f },   { 0.f, -1.f, 0.f },              { 1.f, 0.f, 0.f },     { 0.f, 0.f, -1.f } },
        { { 1.f, -0.707107f, 1.f },     { 1.f, 0.f },   { 0.f, -1.f, 0.f },              { 1.f, 0.f, 0.f },     { 0.f, 0.f, -1.f } },
        { { -1.f, -0.707107f, -1.f },   { 0.f, 1.f },   { 0.f, -1.f, 0.f },              { 1.f, 0.f, 0.f },     { 0.f, 0.f, -1.f } },
        { { 1.f, -0.707107f, -1.f },    { 1.f, 1.f },   { 0.f, -1.f, 0.f },              { 1.f, 0.f, 0.f },     { 0.f, 0.f, -1.f } },
        { { -1.f, -0.707107f, 1.f },    { 0.f, 1.f },   { 0.f, 0.447214f, 0.894427f },   { 1.f, 0.f, -0.f },    { 0.f, -0.816497f, 0.57735f } },
        { { 1.f, -0.707107f, 1.f },     { 1.f, 1.f },   { 0.f, 0.447214f, 0.894427f },   { 1.f, 0.f, -0.f },    { 0.f, -0.816497f, 0.57735f } },
        { { 0.f, 0.707107f, 0.f },      { 0.5f, 0.f },  { 0.f, 0.447214f, 0.894427f },   { 1.f, 0.f, -0.f },    { 0.f, -0.816497f, 0.57735f } },
        { { 1.f, -0.707107f, 1.f },     { 0.f, 1.f },   { 0.894427f, 0.447214f, -0.f },  { -0.f, 0.f, -1.f },   { 0.57735f, -0.816497f, -0.f } },
        { { 1.f, -0.707107f, -1.f },    { 1.f, 1.f },   { 0.894427f, 0.447214f, -0.f },  { -0.f, 0.f, -1.f },   { 0.57735f, -0.816497f, -0.f } },
        { { 0.f, 0.707107f, 0.f },      { 0.5f, 0.f },  { 0.894427f, 0.447214f, -0.f },  { -0.f, 0.f, -1.f },   { 0.57735f, -0.816497f, -0.f } },
        { { 1.f, -0.707107f, -1.f },    { 0.f, 1.f },   { -0.f, 0.447214f, -0.894427f }, { -1.f, 0.f, 0.f },    { -0.f, -0.816497f, -0.57735f } },
        { { -1.f, -0.707107f, -1.f },   { 1.f, 1.f },   { -0.f, 0.447214f, -0.894427f }, { -1.f, 0.f, 0.f },    { -0.f, -0.816497f, -0.57735f } },
        { { 0.f, 0.707107f, 0.f },      { 0.5f, 0.f },  { -0.f, 0.447214f, -0.894427f }, { -1.f, 0.f, 0.f },    { -0.f, -0.816497f, -0.57735f } },
        { { -1.f, -0.707107f, -1.f },   { 0.f, 1.f },   { -0.894427f, 0.447214f, 0.f },  { 0.f, 0.f, 1.f },     { -0.57735f, -0.816497f, 0.f } },
        { { -1.f, -0.707107f, 1.f },    { 1.f, 1.f },   { -0.894427f, 0.447214f, 0.f },  { 0.f, 0.f, 1.f },     { -0.57735f, -0.816497f, 0.f } },
        { { 0.f, 0.707107f, 0.f },      { 0.5f, 0.f },  { -0.894427f, 0.447214f, 0.f },  { 0.f, 0.f, 1.f },     { -0.57735f, -0.816497f, 0.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
        0, 2, 1,
        1, 2, 3,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15
    };

    TestablePyramid* pyramid = new TestablePyramid(ValuesRange::ONE_TO_ONE);

    const auto& v = pyramid->getVertices();
    const auto& i = pyramid->getIndices();

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