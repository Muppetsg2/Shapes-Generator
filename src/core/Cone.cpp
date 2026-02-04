#include "pch.hpp"
#include "Cone.hpp"
#include "Shape.hpp"

#include <algorithm>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

void Cone::_generate(const unsigned int segments, const float height, const float radius, const ValuesRange range, const bool useFlatShading)
{
	const float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

	const float h = height < EPSILON ? 1.f : height;
	float r = radius < EPSILON ? 1.f : radius;

	std::vector<unsigned int> trisNum;

	const float angleXZDiff = 2.f * (float)M_PI / (float)segments;

	// CIRCLE BOTTOM
	// VERTICES AND TEX COORDS
	const float rToH = r / h;
	const float y = -sqrtf(rToH);
	r = sqrtf(rToH);

	float angleXZ = 0.f;
	for (unsigned int j = 0u; j < segments; ++j) {
		const float z = cosf(angleXZ);
		const float x = sinf(angleXZ);
		_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { .5f + x * .5f, .5f + z * .5f }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		if (_shapeConfig.genTangents) trisNum.push_back(2u);
		angleXZ += angleXZDiff;
	}
	_vertices.push_back({ glm::vec3(0.f, _vertices[_vertices.size() - 1ull].Position.y, 0.f) * mult, {.5f, .5f}, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f)});
	if (_shapeConfig.genTangents) trisNum.push_back(segments);

	// INDICES
	const size_t vertSize = _vertices.size();
	glm::vec3 tangent;
	for (size_t i = 0ull; i < vertSize - 1ull; ++i) {

		const size_t right = i + 2ull == vertSize ? 0ull : i + 1ull;

		_indices.push_back((unsigned int)right);
		_indices.push_back((unsigned int)i);
		_indices.push_back((unsigned int)vertSize - 1u);

		if (_shapeConfig.genTangents) {
			tangent = _calcTangent((unsigned int)right, (unsigned int)i, (unsigned int)vertSize - 1u);

			_vertices[right].Tangent += tangent;
			_vertices[i].Tangent += tangent;
			_vertices[vertSize - 1ull].Tangent += tangent;
		}
	}

	// CONE
	// VERTICES AND TEX COORDS
	const size_t start = _vertices.size();
	angleXZ = 0.f;
	const glm::vec3 vr = glm::vec3(r, 0.f, 0.f);
	const glm::vec3 vh = glm::vec3(0.f, -y * 2.f, 0.f);
	const glm::vec3 vp = glm::normalize(glm::cross(vh - vr, glm::cross(vr, vh)));
	const float sin_cone = vp.y;
	const float cos_cone = vp.x;

	const unsigned int count = segments + (useFlatShading ? 0u : 1u);
	for (unsigned int j = 0u; j < count; ++j) {
		if (useFlatShading) {
			const float x_n = cos_cone * (sinf(angleXZ) + sinf(angleXZ + angleXZDiff)) * .5f;
			const float z_n = cos_cone * (cosf(angleXZ) + cosf(angleXZ + angleXZDiff)) * .5f;

			const glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

			for (int i = 0; i < 2; ++i) {
				const float angle = angleXZ + i * angleXZDiff;
				const float z = cosf(angle);
				const float x = sinf(angle);

				_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { (float)i, 1.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
				if (_shapeConfig.genTangents) trisNum.push_back(1u);
			}

			_vertices.push_back({ glm::normalize(glm::vec3(0.f, -y, 0.f)) * mult, {.5f, 0.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
			if (_shapeConfig.genTangents) trisNum.push_back(1u);
		}
		else {
			const float x = sinf(angleXZ);
			const float z = cosf(angleXZ);

			const glm::vec3 norm = glm::normalize(glm::vec3(cos_cone * x, sin_cone, cos_cone * z));

			// Also good aproach
			//constexpr float uC = .5f;
			//constexpr float vC = .25f;
			//constexpr float radiansUV0 = 0.75f * (float)M_PI;
			//constexpr float uvR = (float)M_SQRT2 * .5f;

			//float radiansUV = _fmapf(angleXZ, 0.f, 2.f * (float)M_PI, 0.f, (float)M_PI_2);
			//float u = uC + uvR * cosf(radiansUV0 - radiansUV);
			//float v = vC + uvR * sinf(radiansUV0 - radiansUV);

			constexpr float uC = .5f;
			constexpr float vC = 0.f;
			constexpr float radiansUV0 = (float)M_PI_3 * .5f;
			
			const float radiansUV = _map(angleXZ, 0.f, 2.f * (float)M_PI, 0.f, (float)M_PI_3);
			const float u = uC + sinf(radiansUV - radiansUV0);
			const float v = vC + cosf(radiansUV - radiansUV0);

			_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { u, v }, norm, glm::vec3(0.f), glm::vec3(0.f) });

			if (_shapeConfig.genTangents) {
				if (j == 0u || j == segments) {
					trisNum.push_back(1u);
				}
				else {
					trisNum.push_back(2u);
				}
			}
		}

		angleXZ += angleXZDiff;
	}

	if (!useFlatShading) {
		_vertices.push_back({ glm::normalize(glm::vec3(0.f, -y, 0.f)) * mult, { .5f, 0.f }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
		if (_shapeConfig.genTangents) trisNum.push_back(segments);
	}

	// INDICES
	for (unsigned int i = 0u; i < segments; ++i) {

		const size_t m = useFlatShading ? 3ull : 1ull;

		const size_t left = start + (size_t)i * m;
		const size_t right = start + (size_t)i * m + 1ull;
		const size_t top = useFlatShading ? start + (size_t)i * m + 2ull : _vertices.size() - 1ull;

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

Cone::Cone(const ShapeConfig& config, const unsigned int segments, const float height, const float radius, const ConeShading shading, const ValuesRange range)
{
	_shapeConfig = config;
	_vertices.clear();
	_indices.clear();
	_generate(std::max(3u, segments), std::max(EPSILON, height), std::max(EPSILON, radius), range, shading == ConeShading::FLAT);
}

Cone::~Cone() {}

std::string Cone::getClassName()
{
	return "Cone";
}

std::string Cone::getObjectClassName() const
{
	return Cone::getClassName();
}