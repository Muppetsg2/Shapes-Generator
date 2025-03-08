// PRECOMPILED HEADER
#include "pch.h"
#include "Pyramid.h"

Pyramid::Pyramid(ValuesRange range)
{
	vertices.clear();
	indices.clear();

	float mult = range == ValuesRange::HALF_TO_HALF ? 1.f : 2.f;

	float sqrt_2 = sqrtf(2) * mult;
	float h = sqrt_2 / 2.f;

	std::vector<unsigned int> trisNum;

	// SQUARE BOTTOM
	for (int i = 0; i < 4; ++i) {
		float x = (-.5f + (float)(i % 2)) * mult;
		float z = (.5f - (float)(i / 2)) * mult;
		vertices.push_back({ { x, -h / 2.f, z }, { (float)(i % 2), (float)(i / 2) }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		if (i < 2) {
			trisNum.push_back(1 + i);
		}
		else {
			trisNum.push_back(i - (i % 2));
		}
	}

	for (size_t i = 0; i < 2ull; ++i) {
		indices.push_back((unsigned int)i);
		indices.push_back(2);
		indices.push_back((unsigned int)i * 2 + 1);

		std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent((unsigned int)i, 2, (unsigned int)i * 2 + 1);

		vertices[i].Tangent += TB.first;
		vertices[i].Bitangent += TB.second;

		vertices[2].Tangent += TB.first;
		vertices[2].Bitangent += TB.second;

		vertices[i * 2 + 1].Tangent += TB.first;
		vertices[i * 2 + 1].Bitangent += TB.second;
	}

	// TOP TRIANGLES
	size_t start = vertices.size();
	float cos_cone = sqrt_2 / sqrtf(sqrt_2 * sqrt_2 + h * h);
	float sin_cone = h / sqrtf(sqrt_2 * sqrt_2 + h * h);

	float x = -.5f;
	float z = .5f;

	for (int i = 0; i < 4; ++i) {
		float angle = glm::radians(i * 90.f);

		float x_n = cos_cone * sinf(angle);
		float z_n = cos_cone * cosf(angle);

		glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

		vertices.push_back({ { x * mult, -h / 2.f, z * mult }, { 0.f, 1.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);

		vertices.push_back({ { (x + cosf(angle)) * mult, -h / 2.f, (z + sinf(glm::radians(180.f) + angle)) * mult }, { 1.f, 1.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);

		vertices.push_back({ { 0.f, h / 2.f, 0.f }, { .5f, 0.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);

		x += cosf(angle);
		z += sinf(glm::radians(180.f) + angle);
	}

	for (size_t i = 0; i < 4ull; ++i) {
		size_t left = start + i * 3;
		size_t right = start + i * 3 + 1;
		size_t top = start + i * 3 + 2;

		indices.push_back((unsigned int)left);
		indices.push_back((unsigned int)right);
		indices.push_back((unsigned int)top);

		std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent((unsigned int)left, (unsigned int)right, (unsigned int)top);

		vertices[left].Tangent += TB.first;
		vertices[left].Bitangent += TB.second;

		vertices[right].Tangent += TB.first;
		vertices[right].Bitangent += TB.second;

		vertices[top].Tangent += TB.first;
		vertices[top].Bitangent += TB.second;
	}

	for (size_t i = 0; i < vertices.size(); ++i) {
		vertices[i].Tangent /= (float)trisNum[i];

		if (glm::length(vertices[i].Tangent) != 0.f) {
			vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
		}

		vertices[i].Bitangent /= (float)trisNum[i];

		if (glm::length(vertices[i].Bitangent) != 0.f) {
			vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
		}
	}

	trisNum.clear();
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