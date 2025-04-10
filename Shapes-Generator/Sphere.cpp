// PRECOMPILED HEADER
#include "pch.h"
#include "Sphere.h"

void Sphere::_generate(unsigned int h, unsigned int v, ValuesRange range)
{
	float mult = range == ValuesRange::HALF_TO_HALF ? 0.5f : 1.0f;
	float angleYDiff = (float)M_PI / (float)h;
	float angleXZDiff = 2.f * (float)M_PI / (float)v;

	float texHDiff = 1.f / (float)h;
	float texVDiff = 1.f / (float)v;

	std::vector<unsigned int> trisNum;

	// VERTICIES AND NUMBER OF TRIANGLES
	// TOP VERTEX
	_vertices.push_back({ { 0.f, 1.f * mult, 0.f }, { .5f, 0.f }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
	trisNum.push_back(v);

	// TOP HALF AND BOTTOM HALF
	float angleY = angleYDiff;
	for (unsigned int i = 0u; i < h - 1u; ++i) {
		float r = sinf(angleY) * mult;
		float y = cosf(angleY) * mult;

		unsigned int startTexV = i * v + 1u;
		unsigned int t = 1u;

		if (h == 2u) {
			t = 4u;
		}
		else if (h == 3u) {
			t = 5u;
		}
		else if (h >= 4u) {
			if (i == 0u || i + 2u == h) {
				t = 5u;
			}
			else {
				t = 6u;
			}
		}

		// DRAW CIRCLE
		float angleXZ = 0.f;
		for (unsigned int j = 0u; j < v; ++j) {
			float z = r * cosf(angleXZ);
			float x = r * sinf(angleXZ);

			glm::vec3 vert = { x, y, z };
			_vertices.push_back({ vert, { (float)j * texVDiff, texHDiff * (float)(i + 1u) }, glm::normalize(vert), glm::vec3(0.f), glm::vec3(0.f) });
			trisNum.push_back(t);

			if (j == v - 1u) {
				glm::vec3 vertLast = { 0.f, y, r };
				// Add first in the end again for texCoords
				_vertices.push_back({ vertLast, { 1.f , texHDiff * (float)(i + 1u) }, glm::normalize(vertLast), glm::vec3(0.f), glm::vec3(0.f) });
				trisNum.push_back(t);
			}

			angleXZ += angleXZDiff;
		}
		angleY += angleYDiff;
	}

	// BOTTOM VERTEX
	_vertices.push_back({ { 0.f, -1.f * mult, 0.f }, { .5f, 1.f }, { 0.f, -1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
	trisNum.push_back(v);

	// INDICIES, TANGENTS AND BITANGENTS
	std::pair<glm::vec3, glm::vec3> TB;

	// TOP CIRCLE + BOTTOM CIRCLE
	const size_t verticesNum = _vertices.size();
	for (unsigned int i = 0u; i < v; ++i) {
		// TOP CIRCLE
		unsigned int rightVertex = (i + 1u) + 1u;
		unsigned int topVertex = 0u;
		unsigned int leftVertex = i + 1u;

		_indices.push_back(rightVertex);
		_indices.push_back(topVertex);
		_indices.push_back(leftVertex);

		TB = _calcTangentBitangent(rightVertex, topVertex, leftVertex);

		_vertices[rightVertex].Tangent += TB.first;
		_vertices[rightVertex].Bitangent += TB.second;

		_vertices[topVertex].Tangent += TB.first;
		_vertices[topVertex].Bitangent += TB.second;

		_vertices[leftVertex].Tangent += TB.first;
		_vertices[leftVertex].Bitangent += TB.second;

		// BOTTOM CIRCLE
		rightVertex = (unsigned int)verticesNum - 2u - v - 1u + (i + 1u) + 1u;
		leftVertex = (unsigned int)verticesNum - 2u - v - 1u + i + 1u;
		topVertex = (unsigned int)verticesNum - 1u;

		_indices.push_back(rightVertex);
		_indices.push_back(leftVertex);
		_indices.push_back(topVertex);

		TB = _calcTangentBitangent(rightVertex, leftVertex, topVertex);

		_vertices[rightVertex].Tangent += TB.first;
		_vertices[rightVertex].Bitangent += TB.second;

		_vertices[leftVertex].Tangent += TB.first;
		_vertices[leftVertex].Bitangent += TB.second;

		_vertices[topVertex].Tangent += TB.first;
		_vertices[topVertex].Bitangent += TB.second;
	}

	// CENTER CIRCLES
	for (unsigned int c = 0u; c < h - 2u; ++c) {
		unsigned int startV = c * (v + 1u) + 1u;
		for (unsigned int i = 0u; i < v; ++i) {
			unsigned int topLeft = i + startV;
			unsigned int topRight = (i + 1u) + startV;
			unsigned int bottomLeft = i + v + 1u + startV;
			unsigned int bottomRight = (i + 1u) + v + 1u + startV;

			_indices.push_back(topRight);
			_indices.push_back(topLeft);
			_indices.push_back(bottomLeft);

			TB = _calcTangentBitangent(topRight, topLeft, bottomLeft);

			_vertices[topRight].Tangent += TB.first;
			_vertices[topRight].Bitangent += TB.second;

			_vertices[topLeft].Tangent += TB.first;
			_vertices[topLeft].Bitangent += TB.second;

			_vertices[bottomLeft].Tangent += TB.first;
			_vertices[bottomLeft].Bitangent += TB.second;

			_indices.push_back(bottomRight);
			_indices.push_back(topRight);
			_indices.push_back(bottomLeft);

			TB = _calcTangentBitangent(bottomRight, topRight, bottomLeft);

			_vertices[bottomRight].Tangent += TB.first;
			_vertices[bottomRight].Bitangent += TB.second;

			_vertices[topRight].Tangent += TB.first;
			_vertices[topRight].Bitangent += TB.second;

			_vertices[bottomLeft].Tangent += TB.first;
			_vertices[bottomLeft].Bitangent += TB.second;
		}
	}

	_normalizeTangents(trisNum, 0ull, verticesNum);

	trisNum.clear();
}

Sphere::Sphere(unsigned int h, unsigned int v, ValuesRange range)
{
	_vertices.clear();
	_indices.clear();
	_generate(std::max(2u, h), std::max(3u, v), range);
}

Sphere::~Sphere() {}

std::string Sphere::getClassName()
{
	return "Sphere";
}

std::string Sphere::getObjectClassName() const
{
	return Sphere::getClassName();
}