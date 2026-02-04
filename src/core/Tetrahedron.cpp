#include "pch.hpp"
#include "Tetrahedron.hpp"
#include "Shape.hpp"
#include "Constants.hpp"

#include <string>
#include <utility>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

void Tetrahedron::_generate(const ValuesRange range)
{
	const float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

	const unsigned int segments = 3u;
	const float h = 2.f / 3.f;
	const float r = (float)M_SQRT3 / 3.f;

	std::vector<unsigned int> trisNum;
	glm::vec3 tangent;

	const float angleXZDiff = 2.f * (float)M_PI / (float)segments;

	// CIRCLE BOTTOM
	// VERTICES AND TEX COORDS
	const float y = -h * 0.5f;
	float angleXZ = 0.f;
	for (unsigned int j = 0u; j < segments; ++j) {
		const float z = cosf(angleXZ);
		const float x = sinf(angleXZ);
		_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { fabsf(.5f - (float)((int)((float)j * 1.5f)) * .5f), (j == 0u ? 1.f : 0.f) }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		if (_shapeConfig.genTangents) trisNum.push_back(1u);
		angleXZ += angleXZDiff;
	}

	_indices.push_back(0u);
	_indices.push_back(2u);
	_indices.push_back(1u);

	if (_shapeConfig.genTangents) {
		tangent = _calcTangent(0u, 2u, 1u);

		_vertices[0ull].Tangent += tangent;
		_vertices[2ull].Tangent += tangent;
		_vertices[1ull].Tangent += tangent;
	}

	// CONE
	// VERTICES AND TEX COORDS
	const size_t start = _vertices.size();
	angleXZ = 0.f;
	const float cos_cone = r / sqrtf(r * r + h * h);
	const float sin_cone = h / sqrtf(r * r + h * h);
	for (unsigned int j = 0u; j < segments; ++j) {
		const float x_n = cos_cone * (sinf(angleXZ) + sinf(angleXZ + angleXZDiff)) * .5f;
		const float z_n = cos_cone * (cosf(angleXZ) + cosf(angleXZ + angleXZDiff)) * .5f;

		const glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

		for (int i = 0; i < 2; ++i) {
			const float angle = angleXZ + (float)i * angleXZDiff;
			const float z = cosf(angle);
			const float x = sinf(angle);

			_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { (float)i, 1.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
			if (_shapeConfig.genTangents) trisNum.push_back(1u);
		}

		// sqrt_3 * sqrtf(2) * .5f - .5f it came out from previous normalization of vertices.
		// In shortcut it is height of Tetrahedron minus y value of normalized vertex
		_vertices.push_back({ glm::vec3(0.f, (M_SQRT3 * M_SQRT2 * .5f) - .5f, 0.f) * mult, {.5f, 0.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
		if (_shapeConfig.genTangents) trisNum.push_back(1u);

		angleXZ += angleXZDiff;
	}

	// INDICES
	for (unsigned int i = 0u; i < segments; ++i) {
		const size_t left = start + (size_t)i * 3ull;
		const size_t right = start + (size_t)i * 3ull + 1ull;
		const size_t top = start + (size_t)i * 3ull + 2ull;

		_indices.push_back((unsigned int)left);
		_indices.push_back((unsigned int)right);
		_indices.push_back((unsigned int)top);

		if (_shapeConfig.genTangents) {
			tangent = _calcTangent((unsigned int)left, (unsigned int)right, (unsigned int)top);

			_vertices[left].Tangent += tangent;
			_vertices[right].Tangent += tangent;
			_vertices[top].Tangent += tangent;
		}
	}

	if (_shapeConfig.genTangents) _normalizeTangentsAndGenerateBitangents(trisNum, 0ull, _vertices.size());

	trisNum.clear();
}

Tetrahedron::Tetrahedron(const ShapeConfig& config, const ValuesRange range)
{
	_shapeConfig = config;
	_vertices.clear();
	_indices.clear();
	_generate(range);
}

Tetrahedron::~Tetrahedron() {}

std::string Tetrahedron::getClassName()
{
	return "Tetrahedron";
}

std::string Tetrahedron::getObjectClassName() const
{
	return Tetrahedron::getClassName();
}