#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Plane.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>

class TestablePlane : public Plane {
public:
    using Plane::Plane;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Plane.Generation(2u2uUP)") {
    static const std::vector<Vertex> expectedVertices = {
        //POSITION				//TEX COORD	    //NORMAL		    //TANGENT			//BITANGENT
        { { -1.f, 0.f, -1.f },  { 0.f, 0.f },   { 0.f, 1.f, 0.f },  { 1.f, 0.f, 0.f },  { 0.f, 0.f, 1.f } },
        { { 1.f, 0.f, -1.f },   { 1.f, 0.f },   { 0.f, 1.f, 0.f },  { 1.f, 0.f, 0.f },  { 0.f, 0.f, 1.f } },
        { { -1.f, 0.f, 1.f },   { 0.f, 1.f },   { 0.f, 1.f, 0.f },  { 1.f, 0.f, 0.f },  { 0.f, 0.f, 1.f } },
        { { 1.f, 0.f, 1.f },    { 1.f, 1.f },   { 0.f, 1.f, 0.f },  { 1.f, 0.f, 0.f },  { 0.f, 0.f, 1.f } }
    };

    static const std::vector<unsigned int> expectedIndices = {
        2, 1, 0,
        2, 3, 1
    };

    TestablePlane* plane = new TestablePlane(2u, 2u, PlaneNormalDir::UP, ValuesRange::ONE_TO_ONE);

    const auto& v = plane->getVertices();
    const auto& i = plane->getIndices();

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

TEST_CASE("ShapesGenerator.Plane.Generation(3u3uFRONT)") {
    static const std::vector<Vertex> expectedVertices = {
        //POSITION				//TEX COORD	    //NORMAL		    //TANGENT			//BITANGENT
        { { -1.f, -1.f, 0.f },  { 0.f, 0.f },   { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } },
        { { 0.f, -1.f, 0.f },   { 0.5f, 0.f },  { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } },
        { { 1.f, -1.f, 0.f },   { 1.f, 0.f },   { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } },
        { { -1.f, 0.f, 0.f },   { 0.f, 0.5f },  { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } },
        { { 0.f, 0.f, 0.f },    { 0.5f, 0.5f }, { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } },
        { { 1.f, 0.f, 0.f },    { 1.f, 0.5f },  { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } },
        { { -1.f, 1.f, 0.f },   { 0.f, 1.f },   { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } },
        { { 0.f, 1.f, 0.f },    { 0.5f, 1.f },  { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } },
        { { 1.f, 1.f, 0.f },    { 1.f, 1.f },   { 0.f, 0.f, -1.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f } }
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

    TestablePlane* plane = new TestablePlane(3u, 3u, PlaneNormalDir::FRONT, ValuesRange::ONE_TO_ONE);

    const auto& v = plane->getVertices();
    const auto& i = plane->getIndices();

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