// PRECOMPILED HEADER
#include "pch.h"
#include "Cone.h"

void Cone::_generate(unsigned int segments, float height, float radius, ValuesRange range, bool useFlatShading)
{
	float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

	float h = height < EPSILON ? 1.f : height;
	float r = radius < EPSILON ? 1.f : radius;

	std::vector<unsigned int> trisNum;

	float angleXZDiff = 2.f * (float)M_PI / (float)segments;

	// CIRCLE BOTTOM
	// VERTICES AND TEX COORDS
	float rToH = r / h;
	float y = -sqrtf(rToH);
	r = sqrtf(rToH);

	float angleXZ = 0.f;
	for (unsigned int j = 0u; j < segments; ++j) {
		float z = cosf(angleXZ);
		float x = sinf(angleXZ);
		_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { .5f + x * .5f, .5f + z * .5f }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(2u);
		angleXZ += angleXZDiff;
	}
	_vertices.push_back({ glm::vec3(0.f, _vertices[_vertices.size() - 1ull].Position.y, 0.f) * mult, {.5f, .5f}, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f)});
	trisNum.push_back(segments);

	// INDICES
	const size_t vertSize = _vertices.size();
	for (size_t i = 0ull; i < vertSize - 1ull; ++i) {

		size_t right = i + 2ull == vertSize ? 0ull : i + 1ull;

		_indices.push_back((unsigned int)right);
		_indices.push_back((unsigned int)i);
		_indices.push_back((unsigned int)vertSize - 1u);

		std::pair<glm::vec3, glm::vec3> TB = _calcTangentBitangent((unsigned int)right, (unsigned int)i, (unsigned int)vertSize - 1u);

		_vertices[right].Tangent += TB.first;
		_vertices[right].Bitangent += TB.second;

		_vertices[i].Tangent += TB.first;
		_vertices[i].Bitangent += TB.second;

		_vertices[vertSize - 1ull].Tangent += TB.first;
		_vertices[vertSize - 1ull].Bitangent += TB.second;
	}

	// CONE
	// VERTICES AND TEX COORDS
	size_t start = _vertices.size();
	angleXZ = 0.f;
	glm::vec3 vr = glm::vec3(r, 0.f, 0.f);
	glm::vec3 vh = glm::vec3(0.f, -y * 2.f, 0.f);
	glm::vec3 vp = glm::normalize(glm::cross(vh - vr, glm::cross(vr, vh)));
	float sin_cone = vp.y;
	float cos_cone = vp.x;

	unsigned int count = segments + (useFlatShading ? 0u : 1u);
	for (unsigned int j = 0u; j < count; ++j) {
		if (useFlatShading) {
			float x_n = cos_cone * (sinf(angleXZ) + sinf(angleXZ + angleXZDiff)) * .5f;
			float z_n = cos_cone * (cosf(angleXZ) + cosf(angleXZ + angleXZDiff)) * .5f;

			glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

			for (int i = 0; i < 2; ++i) {
				float angle = angleXZ + i * angleXZDiff;
				float z = cosf(angle);
				float x = sinf(angle);

				_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { (float)i, 1.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
				trisNum.push_back(1u);
			}

			_vertices.push_back({ glm::normalize(glm::vec3(0.f, -y, 0.f)) * mult, {.5f, 0.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
			trisNum.push_back(1u);
		}
		else {
			float x = sinf(angleXZ);
			float z = cosf(angleXZ);

			glm::vec3 norm = glm::normalize(glm::vec3(cos_cone * x, sin_cone, cos_cone * z));

			constexpr float uC = .5f;
			constexpr float vC = .25f;
			constexpr float radiansUV0 = 0.75f * (float)M_PI;
			float uvR = (float)M_SQRT2 * .5f;

			float radiansUV = _fmapf(angleXZ, 0.f, 2.f * (float)M_PI, 0.f, (float)M_PI_2);
			float u = uC + uvR * cosf(radiansUV0 - radiansUV);
			float v = vC + uvR * sinf(radiansUV0 - radiansUV);

			_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { u, v }, norm, glm::vec3(0.f), glm::vec3(0.f) });

			if (j == 0u || j == segments) {
				trisNum.push_back(1u);
			}
			else {
				trisNum.push_back(2u);
			}
		}

		angleXZ += angleXZDiff;
	}

	if (!useFlatShading) {
		_vertices.push_back({ glm::normalize(glm::vec3(0.f, -y, 0.f)) * mult, { .5f, 0.f }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(segments);
	}

	// INDICES
	for (unsigned int i = 0u; i < segments; ++i) {

		size_t m = useFlatShading ? 3ull : 1ull;

		size_t left = start + (size_t)i * m;
		size_t right = start + (size_t)i * m + 1ull;
		size_t top = useFlatShading ? start + (size_t)i * m + 2ull : _vertices.size() - 1ull;

		_indices.push_back((unsigned int)left);
		_indices.push_back((unsigned int)right);
		_indices.push_back((unsigned int)top);

		std::pair<glm::vec3, glm::vec3> TB = _calcTangentBitangent((unsigned int)left, (unsigned int)right, (unsigned int)top);

		_vertices[left].Tangent += TB.first;
		_vertices[left].Bitangent += TB.second;

		_vertices[right].Tangent += TB.first;
		_vertices[right].Bitangent += TB.second;

		_vertices[top].Tangent += TB.first;
		_vertices[top].Bitangent += TB.second;
	}

	_normalizeTangents(trisNum, 0ull, _vertices.size());

	trisNum.clear();
}

Cone::Cone(unsigned int segments, float height, float radius, ConeShading shading, ValuesRange range)
{
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