#include "pch.hpp"
#include "Vertex.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"

#include "Helpers.hpp"

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

class TestableSphere : public Sphere {
public:
    using Sphere::Sphere;
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
};

TEST_CASE("ShapesGenerator.Sphere.Minimal.Valid") {
	ShapeConfig config{};
	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	const auto& v = sphere.getVertices();
	const auto& i = sphere.getIndices();

	REQUIRE_FALSE(v.empty());
	REQUIRE_FALSE(i.empty());

	for (unsigned idx : i) {
		REQUIRE(idx < v.size());
	}
}

TEST_CASE("ShapesGenerator.Sphere.NoTangents") {
	ShapeConfig config{};
	config.genTangents = false;

	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		CHECK(v.Tangent == glm::vec3(0.f));
		CHECK(v.Bitangent == glm::vec3(0.f));
	}
}

TEST_CASE("ShapesGenerator.Sphere.NoBitangents") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = false;

	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent == glm::vec3(0.f));
	}
}

TEST_CASE("ShapesGenerator.Sphere.PositiveHandedness") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent != glm::vec3(0.f));

		glm::vec3 expected = glm::normalize(glm::cross(v.Normal, v.Tangent));
		CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
	}
}

TEST_CASE("ShapesGenerator.Sphere.NegativeHandedness") {
	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = false;

	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		CHECK(v.Tangent != glm::vec3(0.f));
		CHECK(v.Bitangent != glm::vec3(0.f));

		glm::vec3 expected = -glm::normalize(glm::cross(v.Normal, v.Tangent));
		CheckVec3Equal(v.Bitangent, expected, TEST_EPSILON, "Bitangent");
	}
}

TEST_CASE("ShapesGenerator.Sphere.Position.Range.OneToOne") {
	ShapeConfig config{};
	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		CheckInRange(v.Position.x, -1.0f, 1.0f, TEST_EPSILON, "Position.x");
		CheckInRange(v.Position.y, -1.0f, 1.0f, TEST_EPSILON, "Position.y");
		CheckInRange(v.Position.z, -1.0f, 1.0f, TEST_EPSILON, "Position.z");
	}
}

TEST_CASE("ShapesGenerator.Sphere.Position.Range.HalfToHalf") {
	ShapeConfig config{};
	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::HALF_TO_HALF);

	for (const auto& v : sphere.getVertices()) {
		CheckInRange(v.Position.x, -0.5f, 0.5f, TEST_EPSILON, "Position.x");
		CheckInRange(v.Position.y, -0.5f, 0.5f, TEST_EPSILON, "Position.y");
		CheckInRange(v.Position.z, -0.5f, 0.5f, TEST_EPSILON, "Position.z");
	}
}

TEST_CASE("ShapesGenerator.Sphere.Normals.UnitLength") {
	ShapeConfig config{};
	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		float len = glm::length(v.Normal);
		REQUIRE(len == Catch::Approx(1.f).epsilon(TEST_EPSILON));
	}
}

TEST_CASE("ShapesGenerator.Sphere.Tangents.UnitLength") {
	ShapeConfig config{};
	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		REQUIRE(glm::length(v.Tangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
	}
}

TEST_CASE("ShapesGenerator.Sphere.Bitangents.UnitLength") {
	ShapeConfig config{};
	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		REQUIRE(glm::length(v.Bitangent) == Catch::Approx(1.f).epsilon(TEST_EPSILON));
	}
}

TEST_CASE("ShapesGenerator.Sphere.TexCoord.Range") {
	ShapeConfig config{};
	TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	for (const auto& v : sphere.getVertices()) {
		CheckInRange(v.TexCoord.x, 0.0f, 1.0f, TEST_EPSILON, "TexCoord.x");
		CheckInRange(v.TexCoord.y, 0.0f, 1.0f, TEST_EPSILON, "TexCoord.y");
	}
}

TEST_CASE("ShapesGenerator.Sphere.IndexCount") {
	constexpr unsigned h = 3;
	constexpr unsigned v = 3;

	ShapeConfig config{};
	TestableSphere sphere(config, h, v, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

	// Sphere:
	// - top cap:    h triangles
	// - bottom cap: h triangles
	// - middle:     (v - 2) * h * 2 triangles
	const unsigned expectedTriangles = h * 2 + (v - 2) * h * 2;

	REQUIRE(sphere.getIndices().size() == expectedTriangles * 3);
}

TEST_CASE("ShapesGenerator.Sphere.Generation(H[3]V[3].FLAT.TBP)") {
	static const std::vector<Vertex> expectedVertices = {
		   // POSITION                    // TEX COORD              // NORMAL                               // TANGENT                             // BITANGENT
		{ {  0.75f,  0.5f, -0.433013f }, { 0.333333f, 0.333333f }, {  0.344124f,  0.917663f,   0.19868f }, {       0.5f,       0.f, -0.866025f }, { -0.794719f,  0.39736f, -0.458831f } },
		{ {    0.f,   1.f,        0.f }, {      0.5f,       0.f }, {  0.344124f,  0.917663f,   0.19868f }, {       0.5f,       0.f, -0.866025f }, { -0.794719f,  0.39736f, -0.458831f } },
		{ {    0.f,  0.5f,  0.866025f }, {       0.f, 0.333333f }, {  0.344124f,  0.917663f,   0.19868f }, {       0.5f,       0.f, -0.866025f }, { -0.794719f,  0.39736f, -0.458831f } },
		{ {  0.75f, -0.5f, -0.433013f }, { 0.333333f, 0.666667f }, {  0.344123f, -0.917663f,   0.19868f }, {       0.5f,       0.f, -0.866026f }, {   0.79472f,  0.39736f,  0.458831f } },
		{ {    0.f, -0.5f,  0.866025f }, {       0.f, 0.666667f }, {  0.344123f, -0.917663f,   0.19868f }, {       0.5f,       0.f, -0.866026f }, {   0.79472f,  0.39736f,  0.458831f } },
		{ {    0.f,  -1.f,        0.f }, {      0.5f,       1.f }, {  0.344123f, -0.917663f,   0.19868f }, {       0.5f,       0.f, -0.866026f }, {   0.79472f,  0.39736f,  0.458831f } },
		{ { -0.75f,  0.5f, -0.433013f }, { 0.666667f, 0.333333f }, {       -0.f,  0.917663f,  -0.39736f }, {       -1.f,       0.f,        0.f }, {        0.f,  0.39736f,  0.917663f } },
		{ {    0.f,   1.f,        0.f }, {      0.5f,       0.f }, {       -0.f,  0.917663f,  -0.39736f }, {       -1.f,       0.f,        0.f }, {        0.f,  0.39736f,  0.917663f } },
		{ {  0.75f,  0.5f, -0.433013f }, { 0.333333f, 0.333333f }, {       -0.f,  0.917663f,  -0.39736f }, {       -1.f,       0.f,        0.f }, {        0.f,  0.39736f,  0.917663f } },
		{ { -0.75f, -0.5f, -0.433012f }, { 0.666667f, 0.666667f }, {       -0.f, -0.917663f,  -0.39736f }, {       -1.f,      -0.f,        0.f }, {       -0.f,  0.39736f, -0.917663f } },
		{ {  0.75f, -0.5f, -0.433013f }, { 0.333333f, 0.666667f }, {       -0.f, -0.917663f,  -0.39736f }, {       -1.f,      -0.f,        0.f }, {       -0.f,  0.39736f, -0.917663f } },
		{ {    0.f,  -1.f,        0.f }, {      0.5f,       1.f }, {       -0.f, -0.917663f,  -0.39736f }, {       -1.f,      -0.f,        0.f }, {       -0.f,  0.39736f, -0.917663f } },
		{ {    0.f,  0.5f,  0.866025f }, {       1.f, 0.333333f }, { -0.344124f,  0.917663f,   0.19868f }, {       0.5f,      -0.f,  0.866025f }, {  0.794719f,  0.39736f, -0.458832f } },
		{ {    0.f,   1.f,        0.f }, {      0.5f,       0.f }, { -0.344124f,  0.917663f,   0.19868f }, {       0.5f,      -0.f,  0.866025f }, {  0.794719f,  0.39736f, -0.458832f } },
		{ { -0.75f,  0.5f, -0.433013f }, { 0.666667f, 0.333333f }, { -0.344124f,  0.917663f,   0.19868f }, {       0.5f,      -0.f,  0.866025f }, {  0.794719f,  0.39736f, -0.458832f } },
		{ {    0.f, -0.5f,  0.866025f }, {       1.f, 0.666667f }, { -0.344124f, -0.917663f,   0.19868f }, {       0.5f,       0.f,  0.866025f }, { -0.794719f,  0.39736f,  0.458832f } },
		{ { -0.75f, -0.5f, -0.433012f }, { 0.666667f, 0.666667f }, { -0.344124f, -0.917663f,   0.19868f }, {       0.5f,       0.f,  0.866025f }, { -0.794719f,  0.39736f,  0.458832f } },
		{ {    0.f,  -1.f,        0.f }, {      0.5f,       1.f }, { -0.344124f, -0.917663f,   0.19868f }, {       0.5f,       0.f,  0.866025f }, { -0.794719f,  0.39736f,  0.458832f } },
		{ {  0.75f,  0.5f, -0.433013f }, { 0.333333f, 0.333333f }, {  0.474342f,  0.316228f,  0.821584f }, {  0.823545f, 0.170389f, -0.541056f }, { -0.311085f, 0.933257f, -0.179605f } },
		{ {    0.f,  0.5f,  0.866025f }, {       0.f, 0.333333f }, {  0.474342f,  0.316228f,  0.821584f }, {  0.823545f, 0.170389f, -0.541056f }, { -0.311085f, 0.933257f, -0.179605f } },
		{ {    0.f, -0.5f,  0.866025f }, {       0.f, 0.666667f }, {  0.474342f,  0.316228f,  0.821584f }, {  0.823545f, 0.170389f, -0.541056f }, { -0.311085f, 0.933257f, -0.179605f } },
		{ {  0.75f, -0.5f, -0.433013f }, { 0.333333f, 0.666667f }, {  0.948683f, -0.316228f,       -0.f }, {  0.056796f, 0.170389f, -0.983739f }, {  0.311086f, 0.933256f,  0.179605f } },
		{ {  0.75f,  0.5f, -0.433013f }, { 0.333333f, 0.333333f }, {  0.948683f, -0.316228f,       -0.f }, {  0.056796f, 0.170389f, -0.983739f }, {  0.311086f, 0.933256f,  0.179605f } },
		{ {    0.f, -0.5f,  0.866025f }, {       0.f, 0.666667f }, {  0.948683f, -0.316228f,       -0.f }, {  0.056796f, 0.170389f, -0.983739f }, {  0.311086f, 0.933256f,  0.179605f } },
		{ { -0.75f,  0.5f, -0.433013f }, { 0.666667f, 0.333333f }, {  0.474342f,  0.316228f, -0.821584f }, { -0.880341f, 0.170389f, -0.442682f }, {        0.f, 0.933257f,  0.359211f } },
		{ {  0.75f,  0.5f, -0.433013f }, { 0.333333f, 0.333333f }, {  0.474342f,  0.316228f, -0.821584f }, { -0.880341f, 0.170389f, -0.442682f }, {        0.f, 0.933257f,  0.359211f } },
		{ {  0.75f, -0.5f, -0.433013f }, { 0.333333f, 0.666667f }, {  0.474342f,  0.316228f, -0.821584f }, { -0.880341f, 0.170389f, -0.442682f }, {        0.f, 0.933257f,  0.359211f } },
		{ { -0.75f, -0.5f, -0.433012f }, { 0.666667f, 0.666667f }, { -0.474342f, -0.316228f, -0.821584f }, { -0.880341f, 0.170389f,  0.442683f }, {       -0.f, 0.933256f, -0.359211f } },
		{ { -0.75f,  0.5f, -0.433013f }, { 0.666667f, 0.333333f }, { -0.474342f, -0.316228f, -0.821584f }, { -0.880341f, 0.170389f,  0.442683f }, {       -0.f, 0.933256f, -0.359211f } },
		{ {  0.75f, -0.5f, -0.433013f }, { 0.333333f, 0.666667f }, { -0.474342f, -0.316228f, -0.821584f }, { -0.880341f, 0.170389f,  0.442683f }, {       -0.f, 0.933256f, -0.359211f } },
		{ {    0.f,  0.5f,  0.866025f }, {       1.f, 0.333333f }, { -0.948683f,  0.316228f,        0.f }, {  0.056796f, 0.170388f,  0.983739f }, {  0.311085f, 0.933257f, -0.179605f } },
		{ { -0.75f,  0.5f, -0.433013f }, { 0.666667f, 0.333333f }, { -0.948683f,  0.316228f,        0.f }, {  0.056796f, 0.170388f,  0.983739f }, {  0.311085f, 0.933257f, -0.179605f } },
		{ { -0.75f, -0.5f, -0.433012f }, { 0.666667f, 0.666667f }, { -0.948683f,  0.316228f,        0.f }, {  0.056796f, 0.170388f,  0.983739f }, {  0.311085f, 0.933257f, -0.179605f } },
		{ {    0.f, -0.5f,  0.866025f }, {       1.f, 0.666667f }, { -0.474342f, -0.316228f,  0.821584f }, {  0.823545f, 0.170389f,  0.541056f }, { -0.311086f, 0.933256f,  0.179605f } },
		{ {    0.f,  0.5f,  0.866025f }, {       1.f, 0.333333f }, { -0.474342f, -0.316228f,  0.821584f }, {  0.823545f, 0.170389f,  0.541056f }, { -0.311086f, 0.933256f,  0.179605f } },
		{ { -0.75f, -0.5f, -0.433012f }, { 0.666667f, 0.666667f }, { -0.474342f, -0.316228f,  0.821584f }, {  0.823545f, 0.170389f,  0.541056f }, { -0.311086f, 0.933256f,  0.179605f } }
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
		33, 34, 35
	};

	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

    TestableSphere sphere(config, 3u, 3u, SphereShading::FLAT, ValuesRange::ONE_TO_ONE);

    const auto& v = sphere.getVertices();
    const auto& i = sphere.getIndices();

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

TEST_CASE("ShapesGenerator.Sphere.Generation(H[4]V[4].SMOOTH.TBP)") {
	static const std::vector<Vertex> expectedVertices = {
		   // POSITION                             // TEX COORD      // NORMAL                               // TANGENT                              // BITANGENT
		{ {        0.f,        1.f,        0.f }, {  0.5f,   0.f }, {        0.f,        1.f,        0.f }, {        0.f,        0.f,        1.f }, {        1.f,       0.f,        0.f } },
		{ {        0.f,  0.707107f,  0.707107f }, {   0.f, 0.25f }, {        0.f,  0.707107f,  0.707107f }, {  0.816497f,  0.408248f, -0.408248f }, {  -0.57735f,  0.57735f,  -0.57735f } },
		{ {  0.707107f,  0.707107f,       -0.f }, { 0.25f, 0.25f }, {  0.707107f,  0.707107f,       -0.f }, {   0.12843f,  -0.12843f, -0.983368f }, { -0.695346f, 0.695346f, -0.181627f } },
		{ {       -0.f,  0.707107f, -0.707107f }, {  0.5f, 0.25f }, {       -0.f,  0.707107f, -0.707107f }, { -0.983368f,  -0.12843f,  -0.12843f }, { -0.181627f, 0.695346f,  0.695346f } },
		{ { -0.707107f,  0.707107f,        0.f }, { 0.75f, 0.25f }, { -0.707107f,  0.707107f,        0.f }, {  -0.12843f,  -0.12843f,  0.983368f }, {  0.695346f, 0.695346f,  0.181627f } },
		{ {        0.f,  0.707107f,  0.707107f }, {   1.f, 0.25f }, {        0.f,  0.707107f,  0.707107f }, {  0.816497f, -0.408248f,  0.408248f }, {   0.57735f,  0.57735f,  -0.57735f } },
		{ {        0.f,       -0.f,        1.f }, {   0.f,  0.5f }, {        0.f,       -0.f,        1.f }, {        1.f,       -0.f,        0.f }, {        0.f,       1.f,        0.f } },
		{ {        1.f,       -0.f,       -0.f }, { 0.25f,  0.5f }, {        1.f,       -0.f,       -0.f }, {       -0.f,        0.f,       -1.f }, {        0.f,       1.f,       -0.f } },
		{ {       -0.f,       -0.f,       -1.f }, {  0.5f,  0.5f }, {       -0.f,       -0.f,       -1.f }, {       -1.f,       -0.f,        0.f }, {       -0.f,       1.f,       -0.f } },
		{ {       -1.f,       -0.f,        0.f }, { 0.75f,  0.5f }, {       -1.f,       -0.f,        0.f }, {        0.f,        0.f,        1.f }, {       -0.f,       1.f,        0.f } },
		{ {        0.f,       -0.f,        1.f }, {   1.f,  0.5f }, {        0.f,       -0.f,        1.f }, {        1.f,        0.f,        0.f }, {       -0.f,       1.f,        0.f } },
		{ {        0.f, -0.707107f,  0.707107f }, {   0.f, 0.75f }, {        0.f, -0.707107f,  0.707107f }, {  0.816496f, -0.408248f, -0.408248f }, {   0.57735f,  0.57735f,   0.57735f } },
		{ {  0.707107f, -0.707107f,       -0.f }, { 0.25f, 0.75f }, {  0.707107f, -0.707107f,       -0.f }, {  -0.12843f,  -0.12843f, -0.983368f }, {  0.695346f, 0.695346f, -0.181627f } },
		{ {       -0.f, -0.707107f, -0.707107f }, {  0.5f, 0.75f }, {       -0.f, -0.707107f, -0.707107f }, { -0.983368f,  -0.12843f,   0.12843f }, { -0.181627f, 0.695346f, -0.695346f } },
		{ { -0.707107f, -0.707107f,        0.f }, { 0.75f, 0.75f }, { -0.707107f, -0.707107f,        0.f }, {   0.12843f,  -0.12843f,  0.983368f }, { -0.695346f, 0.695346f,  0.181627f } },
		{ {        0.f, -0.707107f,  0.707107f }, {   1.f, 0.75f }, {        0.f, -0.707107f,  0.707107f }, {  0.816497f,  0.408248f,  0.408248f }, {  -0.57735f,  0.57735f,   0.57735f } },
		{ {        0.f,       -1.f,        0.f }, {  0.5f,   1.f }, {        0.f,       -1.f,        0.f }, {        0.f,        0.f,        1.f }, {       -1.f,       0.f,        0.f } }
	};

	static const std::vector<unsigned int> expectedIndices = {
		 2,  0,  1,
		12, 11, 16,
		 3,  0,  2,
		13, 12, 16,
		 4,  0,  3,
		14, 13, 16,
		 5,  0,  4,
		15, 14, 16,
		 2,  1,  6,
		 7,  2,  6,
		 3,  2,  7,
		 8,  3,  7,
		 4,  3,  8,
		 9,  4,  8,
		 5,  4,  9,
		10,  5,  9,
		 7,  6, 11,
		12,  7, 11,
		 8,  7, 12,
		13,  8, 12,
		 9,  8, 13,
		14,  9, 13,
		10,  9, 14,
		15, 10, 14
	};

	ShapeConfig config{};
	config.genTangents = true;
	config.calcBitangents = true;
	config.tangentHandednessPositive = true;

	TestableSphere sphere(config, 4u, 4u, SphereShading::SMOOTH, ValuesRange::ONE_TO_ONE);

	const auto& v = sphere.getVertices();
	const auto& i = sphere.getIndices();

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