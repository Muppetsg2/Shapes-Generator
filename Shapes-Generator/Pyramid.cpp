// PRECOMPILED HEADER
#include "pch.h"
#include "Pyramid.h"

void Pyramid::_generate(ValuesRange range)
{
	float mult = range == ValuesRange::HALF_TO_HALF ? 1.f : 2.f;

	float sqrt_2 = (float)M_SQRT2 * mult;
	float h = sqrt_2 * 0.5f;

	std::vector<unsigned int> trisNum;

	// SQUARE BOTTOM
	for (int i = 0; i < 4; ++i) {
		float x = (-.5f + (float)(i % 2)) * mult;
		float z = (.5f - (float)(i / 2)) * mult;
		_vertices.push_back({ { x, -h * 0.5f, z }, { (float)(i % 2), (float)(i / 2) }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		if (i < 2) {
			trisNum.push_back(1u + (unsigned int)i);
		}
		else {
			trisNum.push_back((unsigned int)(i - (i % 2)));
		}
	}

	for (unsigned int i = 0u; i < 2u; ++i) {
		unsigned int f = i;
		unsigned int s = 2u;
		unsigned int t = i * 2u + 1u;

		_indices.push_back(f);
		_indices.push_back(s);
		_indices.push_back(t);

		std::pair<glm::vec3, glm::vec3> TB = _calcTangentBitangent(f, s, t);

		_vertices[(size_t)f].Tangent += TB.first;
		_vertices[(size_t)f].Bitangent += TB.second;

		_vertices[(size_t)s].Tangent += TB.first;
		_vertices[(size_t)s].Bitangent += TB.second;

		_vertices[(size_t)t].Tangent += TB.first;
		_vertices[(size_t)t].Bitangent += TB.second;
	}

	// TOP TRIANGLES
	size_t start = _vertices.size();
	float cos_cone = sqrt_2 / sqrtf(sqrt_2 * sqrt_2 + h * h);
	float sin_cone = h / sqrtf(sqrt_2 * sqrt_2 + h * h);

	float x = -.5f;
	float z = .5f;

	for (int i = 0; i < 4; ++i) {
		float angle = (float)i * (float)M_PI_2;

		float cos_angle = cosf(angle);
		float sin_angle_pi = sinf((float)M_PI + angle);

		float x_n = cos_cone * sinf(angle);
		float z_n = cos_cone * cos_angle;

		glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

		_vertices.push_back({ { x * mult, -h * 0.5f, z * mult }, { 0.f, 1.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1u);

		_vertices.push_back({ { (x + cos_angle) * mult, -h * 0.5f, (z + sin_angle_pi) * mult }, { 1.f, 1.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1u);

		_vertices.push_back({ { 0.f, h * 0.5f, 0.f }, { .5f, 0.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1u);

		x += cos_angle;
		z += sin_angle_pi;
	}

	std::pair<glm::vec3, glm::vec3> TB;
	for (size_t i = 0ull; i < 4ull; ++i) {
		size_t left = start + i * 3ull;
		size_t right = start + i * 3ull + 1ull;
		size_t top = start + i * 3ull + 2ull;

		_indices.push_back((unsigned int)left);
		_indices.push_back((unsigned int)right);
		_indices.push_back((unsigned int)top);

		TB = _calcTangentBitangent((unsigned int)left, (unsigned int)right, (unsigned int)top);

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

Pyramid::Pyramid(ValuesRange range)
{
	_vertices.clear();
	_indices.clear();
	_generate(range);
}

Pyramid::~Pyramid() {}

std::string Pyramid::getClassName()
{
	return "Pyramid";
}

std::string Pyramid::getObjectClassName() const
{
	return Pyramid::getClassName();
}