#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Torus.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestableTorus : public Torus {
public:
	using Torus::Torus;
	const std::vector<Vertex>& getVertices() const { return _vertices; }
	const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Torus.Minimal.Valid") {
	ShapeConfig config{};
	TestableTorus torus(config, 5u, 5u, 1.0f, 0.2f, TorusShading::FLAT, ValuesRange::ONE_TO_ONE);

	const auto& v = torus.getVertices();
	const auto& i = torus.getIndices();

	REQUIRE_FALSE(v.empty());
	REQUIRE_FALSE(i.empty());

	for (unsigned idx : i) {
		REQUIRE(idx < v.size());
	}
}

TEST_CASE("ShapesGenerator.Torus.NoTangents") {
	ShapeConfig config{};
	config.genTangents = false;

	TestableTorus torus(config, 5u, 10u, 0.9f, 0.2f, TorusShading::SMOOTH, ValuesRange::ONE_TO_ONE);

	for (const auto& v : torus.getVertices()) {
		CHECK(v.Tangent == glm::vec3(0.f));
		CHECK(v.Bitangent == glm::vec3(0.f));
	}
}

TEST_CASE("ShapesGenerator.Torus.NoBitangents") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = false;

	TestableTorus torus(config, 5u, 10u, 0.9f, 0.2f, TorusShading::SMOOTH, ValuesRange::ONE_TO_ONE);

	for (const auto& v : torus.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent == glm::vec3(0.f));
	}
}

TEST_CASE("ShapesGenerator.Torus.PositiveHandedness") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

	TestableTorus torus(config, 3u, 8u, 0.9f, 0.2f, TorusShading::SMOOTH, ValuesRange::ONE_TO_ONE);

	for (const auto& v : torus.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent != glm::vec3(0.f));

		glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
		CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
	}
}

TEST_CASE("ShapesGenerator.Torus.NegativeHandedness") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = false;

	TestableTorus torus(config, 3u, 8u, 0.9f, 0.2f, TorusShading::SMOOTH, ValuesRange::ONE_TO_ONE);

	for (const auto& v : torus.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent != glm::vec3(0.f));

		glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
		CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
	}
}

TEST_CASE("ShapesGenerator.Torus.Position.Range.OneToOne") {
	ShapeConfig config{};
	TestableTorus torus(config, 5u, 16u, 1.2f, 0.6f, TorusShading::SMOOTH, ValuesRange::ONE_TO_ONE);

	for (const auto& v : torus.getVertices()) {
		REQUIRE(v.Position.x >= -1.f);
		REQUIRE(v.Position.x <= 1.f);
		REQUIRE(v.Position.y >= -1.f);
		REQUIRE(v.Position.y <= 1.f);
		REQUIRE(v.Position.z >= -1.f);
		REQUIRE(v.Position.z <= 1.f);
	}
}

TEST_CASE("ShapesGenerator.Torus.Position.Range.HalfToHalf") {
	ShapeConfig config{};
	TestableTorus torus(config, 5u, 16u, 1.2f, 0.6f, TorusShading::SMOOTH, ValuesRange::HALF_TO_HALF);

	for (const auto& v : torus.getVertices()) {
		REQUIRE(v.Position.x >= -0.5f);
		REQUIRE(v.Position.x <= 0.5f);
		REQUIRE(v.Position.y >= -0.5f);
		REQUIRE(v.Position.y <= 0.5f);
		REQUIRE(v.Position.z >= -0.5f);
		REQUIRE(v.Position.z <= 0.5f);
	}
}

TEST_CASE("ShapesGenerator.Torus.Normals.UnitLength") {
	ShapeConfig config{};
	TestableTorus torus(config, 5u, 16u, 1.2f, 0.6f, TorusShading::SMOOTH, ValuesRange::ONE_TO_ONE);

	for (const auto& v : torus.getVertices()) {
		float len = glm::length(v.Normal);
		REQUIRE(len == Catch::Approx(1.f).epsilon(TEST_EPSILON));
	}
}

TEST_CASE("ShapesGenerator.Torus.TexCoord.Range") {
	ShapeConfig config{};
	TestableTorus torus(config, 8u, 8u, 1.2f, 0.6f, TorusShading::SMOOTH, ValuesRange::HALF_TO_HALF);

	for (const auto& v : torus.getVertices()) {
		REQUIRE(v.TexCoord.x >= 0.f);
		REQUIRE(v.TexCoord.x <= 1.f);
		REQUIRE(v.TexCoord.y >= 0.f);
		REQUIRE(v.TexCoord.y <= 1.f);
	}
}

TEST_CASE("ShapesGenerator.Torus.IndexCount") {
	constexpr unsigned h = 4;
	constexpr unsigned r = 12;

	ShapeConfig config{};
	TestableTorus torus(
		config,
		h,
		r,
		1.2f,
		0.6f,
		TorusShading::SMOOTH,
		ValuesRange::HALF_TO_HALF
	);

	// torus:
	// - surface only (no caps)
	// - each quad -> 2 triangles
	// - quads: h * r
	const unsigned expectedTriangles = h * r * 2;

	REQUIRE(torus.getIndices().size() == expectedTriangles * 3);
}

TEST_CASE("ShapesGenerator.Torus.Generation(MS[3]IS[3]MR[1.0]IR[0.2].FLAT.TBP)") {
	static const std::vector<Vertex> expectedVertices = {
		   // POSITION						    // TEX COORD              // NORMAL                               // TANGENT                       // BITANGENT
		{ {   0.75f,  0.144338f,        0.f }, {       0.f, 0.333333f }, {       -0.f,  0.707107f,  0.707107f }, { -0.866025f, -0.25f,   0.25f }, {  0.377964f, -0.654654f,  0.654654f } },
		{ {   -0.5f,        0.f,  0.866025f }, { 0.333333f,       1.f }, {       -0.f,  0.707107f,  0.707107f }, { -0.866025f, -0.25f,   0.25f }, {  0.377964f, -0.654654f,  0.654654f } },
		{ {     1.f,        0.f,        0.f }, {       0.f,       1.f }, {       -0.f,  0.707107f,  0.707107f }, { -0.866025f, -0.25f,   0.25f }, {  0.377964f, -0.654654f,  0.654654f } },
		{ {   -0.5f,        0.f,  0.866025f }, { 0.333333f,       1.f }, { -0.387298f,  0.894427f,  0.223607f }, {  -0.69282f,  -0.4f,    0.4f }, {       0.5f,        0.f,  0.866025f } },
		{ {   0.75f,  0.144338f,        0.f }, {       0.f, 0.333333f }, { -0.387298f,  0.894427f,  0.223607f }, {  -0.69282f,  -0.4f,    0.4f }, {       0.5f,        0.f,  0.866025f } },
		{ { -0.375f,  0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, { -0.387298f,  0.894427f,  0.223607f }, {  -0.69282f,  -0.4f,    0.4f }, {       0.5f,        0.f,  0.866025f } },
		{ { -0.375f,  0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, { -0.612372f,  0.707107f, -0.353554f }, {  0.216507f, -0.25f, -0.875f }, { -0.755929f, -0.654654f,       -0.f } },
		{ {   -0.5f,        0.f, -0.866025f }, { 0.666667f,       1.f }, { -0.612372f,  0.707107f, -0.353554f }, {  0.216507f, -0.25f, -0.875f }, { -0.755929f, -0.654654f,       -0.f } },
		{ {   -0.5f,        0.f,  0.866025f }, { 0.333333f,       1.f }, { -0.612372f,  0.707107f, -0.353554f }, {  0.216507f, -0.25f, -0.875f }, { -0.755929f, -0.654654f,       -0.f } },
		{ {   -0.5f,        0.f, -0.866025f }, { 0.666667f,       1.f }, {        0.f,  0.894427f, -0.447214f }, {        0.f,  -0.4f,   -0.8f }, {       -1.f,        0.f,       -0.f } },
		{ { -0.375f,  0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {        0.f,  0.894427f, -0.447214f }, {        0.f,  -0.4f,   -0.8f }, {       -1.f,        0.f,       -0.f } },
		{ { -0.375f,  0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {        0.f,  0.894427f, -0.447214f }, {        0.f,  -0.4f,   -0.8f }, {       -1.f,        0.f,       -0.f } },
		{ { -0.375f,  0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {  0.612373f,  0.707107f, -0.353553f }, {  0.649519f, -0.25f,  0.625f }, {  0.377965f, -0.654654f, -0.654654f } },
		{ {     1.f,        0.f,        0.f }, {       1.f,       1.f }, {  0.612373f,  0.707107f, -0.353553f }, {  0.649519f, -0.25f,  0.625f }, {  0.377965f, -0.654654f, -0.654654f } },
		{ {   -0.5f,        0.f, -0.866025f }, { 0.666667f,       1.f }, {  0.612373f,  0.707107f, -0.353553f }, {  0.649519f, -0.25f,  0.625f }, {  0.377965f, -0.654654f, -0.654654f } },
		{ {     1.f,        0.f,        0.f }, {       1.f,       1.f }, {  0.387298f,  0.894427f,  0.223607f }, {   0.69282f,  -0.4f,    0.4f }, {       0.5f,        0.f, -0.866025f } },
		{ { -0.375f,  0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {  0.387298f,  0.894427f,  0.223607f }, {   0.69282f,  -0.4f,    0.4f }, {       0.5f,        0.f, -0.866025f } },
		{ {   0.75f,  0.144338f,        0.f }, {       1.f, 0.333333f }, {  0.387298f,  0.894427f,  0.223607f }, {   0.69282f,  -0.4f,    0.4f }, {       0.5f,        0.f, -0.866025f } },
		{ {   0.75f, -0.144338f,        0.f }, {       0.f, 0.333333f }, { -0.612372f,  0.707107f, -0.353554f }, {        0.f,    0.f,    -0.f }, { -0.377964f, -0.654654f, -0.654654f } },
		{ { -0.375f,  0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, { -0.612372f,  0.707107f, -0.353554f }, {        0.f,    0.f,    -0.f }, { -0.377964f, -0.654654f, -0.654654f } },
		{ {   0.75f,  0.144338f,        0.f }, {       0.f, 0.333333f }, { -0.612372f,  0.707107f, -0.353554f }, {        0.f,    0.f,    -0.f }, { -0.377964f, -0.654654f, -0.654654f } },
		{ { -0.375f,  0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {        0.f, -0.707107f, -0.707107f }, {        0.f,    0.f,    -0.f }, {  0.377964f, -0.654654f,  0.654654f } },
		{ {   0.75f, -0.144338f,        0.f }, {       0.f, 0.333333f }, {        0.f, -0.707107f, -0.707107f }, {        0.f,    0.f,    -0.f }, {  0.377964f, -0.654654f,  0.654654f } },
		{ { -0.375f, -0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {        0.f, -0.707107f, -0.707107f }, {        0.f,    0.f,    -0.f }, {  0.377964f, -0.654654f,  0.654654f } },
		{ { -0.375f, -0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {  0.612373f,  0.707107f, -0.353553f }, {       -0.f,    0.f,     0.f }, {  0.755929f, -0.654654f,        0.f } },
		{ { -0.375f,  0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {  0.612373f,  0.707107f, -0.353553f }, {       -0.f,    0.f,     0.f }, {  0.755929f, -0.654654f,        0.f } },
		{ { -0.375f,  0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {  0.612373f,  0.707107f, -0.353553f }, {       -0.f,    0.f,     0.f }, {  0.755929f, -0.654654f,        0.f } },
		{ { -0.375f,  0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {  0.612372f, -0.707107f,  0.353554f }, {       -0.f,    0.f,     0.f }, { -0.755929f, -0.654654f,       -0.f } },
		{ { -0.375f, -0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {  0.612372f, -0.707107f,  0.353554f }, {       -0.f,    0.f,     0.f }, { -0.755929f, -0.654654f,       -0.f } },
		{ { -0.375f, -0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {  0.612372f, -0.707107f,  0.353554f }, {       -0.f,    0.f,     0.f }, { -0.755929f, -0.654654f,       -0.f } },
		{ { -0.375f, -0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {       -0.f,  0.707107f,  0.707107f }, {       -0.f,    0.f,    -0.f }, { -0.377965f, -0.654654f,  0.654654f } },
		{ {   0.75f,  0.144338f,        0.f }, {       1.f, 0.333333f }, {       -0.f,  0.707107f,  0.707107f }, {       -0.f,    0.f,    -0.f }, { -0.377965f, -0.654654f,  0.654654f } },
		{ { -0.375f,  0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {       -0.f,  0.707107f,  0.707107f }, {       -0.f,    0.f,    -0.f }, { -0.377965f, -0.654654f,  0.654654f } },
		{ {   0.75f,  0.144338f,        0.f }, {       1.f, 0.333333f }, { -0.612372f, -0.707107f,  0.353553f }, {       -0.f,    0.f,    -0.f }, {  0.377965f, -0.654653f, -0.654654f } },
		{ { -0.375f, -0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, { -0.612372f, -0.707107f,  0.353553f }, {       -0.f,    0.f,    -0.f }, {  0.377965f, -0.654653f, -0.654654f } },
		{ {   0.75f, -0.144338f,        0.f }, {       1.f, 0.333333f }, { -0.612372f, -0.707107f,  0.353553f }, {       -0.f,    0.f,    -0.f }, {  0.377965f, -0.654653f, -0.654654f } },
		{ {     1.f,        0.f,        0.f }, {       0.f,       1.f }, {  0.387298f, -0.894427f, -0.223607f }, {  -0.69282f,  -0.4f,    0.4f }, {      -0.5f,       -0.f, -0.866026f } },
		{ { -0.375f, -0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {  0.387298f, -0.894427f, -0.223607f }, {  -0.69282f,  -0.4f,    0.4f }, {      -0.5f,       -0.f, -0.866026f } },
		{ {   0.75f, -0.144338f,        0.f }, {       0.f, 0.333333f }, {  0.387298f, -0.894427f, -0.223607f }, {  -0.69282f,  -0.4f,    0.4f }, {      -0.5f,       -0.f, -0.866026f } },
		{ { -0.375f, -0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {  0.612372f, -0.707107f,  0.353554f }, { -0.649519f, -0.25f,  0.625f }, { -0.377964f, -0.654654f, -0.654654f } },
		{ {     1.f,        0.f,        0.f }, {       0.f,       1.f }, {  0.612372f, -0.707107f,  0.353554f }, { -0.649519f, -0.25f,  0.625f }, { -0.377964f, -0.654654f, -0.654654f } },
		{ {   -0.5f,        0.f,  0.866025f }, { 0.333333f,       1.f }, {  0.612372f, -0.707107f,  0.353554f }, { -0.649519f, -0.25f,  0.625f }, { -0.377964f, -0.654654f, -0.654654f } },
		{ {   -0.5f,        0.f,  0.866025f }, { 0.333333f,       1.f }, {       -0.f, -0.894427f,  0.447214f }, {        0.f,  -0.4f,   -0.8f }, {        1.f,       -0.f,        0.f } },
		{ { -0.375f, -0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, {       -0.f, -0.894427f,  0.447214f }, {        0.f,  -0.4f,   -0.8f }, {        1.f,       -0.f,        0.f } },
		{ { -0.375f, -0.144338f,  0.649519f }, { 0.333333f, 0.333333f }, {       -0.f, -0.894427f,  0.447214f }, {        0.f,  -0.4f,   -0.8f }, {        1.f,       -0.f,        0.f } },
		{ { -0.375f, -0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, { -0.612373f, -0.707107f,  0.353553f }, { -0.216506f, -0.25f, -0.875f }, {  0.755929f, -0.654654f,        0.f } },
		{ {   -0.5f,        0.f,  0.866025f }, { 0.333333f,       1.f }, { -0.612373f, -0.707107f,  0.353553f }, { -0.216506f, -0.25f, -0.875f }, {  0.755929f, -0.654654f,        0.f } },
		{ {   -0.5f,        0.f, -0.866025f }, { 0.666667f,       1.f }, { -0.612373f, -0.707107f,  0.353553f }, { -0.216506f, -0.25f, -0.875f }, {  0.755929f, -0.654654f,        0.f } },
		{ {   -0.5f,        0.f, -0.866025f }, { 0.666667f,       1.f }, { -0.387298f, -0.894427f, -0.223607f }, {   0.69282f,  -0.4f,    0.4f }, {      -0.5f,       -0.f,  0.866025f } },
		{ {   0.75f, -0.144338f,        0.f }, {       1.f, 0.333333f }, { -0.387298f, -0.894427f, -0.223607f }, {   0.69282f,  -0.4f,    0.4f }, {      -0.5f,       -0.f,  0.866025f } },
		{ { -0.375f, -0.144338f, -0.649519f }, { 0.666667f, 0.333333f }, { -0.387298f, -0.894427f, -0.223607f }, {   0.69282f,  -0.4f,    0.4f }, {      -0.5f,       -0.f,  0.866025f } },
		{ {   0.75f, -0.144338f,        0.f }, {       1.f, 0.333333f }, {        0.f, -0.707107f, -0.707107f }, {  0.866025f, -0.25f,   0.25f }, { -0.377965f, -0.654654f,  0.654653f } },
		{ {   -0.5f,        0.f, -0.866025f }, { 0.666667f,       1.f }, {        0.f, -0.707107f, -0.707107f }, {  0.866025f, -0.25f,   0.25f }, { -0.377965f, -0.654654f,  0.654653f } },
		{ {     1.f,        0.f,        0.f }, {       1.f,       1.f }, {        0.f, -0.707107f, -0.707107f }, {  0.866025f, -0.25f,   0.25f }, { -0.377965f, -0.654654f,  0.654653f } }
	};

	static const std::vector<unsigned int> expectedIndices = {
		 0,  1,  2,
		 3,  4,  5,
		 6,  7,  8,
		 9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35,
		36, 37, 38,
		39, 40, 41,
		42, 43, 44,
		45, 46, 47,
		48, 49, 50,
		51, 52, 53
	};

	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

    TestableTorus torus(config, 3u, 3u, 1.0f, 0.2f, TorusShading::FLAT, ValuesRange::ONE_TO_ONE);

    const auto& v = torus.getVertices();
    const auto& i = torus.getIndices();

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

TEST_CASE("ShapesGenerator.Torus.Generation(MS[5]IS[5]MR[1.2]IR[1.0].SMOOTH.TBP)") {
	static const std::vector<Vertex> expectedVertices = {
		   // POSITION                             // TEX COORD    // NORMAL                               // TANGENT                              // BITANGENT
		{ {        1.f,        0.f,        0.f }, {  0.f,  1.f }, {        1.f,        0.f,        0.f }, {       -0.f,        0.f,  0.809017f }, {        0.f,       -1.f,        0.f } },
		{ {  0.309017f,        0.f,  0.951056f }, { 0.2f,  1.f }, {  0.309017f,        0.f,  0.951057f }, { -0.924336f,        0.f,  0.300335f }, {        0.f,       -1.f,        0.f } },
		{ { -0.809017f,        0.f,  0.587785f }, { 0.4f,  1.f }, { -0.809017f,        0.f,  0.587785f }, { -0.571271f,        0.f, -0.786287f }, {       -0.f,       -1.f,        0.f } },
		{ { -0.809017f,        0.f, -0.587785f }, { 0.6f,  1.f }, { -0.809017f,        0.f, -0.587785f }, {  0.571271f,        0.f, -0.786287f }, {        0.f,       -1.f,       -0.f } },
		{ {  0.309017f,        0.f, -0.951056f }, { 0.8f,  1.f }, {  0.309017f,        0.f, -0.951056f }, {  0.924336f,        0.f,  0.300335f }, {        0.f,       -1.f,        0.f } },
		{ {        1.f,        0.f,        0.f }, {  1.f,  1.f }, {        1.f,        0.f,        0.f }, {       -0.f,        0.f,  0.809017f }, {        0.f,       -1.f,        0.f } },
		{ {  0.685917f,  0.432298f,        0.f }, {  0.f, 0.6f }, {  0.309017f,  0.951057f,        0.f }, { -0.531657f,  0.172746f,  0.809017f }, {  0.782436f, -0.254229f,  0.568473f } },
		{ {   0.21196f,  0.432298f,  0.652346f }, { 0.2f, 0.6f }, {  0.095491f,  0.951057f,  0.293893f }, { -0.951057f,       -0.f,  0.309017f }, {  0.293893f, -0.309017f,  0.904509f } },
		{ { -0.554918f,  0.432298f,  0.403172f }, { 0.4f, 0.6f }, {     -0.25f,  0.951057f,  0.181636f }, { -0.587785f,        0.f, -0.809017f }, { -0.769421f, -0.309017f,  0.559017f } },
		{ { -0.554918f,  0.432298f, -0.403172f }, { 0.6f, 0.6f }, {     -0.25f,  0.951057f, -0.181636f }, {  0.587785f,        0.f, -0.809017f }, { -0.769421f, -0.309017f, -0.559017f } },
		{ {   0.21196f,  0.432298f, -0.652346f }, { 0.8f, 0.6f }, {  0.095492f,  0.951057f, -0.293893f }, {  0.951056f,        0.f,  0.309017f }, {  0.293893f, -0.309017f, -0.904508f } },
		{ {  0.685917f,  0.432298f,        0.f }, {  1.f, 0.6f }, {  0.309017f,  0.951057f,        0.f }, {  0.531657f, -0.172746f,  0.809017f }, {  0.782436f, -0.254229f, -0.568473f } },
		{ {  0.177719f,  0.267175f,        0.f }, {  0.f, 0.2f }, { -0.809017f,  0.587785f,        0.f }, { -0.203075f, -0.279509f,  0.809017f }, {  0.540557f,  0.744013f,  0.392738f } },
		{ {  0.054918f,  0.267175f,  0.169021f }, { 0.2f, 0.2f }, {     -0.25f,  0.587785f, -0.769421f }, { -0.949465f, -0.111928f,  0.222994f }, {  0.045791f,  0.800943f,  0.596987f } },
		{ { -0.143778f,  0.267175f,  0.104461f }, { 0.4f, 0.2f }, {  0.654509f,  0.587785f, -0.475528f }, { -0.505481f, -0.111928f, -0.834086f }, { -0.553619f,  0.800943f,  0.228029f } },
		{ { -0.143778f,  0.267175f, -0.104461f }, { 0.6f, 0.2f }, {  0.654508f,  0.587785f,  0.475528f }, {  0.637061f, -0.111928f, -0.738488f }, { -0.387946f,  0.800943f, -0.456058f } },
		{ {  0.054918f,  0.267175f, -0.169021f }, { 0.8f, 0.2f }, {     -0.25f,  0.587785f,  0.769421f }, {  0.899206f, -0.111928f,  0.377676f }, {  0.313855f,  0.800943f, -0.509888f } },
		{ {  0.177719f,  0.267175f,        0.f }, {  1.f, 0.2f }, { -0.809017f,  0.587785f,       -0.f }, {  0.203075f,  0.279508f,  0.809017f }, {  0.540557f,  0.744013f, -0.392737f } },
		{ {   0.17772f, -0.267175f,        0.f }, {  0.f, 0.2f }, { -0.809017f, -0.587785f,        0.f }, { -0.203075f,  0.279509f,  0.809017f }, { -0.540557f,  0.744013f, -0.392738f } },
		{ {  0.054918f, -0.267175f,  0.169021f }, { 0.2f, 0.2f }, {     -0.25f, -0.587785f, -0.769421f }, { -0.899207f, -0.111928f,  0.377675f }, { -0.313855f,  0.800943f, -0.509888f } },
		{ { -0.143778f, -0.267175f,  0.104461f }, { 0.4f, 0.2f }, {  0.654509f, -0.587785f, -0.475528f }, { -0.637061f, -0.111928f, -0.738488f }, {  0.387946f,  0.800943f, -0.456058f } },
		{ { -0.143778f, -0.267175f, -0.104461f }, { 0.6f, 0.2f }, {  0.654508f, -0.587785f,  0.475528f }, {  0.505481f, -0.111928f, -0.834086f }, {  0.553619f,  0.800943f,  0.228029f } },
		{ {  0.054918f, -0.267175f, -0.169021f }, { 0.8f, 0.2f }, {     -0.25f, -0.587785f,  0.769421f }, {  0.949465f, -0.111928f,  0.222995f }, { -0.045791f,  0.800943f,  0.596988f } },
		{ {   0.17772f, -0.267175f,        0.f }, {  1.f, 0.2f }, { -0.809017f, -0.587785f,       -0.f }, {  0.203075f, -0.279509f,  0.809017f }, { -0.540557f,  0.744013f,  0.392738f } },
		{ {  0.685917f, -0.432298f,        0.f }, {  0.f, 0.6f }, {  0.309017f, -0.951056f,        0.f }, { -0.531657f, -0.172746f,  0.809017f }, { -0.782436f, -0.254229f, -0.568473f } },
		{ {   0.21196f, -0.432298f,  0.652346f }, { 0.2f, 0.6f }, {  0.095492f, -0.951056f,  0.293893f }, { -0.951057f,        0.f,  0.309017f }, { -0.293893f, -0.309017f, -0.904508f } },
		{ { -0.554919f, -0.432298f,  0.403172f }, { 0.4f, 0.6f }, {     -0.25f, -0.951056f,  0.181636f }, { -0.587785f,       -0.f, -0.809017f }, {  0.769421f, -0.309017f, -0.559017f } },
		{ { -0.554918f, -0.432298f, -0.403172f }, { 0.6f, 0.6f }, {     -0.25f, -0.951056f, -0.181636f }, {  0.587785f,       -0.f, -0.809017f }, {  0.769421f, -0.309017f,  0.559017f } },
		{ {   0.21196f, -0.432298f, -0.652346f }, { 0.8f, 0.6f }, {  0.095492f, -0.951056f, -0.293893f }, {  0.951057f,        0.f,  0.309017f }, { -0.293893f, -0.309017f,  0.904508f } },
		{ {  0.685917f, -0.432298f,        0.f }, {  1.f, 0.6f }, {  0.309017f, -0.951056f,        0.f }, {  0.531656f,  0.172746f,  0.809017f }, { -0.782436f, -0.254229f,  0.568473f } },
		{ {        1.f,        0.f,        0.f }, {  0.f,  1.f }, {        1.f,        0.f,        0.f }, {       -0.f,        0.f,  0.809017f }, {        0.f,       -1.f,        0.f } },
		{ {  0.309017f,        0.f,  0.951056f }, { 0.2f,  1.f }, {  0.309017f,        0.f,  0.951057f }, { -0.924336f,        0.f,  0.300335f }, {        0.f,       -1.f,        0.f } },
		{ { -0.809017f,        0.f,  0.587785f }, { 0.4f,  1.f }, { -0.809017f,        0.f,  0.587785f }, { -0.571271f,        0.f, -0.786287f }, {       -0.f,       -1.f,        0.f } },
		{ { -0.809017f,        0.f, -0.587785f }, { 0.6f,  1.f }, { -0.809017f,        0.f, -0.587785f }, {  0.571271f,        0.f, -0.786287f }, {       -0.f,       -1.f,       -0.f } },
		{ {  0.309017f,        0.f, -0.951056f }, { 0.8f,  1.f }, {  0.309017f,        0.f, -0.951056f }, {  0.924336f,        0.f,  0.300335f }, {        0.f,       -1.f,       -0.f } },
		{ {        1.f,        0.f,        0.f }, {  1.f,  1.f }, {        1.f,        0.f,        0.f }, {       -0.f,       -0.f,  0.809017f }, {        0.f,       -1.f,       -0.f } }
	};

	static const std::vector<unsigned int> expectedIndices = {
		 6,  1,  0,
		 1,  6,  7,
		 7,  2,  1,
		 2,  7,  8,
		 8,  3,  2,
		 3,  8,  9,
		 9,  4,  3,
		 4,  9, 10,
		10,  5,  4,
		 5, 10, 11,
		12,  7,  6,
		 7, 12, 13,
		13,  8,  7,
		 8, 13, 14,
		14,  9,  8,
		 9, 14, 15,
		15, 10,  9,
		10, 15, 16,
		16, 11, 10,
		11, 16, 17,
		18, 13, 12,
		13, 18, 19,
		19, 14, 13,
		14, 19, 20,
		20, 15, 14,
		15, 20, 21,
		21, 16, 15,
		16, 21, 22,
		22, 17, 16,
		17, 22, 23,
		24, 19, 18,
		19, 24, 25,
		25, 20, 19,
		20, 25, 26,
		26, 21, 20,
		21, 26, 27,
		27, 22, 21,
		22, 27, 28,
		28, 23, 22,
		23, 28, 29,
		30, 25, 24,
		25, 30, 31,
		31, 26, 25,
		26, 31, 32,
		32, 27, 26,
		27, 32, 33,
		33, 28, 27,
		28, 33, 34,
		34, 29, 28,
		29, 34, 35
	};

	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

	TestableTorus torus(config, 5u, 5u, 1.2f, 1.0f, TorusShading::SMOOTH, ValuesRange::ONE_TO_ONE);

	const auto& v = torus.getVertices();
	const auto& i = torus.getIndices();

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