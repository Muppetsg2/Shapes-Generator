#include "pch.hpp"
#include "Sphere.hpp"
#include "Shape.hpp"
#include "Config.hpp"
#include "Constants.hpp"
#include "Vertex.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

using namespace config;

glm::vec3 Sphere::_getAverageNormal(const glm::vec3 n1, const glm::vec3 n2, const glm::vec3 n3) const
{
	const glm::vec3 average = n1 + n2 + n3;
	return fabsf(glm::length(average)) >= EPSILON ? glm::normalize(average) : average;
}

void Sphere::_generate(const unsigned int h, const unsigned int v, const ValuesRange range, const bool useFlatShading)
{
	const Config& config = get_config();
	const float mult = range == ValuesRange::HALF_TO_HALF ? 0.5f : 1.0f;
	const float angleYDiff = (float)M_PI / (float)h;
	const float angleXZDiff = 2.f * (float)M_PI / (float)v;

	const float texHDiff = 1.f / (float)h;
	const float texVDiff = 1.f / (float)v;

	std::vector<unsigned int> trisNum;

	// VERTICIES AND NUMBER OF TRIANGLES
	// TOP VERTEX
	_vertices.push_back({ { 0.f, 1.f * mult, 0.f }, { .5f, 0.f }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
	if (config.genTangents) trisNum.push_back(v);

	// TOP HALF AND BOTTOM HALF
	float angleY = angleYDiff;
	for (unsigned int i = 0u; i < h - 1u; ++i) {
		const float r = sinf(angleY) * mult;
		const float y = cosf(angleY) * mult;

		const unsigned int startTexV = i * v + 1u;
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
			const float z = r * cosf(angleXZ);
			const float x = r * sinf(angleXZ);

			const glm::vec3 vert = { x, y, z };
			_vertices.push_back({ vert, { (float)j * texVDiff, texHDiff * (float)(i + 1u) }, glm::normalize(vert), glm::vec3(0.f), glm::vec3(0.f) });
			if (config.genTangents) trisNum.push_back(t);

			if (j == v - 1u) {
				const glm::vec3 vertLast = { 0.f, y, r };
				// Add first in the end again for texCoords
				_vertices.push_back({ vertLast, { 1.f , texHDiff * (float)(i + 1u) }, glm::normalize(vertLast), glm::vec3(0.f), glm::vec3(0.f) });
				if (config.genTangents) trisNum.push_back(t);
			}

			angleXZ += angleXZDiff;
		}
		angleY += angleYDiff;
	}

	// BOTTOM VERTEX
	_vertices.push_back({ { 0.f, -1.f * mult, 0.f }, { .5f, 1.f }, { 0.f, -1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
	if (config.genTangents) trisNum.push_back(v);

	// INDICIES, TANGENTS AND BITANGENTS
	glm::vec3 tangent;

	if (useFlatShading) {
		std::vector<Vertex> tempVertices(_vertices);
		_vertices.clear();

		// TOP CIRCLE + BOTTOM CIRCLE
		const size_t verticesNum = tempVertices.size();
		for (unsigned int i = 0u; i < v; ++i) {
			const size_t start = _vertices.size();
			unsigned int first = (unsigned int)start;
			unsigned int second = (unsigned int)start + 1u;
			unsigned int third = (unsigned int)start + 2u;
			for (int s = 0; s < 2; ++s) {
				// 0 - TOP CIRCLE
				// 1 - BOTTOM CIRCLE
				const bool isTop = s == 0;
				const unsigned int rightVertex = isTop ? (i + 1u) + 1u : (unsigned int)verticesNum - 2u - v - 1u + (i + 1u) + 1u;
				const unsigned int topVertex = isTop ? 0u : (unsigned int)verticesNum - 1u;
				const unsigned int leftVertex = isTop ? i + 1u : (unsigned int)verticesNum - 2u - v - 1u + i + 1u;

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

				if (config.genTangents) {
					tangent = _calcTangent(first, second, third);

					_vertices[first].Tangent = tangent;
					_normalizeTangentAndGenerateBitangent(first);

					_vertices[second].Tangent = tangent;
					_normalizeTangentAndGenerateBitangent(second);

					_vertices[third].Tangent = tangent;
					_normalizeTangentAndGenerateBitangent(third);
				}

				first += 3u;
				second += 3u;
				third += 3u;
			}
		}

		// CENTER CIRCLES
		for (unsigned int c = 0u; c < h - 2u; ++c) {
			const unsigned int startV = c * (v + 1u) + 1u;
			for (unsigned int i = 0u; i < v; ++i) {
				const unsigned int topLeft = i + startV;
				const unsigned int topRight = (i + 1u) + startV;
				const unsigned int bottomLeft = i + v + 1u + startV;
				const unsigned int bottomRight = (i + 1u) + v + 1u + startV;
				const size_t start = _vertices.size();
				unsigned int first = (unsigned int)start;
				unsigned int second = (unsigned int)start + 1u;
				unsigned int third = (unsigned int)start + 2u;

				for (int s = 0; s < 2; ++s) {
					const bool isFirst = s == 0;
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

					if (config.genTangents) {
						tangent = _calcTangent(first, second, third);

						_vertices[first].Tangent = tangent;
						_normalizeTangentAndGenerateBitangent(first);

						_vertices[second].Tangent = tangent;
						_normalizeTangentAndGenerateBitangent(second);

						_vertices[third].Tangent = tangent;
						_normalizeTangentAndGenerateBitangent(third);
					}

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

			if (config.genTangents) {
				tangent = _calcTangent(rightVertex, topVertex, leftVertex);

				_vertices[rightVertex].Tangent += tangent;
				_vertices[topVertex].Tangent += tangent;
				_vertices[leftVertex].Tangent += tangent;
			}

			// BOTTOM CIRCLE
			rightVertex = (unsigned int)verticesNum - 2u - v - 1u + (i + 1u) + 1u;
			leftVertex = (unsigned int)verticesNum - 2u - v - 1u + i + 1u;
			topVertex = (unsigned int)verticesNum - 1u;

			_indices.push_back(rightVertex);
			_indices.push_back(leftVertex);
			_indices.push_back(topVertex);

			if (config.genTangents) {
				tangent = _calcTangent(rightVertex, leftVertex, topVertex);

				_vertices[rightVertex].Tangent += tangent;
				_vertices[leftVertex].Tangent += tangent;
				_vertices[topVertex].Tangent += tangent;
			}
		}

		// CENTER CIRCLES
		for (unsigned int c = 0u; c < h - 2u; ++c) {
			const unsigned int startV = c * (v + 1u) + 1u;
			for (unsigned int i = 0u; i < v; ++i) {
				const unsigned int topLeft = i + startV;
				const unsigned int topRight = (i + 1u) + startV;
				const unsigned int bottomLeft = i + v + 1u + startV;
				const unsigned int bottomRight = (i + 1u) + v + 1u + startV;

				// first triangle
				_indices.push_back(topRight);
				_indices.push_back(topLeft);
				_indices.push_back(bottomLeft);

				// second triangle
				_indices.push_back(bottomRight);
				_indices.push_back(topRight);
				_indices.push_back(bottomLeft);

				if (config.genTangents) {
					// first triangle
					tangent = _calcTangent(topRight, topLeft, bottomLeft);

					_vertices[topRight].Tangent += tangent;
					_vertices[topLeft].Tangent += tangent;
					_vertices[bottomLeft].Tangent += tangent;

					// second triangle
					tangent = _calcTangent(bottomRight, topRight, bottomLeft);

					_vertices[bottomRight].Tangent += tangent;
					_vertices[topRight].Tangent += tangent;
					_vertices[bottomLeft].Tangent += tangent;
				}
			}
		}

		if (config.genTangents) _normalizeTangentsAndGenerateBitangents(trisNum, 0ull, verticesNum);
	}

	trisNum.clear();
}

Sphere::Sphere(const unsigned int h, const unsigned int v, const SphereShading shading, const ValuesRange range)
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