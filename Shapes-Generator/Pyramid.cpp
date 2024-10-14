#include "Pyramid.h"

Pyramid::Pyramid()
{
	float sqrt_2 = sqrtf(2);
	float h = sqrt_2 / 2.f;

	std::vector<unsigned int> trisNum;

	// SQUARE BOTTOM
	for (int i = 0; i < 4; ++i) {
		float x = -.5f + (float)(i % 2);
		float z = .5f - (float)(i / 2);
		vertices.push_back({ { x, -h / 2.f, z }, { (float)(i % 2), (float)(i / 2) }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		if (i < 2) {
			trisNum.push_back(1 + i);
		}
		else {
			trisNum.push_back(i - (i % 2));
		}
	}

	for (int i = 0; i < 2; ++i) {
		indices.push_back(i);
		indices.push_back(2);
		indices.push_back(i * 2 + 1);

		std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(i, 2, i * 2 + 1);

		vertices[i].Tangent += TB.first;
		vertices[i].Bitangent += TB.second;

		vertices[2].Tangent += TB.first;
		vertices[2].Bitangent += TB.second;

		vertices[i * 2 + 1].Tangent += TB.first;
		vertices[i * 2 + 1].Bitangent += TB.second;
	}

	// TOP TRIANGLES
	int start = vertices.size();
	float cos_cone = sqrt_2 / sqrtf(sqrt_2 * sqrt_2 + h * h);
	float sin_cone = h / sqrtf(sqrt_2 * sqrt_2 + h * h);

	float x = -.5f;
	float z = .5f;

	for (int i = 0; i < 4; ++i) {
		float angle = glm::radians(i * 90.f);

		float x_n = cos_cone * sinf(angle);
		float z_n = cos_cone * cosf(angle);

		glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

		vertices.push_back({ { x, -h / 2.f, z }, { 0.f, 1.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);

		vertices.push_back({ { x + cosf(angle), -h / 2.f, z + sinf(glm::radians(180.f) + angle) }, { 1.f, 1.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);

		vertices.push_back({ { 0.f, h / 2.f, 0.f }, { .5f, 0.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);

		x += cosf(angle);
		z += sinf(glm::radians(180.f) + angle);
	}

	for (int i = 0; i < 4; ++i) {
		int left = start + i * 3;
		int right = start + i * 3 + 1;
		int top = start + i * 3 + 2;

		indices.push_back(left);
		indices.push_back(right);
		indices.push_back(top);

		std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(left, right, top);

		vertices[left].Tangent += TB.first;
		vertices[left].Bitangent += TB.second;

		vertices[right].Tangent += TB.first;
		vertices[right].Bitangent += TB.second;

		vertices[top].Tangent += TB.first;
		vertices[top].Bitangent += TB.second;
	}

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		vertices[i].Tangent /= (float)trisNum[i];
		vertices[i].Tangent = glm::normalize(vertices[i].Tangent);

		vertices[i].Bitangent /= (float)trisNum[i];
		vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
	}

	trisNum.clear();
}

Pyramid::~Pyramid() {}