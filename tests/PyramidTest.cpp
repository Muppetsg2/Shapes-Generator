#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Pyramid.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestablePyramid : public Pyramid {
public:
    using Pyramid::Pyramid;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Pyramid.Minimal.Valid") {
	ShapeConfig config{};
	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	const auto& v = pyramid.getVertices();
	const auto& i = pyramid.getIndices();

	REQUIRE_FALSE(v.empty());
	REQUIRE_FALSE(i.empty());

	for (unsigned idx : i) {
		REQUIRE(idx < v.size());
	}
}

TEST_CASE("ShapesGenerator.Pyramid.NoTangents") {
	ShapeConfig config{};
	config.genTangents = false;

	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		CHECK(v.Tangent == glm::vec3(0.f));
		CHECK(v.Bitangent == glm::vec3(0.f));
	}
}

TEST_CASE("ShapesGenerator.Pyramid.NoBitangents") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = false;

	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent == glm::vec3(0.f));
	}
}

TEST_CASE("ShapesGenerator.Pyramid.PositiveHandedness") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent != glm::vec3(0.f));

		glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
		CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
	}
}

TEST_CASE("ShapesGenerator.Pyramid.NegativeHandedness") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = false;

	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent != glm::vec3(0.f));

		glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
		CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
	}
}

TEST_CASE("ShapesGenerator.Pyramid.Position.Range.OneToOne") {
	ShapeConfig config{};
	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		REQUIRE(v.Position.x >= -1.f);
		REQUIRE(v.Position.x <= 1.f);
		REQUIRE(v.Position.y >= -1.f);
		REQUIRE(v.Position.y <= 1.f);
		REQUIRE(v.Position.z >= -1.f);
		REQUIRE(v.Position.z <= 1.f);
	}
}

TEST_CASE("ShapesGenerator.Pyramid.Position.Range.HalfToHalf") {
	ShapeConfig config{};
	TestablePyramid pyramid(config, ValuesRange::HALF_TO_HALF);

	for (const auto& v : pyramid.getVertices()) {
		REQUIRE(v.Position.x >= -0.5f);
		REQUIRE(v.Position.x <= 0.5f);
		REQUIRE(v.Position.y >= -0.5f);
		REQUIRE(v.Position.y <= 0.5f);
		REQUIRE(v.Position.z >= -0.5f);
		REQUIRE(v.Position.z <= 0.5f);
	}
}

TEST_CASE("ShapesGenerator.Pyramid.Normals.UnitLength") {
	ShapeConfig config{};
	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		float len = glm::length(v.Normal);
		REQUIRE(len == Catch::Approx(1.f).epsilon(TEST_EPSILON));
	}
}

TEST_CASE("ShapesGenerator.Pyramid.Tangents.UnitLength") {
	ShapeConfig config{};
	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		REQUIRE(glm::length(v.Tangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
	}
}

TEST_CASE("ShapesGenerator.Pyramid.Bitangents.UnitLength") {
	ShapeConfig config{};
	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		REQUIRE(glm::length(v.Bitangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
	}
}

TEST_CASE("ShapesGenerator.Pyramid.TexCoord.Range") {
	ShapeConfig config{};
	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	for (const auto& v : pyramid.getVertices()) {
		REQUIRE(v.TexCoord.x >= 0.f);
		REQUIRE(v.TexCoord.x <= 1.f);
		REQUIRE(v.TexCoord.y >= 0.f);
		REQUIRE(v.TexCoord.y <= 1.f);
	}
}

TEST_CASE("ShapesGenerator.Pyramid.IndexCount") {
	constexpr unsigned h = 2;

	ShapeConfig config{};
	TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

	// Pyramid (square base):
	// - bottom cap: 2 triangles
	// - side:       4 triangles
	const unsigned expectedTriangles = 6;

	REQUIRE(pyramid.getIndices().size() == expectedTriangles * 3);
}

TEST_CASE("ShapesGenerator.Pyramid.Generation(TBP)") {
	static const std::vector<Vertex> expectedVertices = {
		   // POSITION                 // TEX COORD   // NORMAL                              // TANGENT           // BITANGENT
		{ { -1.f, -0.707107f,  1.f }, {  0.f, 0.f }, {        0.f,      -1.f,        0.f }, {  1.f, 0.f,  0.f }, {       -0.f,       0.f,        1.f } },
		{ {  1.f, -0.707107f,  1.f }, {  1.f, 0.f }, {        0.f,      -1.f,        0.f }, {  1.f, 0.f,  0.f }, {       -0.f,       0.f,        1.f } },
		{ { -1.f, -0.707107f, -1.f }, {  0.f, 1.f }, {        0.f,      -1.f,        0.f }, {  1.f, 0.f,  0.f }, {       -0.f,       0.f,        1.f } },
		{ {  1.f, -0.707107f, -1.f }, {  1.f, 1.f }, {        0.f,      -1.f,        0.f }, {  1.f, 0.f,  0.f }, {       -0.f,       0.f,        1.f } },
		{ { -1.f, -0.707107f,  1.f }, {  0.f, 1.f }, {        0.f, 0.447214f,  0.894427f }, {  1.f, 0.f, -0.f }, {       -0.f, 0.894427f, -0.447214f } },
		{ {  1.f, -0.707107f,  1.f }, {  1.f, 1.f }, {        0.f, 0.447214f,  0.894427f }, {  1.f, 0.f, -0.f }, {       -0.f, 0.894427f, -0.447214f } },
		{ {  0.f,  0.707107f,  0.f }, { 0.5f, 0.f }, {        0.f, 0.447214f,  0.894427f }, {  1.f, 0.f, -0.f }, {       -0.f, 0.894427f, -0.447214f } },
		{ {  1.f, -0.707107f,  1.f }, {  0.f, 1.f }, {  0.894427f, 0.447214f,       -0.f }, { -0.f, 0.f, -1.f }, { -0.447214f, 0.894427f,        0.f } },
		{ {  1.f, -0.707107f, -1.f }, {  1.f, 1.f }, {  0.894427f, 0.447214f,       -0.f }, { -0.f, 0.f, -1.f }, { -0.447214f, 0.894427f,        0.f } },
		{ {  0.f,  0.707107f,  0.f }, { 0.5f, 0.f }, {  0.894427f, 0.447214f,       -0.f }, { -0.f, 0.f, -1.f }, { -0.447214f, 0.894427f,        0.f } },
		{ {  1.f, -0.707107f, -1.f }, {  0.f, 1.f }, {       -0.f, 0.447214f, -0.894427f }, { -1.f, 0.f,  0.f }, {        0.f, 0.894427f,  0.447214f } },
		{ { -1.f, -0.707107f, -1.f }, {  1.f, 1.f }, {       -0.f, 0.447214f, -0.894427f }, { -1.f, 0.f,  0.f }, {        0.f, 0.894427f,  0.447214f } },
		{ {  0.f,  0.707107f,  0.f }, { 0.5f, 0.f }, {       -0.f, 0.447214f, -0.894427f }, { -1.f, 0.f,  0.f }, {        0.f, 0.894427f,  0.447214f } },
		{ { -1.f, -0.707107f, -1.f }, {  0.f, 1.f }, { -0.894427f, 0.447214f,        0.f }, {  0.f, 0.f,  1.f }, {  0.447214f, 0.894427f,       -0.f } },
		{ { -1.f, -0.707107f,  1.f }, {  1.f, 1.f }, { -0.894427f, 0.447214f,        0.f }, {  0.f, 0.f,  1.f }, {  0.447214f, 0.894427f,       -0.f } },
		{ {  0.f,  0.707107f,  0.f }, { 0.5f, 0.f }, { -0.894427f, 0.447214f,        0.f }, {  0.f, 0.f,  1.f }, {  0.447214f, 0.894427f,       -0.f } }
	};

	static const std::vector<unsigned int> expectedIndices = {
		 0,  2,  1,
		 1,  2,  3,
		 4,  5,  6,
		 7,  8,  9,
		10, 11, 12,
		13, 14, 15
	};

	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

    TestablePyramid pyramid(config, ValuesRange::ONE_TO_ONE);

    const auto& v = pyramid.getVertices();
    const auto& i = pyramid.getIndices();

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