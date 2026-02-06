#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Hexagon.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestableHexagon : public Hexagon {
public:
    using Hexagon::Hexagon;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Hexagon.Minimal.Valid") {
	ShapeConfig config{};
	TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

	const auto& v = hexagon.getVertices();
	const auto& i = hexagon.getIndices();

	REQUIRE_FALSE(v.empty());
	REQUIRE_FALSE(i.empty());

	for (unsigned idx : i) {
		REQUIRE(idx < v.size());
	}
}

TEST_CASE("ShapesGenerator.Hexagon.NoTangents") {
	ShapeConfig config{};
	config.genTangents = false;

	TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

	for (const auto& v : hexagon.getVertices()) {
		CHECK(v.Tangent == glm::vec3(0.f));
		CHECK(v.Bitangent == glm::vec3(0.f));
	}
}

TEST_CASE("ShapesGenerator.Hexagon.NoBitangents") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = false;

	TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

	for (const auto& v : hexagon.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent == glm::vec3(0.f));
	}
}

TEST_CASE("ShapesGenerator.Hexagon.PositiveHandedness") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

	TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

	for (const auto& v : hexagon.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent != glm::vec3(0.f));

		glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
		CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
	}
}

TEST_CASE("ShapesGenerator.Hexagon.NegativeHandedness") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = false;

	TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

	for (const auto& v : hexagon.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent != glm::vec3(0.f));

		glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
		CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
	}
}

TEST_CASE("ShapesGenerator.Hexagon.Position.Range.OneToOne") {
	ShapeConfig config{};
	TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

	for (const auto& v : hexagon.getVertices()) {
		REQUIRE(v.Position.x >= -1.f);
		REQUIRE(v.Position.x <= 1.f);
		REQUIRE(v.Position.y >= -1.f);
		REQUIRE(v.Position.y <= 1.f);
		REQUIRE(v.Position.z >= -1.f);
		REQUIRE(v.Position.z <= 1.f);
	}
}

TEST_CASE("ShapesGenerator.Hexagon.Position.Range.HalfToHalf") {
	ShapeConfig config{};
	TestableHexagon hexagon(config, 1u, ValuesRange::HALF_TO_HALF);

	for (const auto& v : hexagon.getVertices()) {
		REQUIRE(v.Position.x >= -0.5f);
		REQUIRE(v.Position.x <= 0.5f);
		REQUIRE(v.Position.y >= -0.5f);
		REQUIRE(v.Position.y <= 0.5f);
		REQUIRE(v.Position.z >= -0.5f);
		REQUIRE(v.Position.z <= 0.5f);
	}
}

TEST_CASE("ShapesGenerator.Hexagon.Normals.UnitLength") {
	ShapeConfig config{};
	TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

	for (const auto& v : hexagon.getVertices()) {
		float len = glm::length(v.Normal);
		REQUIRE(len == Catch::Approx(1.f).epsilon(TEST_EPSILON));
	}
}

TEST_CASE("ShapesGenerator.Hexagon.TexCoord.Range") {
	ShapeConfig config{};
	TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

	for (const auto& v : hexagon.getVertices()) {
		REQUIRE(v.TexCoord.x >= 0.f);
		REQUIRE(v.TexCoord.x <= 1.f);
		REQUIRE(v.TexCoord.y >= 0.f);
		REQUIRE(v.TexCoord.y <= 1.f);
	}
}

TEST_CASE("ShapesGenerator.Hexagon.IndexCount") {
	constexpr unsigned h = 2;

	ShapeConfig config{};
	TestableHexagon hexagon(config, h, ValuesRange::ONE_TO_ONE);

	// Hexagon:
	// - top cap:    (6 - 2) triangles
	// - bottom cap: (6 - 2) triangles
	// - side:       h * 6 * 2 triangles
	const unsigned expectedTriangles = 8 + h * 12;

	REQUIRE(hexagon.getIndices().size() == expectedTriangles * 3);
}

TEST_CASE("ShapesGenerator.Hexagon.Generation(H[1].TBP)") {
	static const std::vector<Vertex> expectedVertices = {
		   // POSITION                  // TEX COORD          // NORMAL                    // TANGENT                   // BITANGENT
		{ {        0.f,  1.f,   1.f }, {      0.5f,   1.f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ {  0.866025f,  1.f,  0.5f }, { 0.933013f, 0.75f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ {  0.866025f,  1.f, -0.5f }, { 0.933013f, 0.25f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ {       -0.f,  1.f,  -1.f }, {      0.5f,   0.f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ { -0.866025f,  1.f, -0.5f }, { 0.066987f, 0.25f }, {   0.f,  1.f,        0.f }, {        1.f,  0.f,  -0.f }, { -0.f, 0.f, -1.f } },
		{ { -0.866025f,  1.f,  0.5f }, { 0.066987f, 0.75f }, {   0.f,  1.f,        0.f }, {        1.f,  0.f,  -0.f }, { -0.f, 0.f, -1.f } },
		{ {        0.f,  1.f,   0.f }, {      0.5f,  0.5f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ {        0.f,  1.f,   1.f }, {       0.f,   0.f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,  1.f,  0.5f }, {       1.f,   0.f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,  1.f,  0.5f }, {       0.f,   0.f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,  1.f, -0.5f }, {       1.f,   0.f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,  1.f, -0.5f }, {       0.f,   0.f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,  1.f,  -1.f }, {       1.f,   0.f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,  1.f,  -1.f }, {       0.f,   0.f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,  1.f, -0.5f }, {       1.f,   0.f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,  1.f, -0.5f }, {       0.f,   0.f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,  1.f,  0.5f }, {       1.f,   0.f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,  1.f,  0.5f }, {       0.f,   0.f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f,  1.f,   1.f }, {       1.f,   0.f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f, -1.f,   1.f }, {       0.f,   1.f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f, -1.f,  0.5f }, {       1.f,   1.f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f, -1.f,  0.5f }, {       0.f,   1.f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f, -1.f, -0.5f }, {       1.f,   1.f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f, -1.f, -0.5f }, {       0.f,   1.f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f, -1.f,  -1.f }, {       1.f,   1.f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f, -1.f,  -1.f }, {       0.f,   1.f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f, -1.f, -0.5f }, {       1.f,   1.f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f, -1.f, -0.5f }, {       0.f,   1.f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f, -1.f,  0.5f }, {       1.f,   1.f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f, -1.f,  0.5f }, {       0.f,   1.f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f, -1.f,   1.f }, {       1.f,   1.f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f, -1.f,   1.f }, {      0.5f,   1.f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,   0.f }, { -0.f, 0.f,  1.f } },
		{ {  0.866025f, -1.f,  0.5f }, { 0.933013f, 0.75f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,   0.f }, { -0.f, 0.f,  1.f } },
		{ {  0.866025f, -1.f, -0.5f }, { 0.933013f, 0.25f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,   0.f }, { -0.f, 0.f,  1.f } },
		{ {       -0.f, -1.f,  -1.f }, {      0.5f,   0.f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,   0.f }, { -0.f, 0.f,  1.f } },
		{ { -0.866025f, -1.f, -0.5f }, { 0.066987f, 0.25f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,  -0.f }, {  0.f, 0.f,  1.f } },
		{ { -0.866025f, -1.f,  0.5f }, { 0.066987f, 0.75f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,  -0.f }, {  0.f, 0.f,  1.f } },
		{ {        0.f, -1.f,   0.f }, {      0.5f,  0.5f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,  -0.f }, {  0.f, 0.f,  1.f } }
	};

	static const std::vector<unsigned int> expectedIndices = {
		 0,  1,  6,
		 1,  2,  6,
		 2,  3,  6,
		 3,  4,  6,
		 4,  5,  6,
		 5,  0,  6,
		 7, 19,  8,
		 8, 19, 20,
		 9, 21, 10,
		10, 21, 22,
		11, 23, 12,
		12, 23, 24,
		13, 25, 14,
		14, 25, 26,
		15, 27, 16,
		16, 27, 28,
		17, 29, 18,
		18, 29, 30,
		32, 31, 37,
		33, 32, 37,
		34, 33, 37,
		35, 34, 37,
		36, 35, 37,
		31, 36, 37
	};

	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

    TestableHexagon hexagon(config, 1u, ValuesRange::ONE_TO_ONE);

    const auto& v = hexagon.getVertices();
    const auto& i = hexagon.getIndices();

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

TEST_CASE("ShapesGenerator.Hexagon.Generation(H[4].TBP)") {
	static const std::vector<Vertex> expectedVertices = {
		   // POSITION                   // TEX COORD          // NORMAL                    // TANGENT                   // BITANGENT
		{ {        0.f,   1.f,   1.f }, {      0.5f,   1.f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ {  0.866025f,   1.f,  0.5f }, { 0.933013f, 0.75f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ {  0.866025f,   1.f, -0.5f }, { 0.933013f, 0.25f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ {       -0.f,   1.f,  -1.f }, {      0.5f,   0.f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ { -0.866025f,   1.f, -0.5f }, { 0.066987f, 0.25f }, {   0.f,  1.f,        0.f }, {        1.f,  0.f,  -0.f }, { -0.f, 0.f, -1.f } },
		{ { -0.866025f,   1.f,  0.5f }, { 0.066987f, 0.75f }, {   0.f,  1.f,        0.f }, {        1.f,  0.f,  -0.f }, { -0.f, 0.f, -1.f } },
		{ {        0.f,   1.f,   0.f }, {      0.5f,  0.5f }, {   0.f,  1.f,        0.f }, {        1.f, -0.f,   0.f }, {  0.f, 0.f, -1.f } },
		{ {        0.f,   1.f,   1.f }, {       0.f,   0.f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,   1.f,  0.5f }, {       1.f,   0.f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,   1.f,  0.5f }, {       0.f,   0.f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,   1.f, -0.5f }, {       1.f,   0.f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,   1.f, -0.5f }, {       0.f,   0.f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,   1.f,  -1.f }, {       1.f,   0.f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,   1.f,  -1.f }, {       0.f,   0.f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,   1.f, -0.5f }, {       1.f,   0.f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,   1.f, -0.5f }, {       0.f,   0.f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,   1.f,  0.5f }, {       1.f,   0.f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,   1.f,  0.5f }, {       0.f,   0.f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f,   1.f,   1.f }, {       1.f,   0.f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f,  0.5f,   1.f }, {       0.f, 0.25f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,  0.5f,  0.5f }, {       1.f, 0.25f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,  0.5f,  0.5f }, {       0.f, 0.25f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,  0.5f, -0.5f }, {       1.f, 0.25f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,  0.5f, -0.5f }, {       0.f, 0.25f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,  0.5f,  -1.f }, {       1.f, 0.25f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,  0.5f,  -1.f }, {       0.f, 0.25f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,  0.5f, -0.5f }, {       1.f, 0.25f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,  0.5f, -0.5f }, {       0.f, 0.25f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,  0.5f,  0.5f }, {       1.f, 0.25f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,  0.5f,  0.5f }, {       0.f, 0.25f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f,  0.5f,   1.f }, {       1.f, 0.25f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f,  0.5f,   1.f }, {       0.f, 0.25f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,  0.5f,  0.5f }, {       1.f, 0.25f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,  0.5f,  0.5f }, {       0.f, 0.25f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,  0.5f, -0.5f }, {       1.f, 0.25f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,  0.5f, -0.5f }, {       0.f, 0.25f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,  0.5f,  -1.f }, {       1.f, 0.25f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,  0.5f,  -1.f }, {       0.f, 0.25f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,  0.5f, -0.5f }, {       1.f, 0.25f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,  0.5f, -0.5f }, {       0.f, 0.25f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,  0.5f,  0.5f }, {       1.f, 0.25f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,  0.5f,  0.5f }, {       0.f, 0.25f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f,  0.5f,   1.f }, {       1.f, 0.25f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f,   0.f,   1.f }, {       0.f,  0.5f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,   0.f,  0.5f }, {       1.f,  0.5f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,   0.f,  0.5f }, {       0.f,  0.5f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,   0.f, -0.5f }, {       1.f,  0.5f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,   0.f, -0.5f }, {       0.f,  0.5f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,   0.f,  -1.f }, {       1.f,  0.5f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,   0.f,  -1.f }, {       0.f,  0.5f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,   0.f, -0.5f }, {       1.f,  0.5f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,   0.f, -0.5f }, {       0.f,  0.5f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,   0.f,  0.5f }, {       1.f,  0.5f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,   0.f,  0.5f }, {       0.f,  0.5f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f,   0.f,   1.f }, {       1.f,  0.5f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f,   0.f,   1.f }, {       0.f,  0.5f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,   0.f,  0.5f }, {       1.f,  0.5f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,   0.f,  0.5f }, {       0.f,  0.5f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,   0.f, -0.5f }, {       1.f,  0.5f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,   0.f, -0.5f }, {       0.f,  0.5f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,   0.f,  -1.f }, {       1.f,  0.5f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,   0.f,  -1.f }, {       0.f,  0.5f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,   0.f, -0.5f }, {       1.f,  0.5f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,   0.f, -0.5f }, {       0.f,  0.5f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,   0.f,  0.5f }, {       1.f,  0.5f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,   0.f,  0.5f }, {       0.f,  0.5f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f,   0.f,   1.f }, {       1.f,  0.5f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f, -0.5f,   1.f }, {       0.f, 0.75f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f, -0.5f,  0.5f }, {       1.f, 0.75f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f, -0.5f,  0.5f }, {       0.f, 0.75f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f, -0.5f, -0.5f }, {       1.f, 0.75f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f, -0.5f, -0.5f }, {       0.f, 0.75f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f, -0.5f,  -1.f }, {       1.f, 0.75f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f, -0.5f,  -1.f }, {       0.f, 0.75f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f, -0.5f, -0.5f }, {       1.f, 0.75f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f, -0.5f, -0.5f }, {       0.f, 0.75f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f, -0.5f,  0.5f }, {       1.f, 0.75f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f, -0.5f,  0.5f }, {       0.f, 0.75f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f, -0.5f,   1.f }, {       1.f, 0.75f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f, -0.5f,   1.f }, {       0.f, 0.75f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f, -0.5f,  0.5f }, {       1.f, 0.75f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f, -0.5f,  0.5f }, {       0.f, 0.75f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f, -0.5f, -0.5f }, {       1.f, 0.75f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f, -0.5f, -0.5f }, {       0.f, 0.75f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f, -0.5f,  -1.f }, {       1.f, 0.75f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f, -0.5f,  -1.f }, {       0.f, 0.75f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f, -0.5f, -0.5f }, {       1.f, 0.75f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f, -0.5f, -0.5f }, {       0.f, 0.75f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f, -0.5f,  0.5f }, {       1.f, 0.75f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f, -0.5f,  0.5f }, {       0.f, 0.75f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f, -0.5f,   1.f }, {       1.f, 0.75f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f,  -1.f,   1.f }, {       0.f,   1.f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,  -1.f,  0.5f }, {       1.f,   1.f }, {  0.5f,  0.f,  0.866025f }, {  0.866025f,  0.f, -0.5f }, { -0.f, 1.f,  0.f } },
		{ {  0.866025f,  -1.f,  0.5f }, {       0.f,   1.f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,  -1.f, -0.5f }, {       1.f,   1.f }, {   1.f,  0.f,       -0.f }, {       -0.f,  0.f,  -1.f }, {  0.f, 1.f,  0.f } },
		{ {  0.866025f,  -1.f, -0.5f }, {       0.f,   1.f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,  -1.f,  -1.f }, {       1.f,   1.f }, {  0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f, -0.5f }, {  0.f, 1.f,  0.f } },
		{ {       -0.f,  -1.f,  -1.f }, {       0.f,   1.f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,  -1.f, -0.5f }, {       1.f,   1.f }, { -0.5f,  0.f, -0.866025f }, { -0.866025f,  0.f,  0.5f }, {  0.f, 1.f,  0.f } },
		{ { -0.866025f,  -1.f, -0.5f }, {       0.f,   1.f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,  -1.f,  0.5f }, {       1.f,   1.f }, {  -1.f,  0.f,        0.f }, {        0.f,  0.f,   1.f }, {  0.f, 1.f, -0.f } },
		{ { -0.866025f,  -1.f,  0.5f }, {       0.f,   1.f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {       -0.f,  -1.f,   1.f }, {       1.f,   1.f }, { -0.5f,  0.f,  0.866025f }, {  0.866026f,  0.f,  0.5f }, {  0.f, 1.f, -0.f } },
		{ {        0.f,  -1.f,   1.f }, {      0.5f,   1.f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,   0.f }, { -0.f, 0.f,  1.f } },
		{ {  0.866025f,  -1.f,  0.5f }, { 0.933013f, 0.75f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,   0.f }, { -0.f, 0.f,  1.f } },
		{ {  0.866025f,  -1.f, -0.5f }, { 0.933013f, 0.25f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,   0.f }, { -0.f, 0.f,  1.f } },
		{ {       -0.f,  -1.f,  -1.f }, {      0.5f,   0.f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,   0.f }, { -0.f, 0.f,  1.f } },
		{ { -0.866025f,  -1.f, -0.5f }, { 0.066987f, 0.25f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,  -0.f }, {  0.f, 0.f,  1.f } },
		{ { -0.866025f,  -1.f,  0.5f }, { 0.066987f, 0.75f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,  -0.f }, {  0.f, 0.f,  1.f } },
		{ {        0.f,  -1.f,   0.f }, {      0.5f,  0.5f }, {   0.f, -1.f,        0.f }, {        1.f,  0.f,  -0.f }, {  0.f, 0.f,  1.f } }
	};

	static const std::vector<unsigned int> expectedIndices = {
		  0,   1,   6,
		  1,   2,   6,
		  2,   3,   6,
		  3,   4,   6,
		  4,   5,   6,
		  5,   0,   6,
		  7,  19,   8,
		  8,  19,  20,
		  9,  21,  10,
		 10,  21,  22,
		 11,  23,  12,
		 12,  23,  24,
		 13,  25,  14,
		 14,  25,  26,
		 15,  27,  16,
		 16,  27,  28,
		 17,  29,  18,
		 18,  29,  30,
		 31,  43,  32,
		 32,  43,  44,
		 33,  45,  34,
		 34,  45,  46,
		 35,  47,  36,
		 36,  47,  48,
		 37,  49,  38,
		 38,  49,  50,
		 39,  51,  40,
		 40,  51,  52,
		 41,  53,  42,
		 42,  53,  54,
		 55,  67,  56,
		 56,  67,  68,
		 57,  69,  58,
		 58,  69,  70,
		 59,  71,  60,
		 60,  71,  72,
		 61,  73,  62,
		 62,  73,  74,
		 63,  75,  64,
		 64,  75,  76,
		 65,  77,  66,
		 66,  77,  78,
		 79,  91,  80,
		 80,  91,  92,
		 81,  93,  82,
		 82,  93,  94,
		 83,  95,  84,
		 84,  95,  96,
		 85,  97,  86,
		 86,  97,  98,
		 87,  99,  88,
		 88,  99, 100,
		 89, 101,  90,
		 90, 101, 102,
		104, 103, 109,
		105, 104, 109,
		106, 105, 109,
		107, 106, 109,
		108, 107, 109,
		103, 108, 109
	};

	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

	TestableHexagon hexagon(config, 4u, ValuesRange::ONE_TO_ONE);

	const auto& v = hexagon.getVertices();
	const auto& i = hexagon.getIndices();

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