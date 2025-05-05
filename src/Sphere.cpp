#include "Sphere.hpp"

glm::vec3 Sphere::_getAverageNormal(glm::vec3 n1, glm::vec3 n2, glm::vec3 n3)
{
	glm::vec3 average = n1 + n2 + n3;
	return fabsf(glm::length(average)) >= EPSILON ? glm::normalize(average) : average;
}

void Sphere::_generate(unsigned int h, unsigned int v, ValuesRange range, bool useFlatShading)
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

	if (useFlatShading) {
		std::vector<Vertex> tempVertices(_vertices);
		_vertices.clear();

		// TOP CIRCLE + BOTTOM CIRCLE
		const size_t verticesNum = tempVertices.size();
		for (unsigned int i = 0u; i < v; ++i) {
			size_t start = _vertices.size();
			unsigned int first = (unsigned int)start;
			unsigned int second = (unsigned int)start + 1u;
			unsigned int third = (unsigned int)start + 2u;
			for (int s = 0; s < 2; ++s) {
				// 0 - TOP CIRCLE
				// 1 - BOTTOM CIRCLE
				bool isTop = s == 0;
				unsigned int rightVertex = isTop ? (i + 1u) + 1u : (unsigned int)verticesNum - 2u - v - 1u + (i + 1u) + 1u;
				unsigned int topVertex = isTop ? 0u : (unsigned int)verticesNum - 1u;
				unsigned int leftVertex = isTop ? i + 1u : (unsigned int)verticesNum - 2u - v - 1u + i + 1u;

				Vertex v1 = tempVertices[rightVertex];
				Vertex v2 = tempVertices[topVertex];
				Vertex v3 = tempVertices[leftVertex];

				if (!isTop) std::swap(v2, v3);

				glm::vec3 normal = _getAverageNormal(v1.Normal, v2.Normal, v3.Normal);
				v1.Normal = normal;
				v2.Normal = normal;
				v3.Normal = normal;

				_vertices.push_back(v1);
				_vertices.push_back(v2);
				_vertices.push_back(v3);

				_indices.push_back(first);
				_indices.push_back(second);
				_indices.push_back(third);

				TB = _calcTangentBitangent(first, second, third);

				_vertices[first].Tangent = TB.first;
				_vertices[first].Bitangent = TB.second;

				_vertices[second].Tangent = TB.first;
				_vertices[second].Bitangent = TB.second;

				_vertices[third].Tangent = TB.first;
				_vertices[third].Bitangent = TB.second;

				first += 3u;
				second += 3u;
				third += 3u;
			}
		}

		// CENTER CIRCLES
		for (unsigned int c = 0u; c < h - 2u; ++c) {
			unsigned int startV = c * (v + 1u) + 1u;
			for (unsigned int i = 0u; i < v; ++i) {
				unsigned int topLeft = i + startV;
				unsigned int topRight = (i + 1u) + startV;
				unsigned int bottomLeft = i + v + 1u + startV;
				unsigned int bottomRight = (i + 1u) + v + 1u + startV;
				size_t start = _vertices.size();
				unsigned int first = (unsigned int)start;
				unsigned int second = (unsigned int)start + 1u;
				unsigned int third = (unsigned int)start + 2u;

				for (int s = 0; s < 2; ++s) {
					bool isFirst = s == 0;
					Vertex v1 = isFirst ? tempVertices[topRight] : tempVertices[bottomRight];
					Vertex v2 = isFirst ? tempVertices[topLeft] : tempVertices[topRight];
					Vertex v3 = tempVertices[bottomLeft];

					glm::vec3 normal = _getAverageNormal(v1.Normal, v2.Normal, v3.Normal);
					v1.Normal = normal;
					v2.Normal = normal;
					v3.Normal = normal;

					_vertices.push_back(v1);
					_vertices.push_back(v2);
					_vertices.push_back(v3);

					_indices.push_back(first);
					_indices.push_back(second);
					_indices.push_back(third);

					TB = _calcTangentBitangent(first, second, third);

					_vertices[first].Tangent = TB.first;
					_vertices[first].Bitangent = TB.second;

					_vertices[second].Tangent = TB.first;
					_vertices[second].Bitangent = TB.second;

					_vertices[third].Tangent = TB.first;
					_vertices[third].Bitangent = TB.second;

					first += 3u;
					second += 3u;
					third += 3u;
				}
			}
		}
	}
	else {
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
	}

	trisNum.clear();
}

Sphere::Sphere(unsigned int h, unsigned int v, SphereShading shading, ValuesRange range)
{
	_vertices.clear();
	_indices.clear();
	_generate(std::max(2u, h), std::max(3u, v), range, shading == SphereShading::FLAT);
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