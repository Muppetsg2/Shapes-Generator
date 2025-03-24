// PRECOMPILED HEADER
#include "pch.h"
#include "Cone.h"

void Cone::generate(unsigned int segments, float height, float radius, ValuesRange range, bool useFlatShading)
{
	float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

	float h = height < 1e-8f ? 1.f : height;
	float r = radius < 1e-8f ? 1.f : radius;

	std::vector<unsigned int> trisNum;

	float angleXZDiff = 360.f / (float)segments;

	// CIRCLE BOTTOM
	// VERTICES AND TEX COORDS
	size_t start = vertices.size();
	float rToH = r / h;
	float y = -sqrtf(rToH);
	r = sqrtf(rToH);

	float angleXZ = 0.f;
	for (unsigned int j = 0; j < segments; ++j) {
		float radiansXZ = glm::radians(angleXZ);
		float z = cosf(radiansXZ);
		float x = sinf(radiansXZ);
		vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { .5f + x * .5f, .5f + z * .5f }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(2);
		angleXZ += angleXZDiff;
	}
	vertices.push_back({ glm::vec3(0.f, vertices[vertices.size() - 1].Position.y, 0.f) * mult, {.5f, .5f}, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f)});
	trisNum.push_back(segments);

	// INDICES
	for (size_t i = start; i < vertices.size() - 1ull; ++i) {

		size_t right = i + 2ull == vertices.size() ? start : i + 1ull;

		indices.push_back((unsigned int)right);
		indices.push_back((unsigned int)i);
		indices.push_back((unsigned int)vertices.size() - 1);

		std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent((unsigned int)right, (unsigned int)i, (unsigned int)vertices.size() - 1);

		vertices[right].Tangent += TB.first;
		vertices[right].Bitangent += TB.second;

		vertices[i].Tangent += TB.first;
		vertices[i].Bitangent += TB.second;

		vertices[vertices.size() - 1].Tangent += TB.first;
		vertices[vertices.size() - 1].Bitangent += TB.second;
	}

	// CONE
	// VERTICES AND TEX COORDS
	start = vertices.size();
	angleXZ = 0.f;
	glm::vec3 vr = glm::vec3(r, 0.f, 0.f);
	glm::vec3 vh = glm::vec3(0.f, -y * 2.f, 0.f);
	glm::vec3 vp = glm::normalize(glm::cross(vh - vr, glm::cross(vr, vh)));
	float sin_cone = vp.y;
	float cos_cone = vp.x;

	for (unsigned int j = 0; j < segments + (useFlatShading ? 0 : 1); ++j) {

		if (useFlatShading) {
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

			vertices.push_back({ glm::normalize(glm::vec3(0.f, -y, 0.f)) * mult, {.5f, 0.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
			trisNum.push_back(1);
		}
		else {
			float radiansXZ = glm::radians(angleXZ);
			float x = sinf(radiansXZ);
			float z = cosf(radiansXZ);

			glm::vec3 norm = glm::normalize(glm::vec3(cos_cone * x, sin_cone, cos_cone * z));

			constexpr float uC = .5f;
			constexpr float vC = .25f;
			constexpr float radiansUV0 = glm::radians(135.f);
			float uvR = sqrtf(2.f) * .5f;

			float radiansUV = glm::radians(fmapf(angleXZ, 0.f, 360.f, 0.f, 90.f));
			float u = uC + uvR * std::cosf(radiansUV0 - radiansUV);
			float v = vC + uvR * std::sinf(radiansUV0 - radiansUV);

			vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { u, v }, norm, glm::vec3(0.f), glm::vec3(0.f) });

			if (j == 0 || j == segments) {
				trisNum.push_back(1);
			}
			else {
				trisNum.push_back(2);
			}
		}

		angleXZ += angleXZDiff;
	}

	if (!useFlatShading) {
		vertices.push_back({ glm::normalize(glm::vec3(0.f, -y, 0.f)) * mult, { .5f, 0.f }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(segments);
	}

	// INDICES
	for (unsigned int i = 0; i < segments; ++i) {

		size_t m = useFlatShading ? 3 : 1;

		size_t left = start + (size_t)i * m;
		size_t right = start + (size_t)i * m + 1;
		size_t top = useFlatShading ? start + (size_t)i * m + 2 : vertices.size() - 1;

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

Cone::Cone(unsigned int segments, float height, float radius, ConeShading shading, ValuesRange range)
{
	vertices.clear();
	indices.clear();
	generate(std::max(3u, segments), std::max(1e-6f, height), std::max(1e-6f, radius), range, shading == ConeShading::FLAT);
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