// PRECOMPILED HEADER
#include "pch.h"
#include "Tetrahedron.h"

Tetrahedron::Tetrahedron(ValuesRange range)
{
	vertices.clear();
	indices.clear();
	//float sqrt_3 = 1.732050808;
	float sqrt_3 = sqrtf(3.f);

	float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

	unsigned int segments = 3;
	float h = 2.f / 3.f;
	float r = sqrt_3 / 3.f;

	std::vector<unsigned int> trisNum;

	float angleXZDiff = 360.f / (float)segments;

	// CIRCLE BOTTOM
	// VERTICES AND TEX COORDS
	size_t start = vertices.size();
	float y = -h / 2.f;

	float angleXZ = 0.f;
	for (unsigned int j = 0; j < segments; ++j) {
		float radiansXZ = glm::radians(angleXZ);
		float z = cosf(radiansXZ);
		float x = sinf(radiansXZ);
		vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { fabsf(.5f - (float)((int)(j * 1.5f)) * .5f), (j == 0 ? 1.f : 0.f) }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);
		angleXZ += angleXZDiff;
	}

	indices.push_back((unsigned int)start);
	indices.push_back((unsigned int)start + 2);
	indices.push_back((unsigned int)start + 1);

	std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent((unsigned int)start, (unsigned int)start + 2, (unsigned int)start + 1);

	vertices[start].Tangent += TB.first;
	vertices[start].Bitangent += TB.second;

	vertices[start + 2].Tangent += TB.first;
	vertices[start + 2].Bitangent += TB.second;

	vertices[start + 1].Tangent += TB.first;
	vertices[start + 1].Bitangent += TB.second;

	// CONE
	// VERTICES AND TEX COORDS
	start = vertices.size();
	angleXZ = 0.f;
	float cos_cone = r / sqrtf(r * r + h * h);
	float sin_cone = h / sqrtf(r * r + h * h);
	for (unsigned int j = 0; j < segments; ++j) {

		float x_n = cos_cone * (sinf(glm::radians(angleXZ)) + sinf(glm::radians(angleXZ + angleXZDiff))) * .5f;
		float z_n = cos_cone * (cosf(glm::radians(angleXZ)) + cosf(glm::radians(angleXZ + angleXZDiff))) * .5f;

		glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

		for (int i = 0; i < 2; ++i) {
			float angle = angleXZ + i * angleXZDiff;
			float radiansXZ = glm::radians(angle);
			float z = cosf(radiansXZ);
			float x = sinf(radiansXZ);

			vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { (float)i, 1.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
			trisNum.push_back(1);
		}

		// sqrt_3 * sqrtf(2) * .5f - .5f it came out from previous normalization of vertices.
		// In shortcut it is height of Tetrahedron minus y value of normalized vertex
		vertices.push_back({ glm::vec3(0.f, (sqrt_3 * sqrtf(2) * .5f) - .5f, 0.f) * mult, {.5f, 0.f}, norm, glm::vec3(0.f), glm::vec3(0.f)});
		trisNum.push_back(1);

		angleXZ += angleXZDiff;
	}

	// INDICES
	for (unsigned int i = 0; i < segments; ++i) {

		size_t left = start + (size_t)i * 3;
		size_t right = start + (size_t)i * 3 + 1;
		size_t top = start + (size_t)i * 3 + 2;

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

	for (unsigned int i = 0; i < vertices.size(); ++i) {
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

Tetrahedron::~Tetrahedron() {}

std::string Tetrahedron::getClassName()
{
	return "Tetrahedron";
}

std::string Tetrahedron::getObjectClassName() const
{
	return Tetrahedron::getClassName();
}