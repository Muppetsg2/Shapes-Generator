#include "Sphere.h"

void Sphere::updateVerticiesAndIndices()
{
	vertices.clear();
	indices.clear();

	float angleYDiff = 180.f / (float)_segmentsHorizontal;
	float angleXZDiff = 360.f / (float)_segmentsVertical;

	float texHDiff = 1.f / (float)_segmentsHorizontal;
	float texVDiff = 1.f / (float)_segmentsVertical;

	std::vector<unsigned int> trisNum = std::vector<unsigned int>();

	// VERTICIES AND NUMBER OF TRIANGLES
	// TOP VERTEX
	vertices.push_back({ { 0.f, 1.f, 0.f }, { .5f, 0.f }, { 0.f, 1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
	trisNum.push_back(_segmentsVertical);
	// TOP HALF AND BOTTOM HALF
	float angleY = angleYDiff;
	for (unsigned int i = 0; i < _segmentsHorizontal - 1; ++i) {
		float radiansY = glm::radians(angleY);
		float r = sinf(radiansY);
		float y = cosf(radiansY);

		unsigned int startTexV = i * _segmentsVertical + 1;
		unsigned int t = 1;

		if (_segmentsHorizontal == 2) {
			t = 4;
		}
		else if (_segmentsHorizontal == 3) {
			t = 5;
		}
		else if (_segmentsHorizontal >= 4) {
			if (i == 0 || i + 2 == _segmentsHorizontal) {
				t = 5;
			}
			else {
				t = 6;
			}
		}

		// DRAW CIRCLE
		float angleXZ = 0.f;
		for (unsigned int j = 0; j < _segmentsVertical; ++j) {
			float radiansXZ = glm::radians(angleXZ);
			float z = r * cosf(radiansXZ);
			float x = r * sinf(radiansXZ);

			glm::vec3 vert = { x, y, z };
			vertices.push_back({ vert, { j * texVDiff, texHDiff * (i + 1) }, glm::normalize(vert), glm::vec3(0.f), glm::vec3(0.f) });
			trisNum.push_back(t);

			if (j == _segmentsVertical - 1)
			{
				glm::vec3 vertLast = { r * sinf(glm::radians(0.f)), y, r * cosf(glm::radians(0.f)) };
				// Add first in the end again for texCoords
				vertices.push_back({ vertLast, { 1.f , texHDiff * (i + 1) }, glm::normalize(vertLast), glm::vec3(0.f), glm::vec3(0.f) });
				trisNum.push_back(t);
			}

			angleXZ += angleXZDiff;
		}
		angleY += angleYDiff;
	}
	// BOTTOM VERTEX
	vertices.push_back({ { 0.f, -1.f, 0.f }, { .5f, 1.f }, { 0.f, -1.f, 0.f }, glm::vec3(0.f), glm::vec3(0.f) });
	trisNum.push_back(_segmentsVertical);

	// INDICIES, TANGENTS AND BITANGENTS
	std::pair<glm::vec3, glm::vec3> TB;

	size_t verticesNum = vertices.size();
	// TOP CIRCLE + BOTTOM CIRCLE
	for (unsigned int i = 0; i < _segmentsVertical; ++i) {
		// TOP CIRCLE
		unsigned int rightVertex = (i + 1) + 1;
		unsigned int topVertex = 0;
		unsigned int leftVertex = i + 1;

		indices.push_back(rightVertex);
		indices.push_back(topVertex);
		indices.push_back(leftVertex);

		TB = calcTangentBitangent(rightVertex, topVertex, leftVertex);

		vertices[rightVertex].Tangent += TB.first;
		vertices[rightVertex].Bitangent += TB.second;

		vertices[topVertex].Tangent += TB.first;
		vertices[topVertex].Bitangent += TB.second;

		vertices[leftVertex].Tangent += TB.first;
		vertices[leftVertex].Bitangent += TB.second;

		// BOTTOM CIRCLE
		rightVertex = verticesNum - 2 - _segmentsVertical - 1 + (i + 1) + 1;
		leftVertex = verticesNum - 2 - _segmentsVertical - 1 + i + 1;
		topVertex = verticesNum - 1;

		indices.push_back(rightVertex);
		indices.push_back(leftVertex);
		indices.push_back(topVertex);

		TB = calcTangentBitangent(rightVertex, leftVertex, topVertex);

		vertices[rightVertex].Tangent += TB.first;
		vertices[rightVertex].Bitangent += TB.second;

		vertices[leftVertex].Tangent += TB.first;
		vertices[leftVertex].Bitangent += TB.second;

		vertices[topVertex].Tangent += TB.first;
		vertices[topVertex].Bitangent += TB.second;
	}

	// CENTER CIRCLES
	for (unsigned int c = 0; c < _segmentsHorizontal - 2; ++c) {
		unsigned int startV = c * (_segmentsVertical + 1) + 1;
		for (unsigned int i = 0; i < _segmentsVertical; ++i) {
			unsigned int topLeft = i + startV;
			unsigned int topRight = (i + 1) + startV;
			unsigned int bottomLeft = i + _segmentsVertical + 1 + startV;
			unsigned int bottomRight = (i + 1) + _segmentsVertical + 1 + startV;

			indices.push_back(topRight);
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);

			TB = calcTangentBitangent(topRight, topLeft, bottomLeft);

			vertices[topRight].Tangent += TB.first;
			vertices[topRight].Bitangent += TB.second;

			vertices[topLeft].Tangent += TB.first;
			vertices[topLeft].Bitangent += TB.second;

			vertices[bottomLeft].Tangent += TB.first;
			vertices[bottomLeft].Bitangent += TB.second;

			indices.push_back(bottomRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);

			TB = calcTangentBitangent(bottomRight, topRight, bottomLeft);

			vertices[bottomRight].Tangent += TB.first;
			vertices[bottomRight].Bitangent += TB.second;

			vertices[topRight].Tangent += TB.first;
			vertices[topRight].Bitangent += TB.second;

			vertices[bottomLeft].Tangent += TB.first;
			vertices[bottomLeft].Bitangent += TB.second;
		}
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

Sphere::Sphere()
{
	this->updateVerticiesAndIndices();
}

Sphere::Sphere(unsigned int h, unsigned int v)
{
	_segmentsHorizontal = h >= 2 ? h : 2;
	_segmentsVertical = v >= 3 ? v : 3;

	this->updateVerticiesAndIndices();
}

Sphere::~Sphere() {}

void Sphere::setSegmentsHorizontal(unsigned int segmentsHorizontal)
{
	if (segmentsHorizontal >= 2 && _segmentsHorizontal != segmentsHorizontal) {
		_segmentsHorizontal = segmentsHorizontal;
		updateVerticiesAndIndices();
	}
}

void Sphere::setSegmentsVertical(unsigned int segmentsVertical)
{
	if (segmentsVertical >= 3 && _segmentsVertical != segmentsVertical) {
		_segmentsVertical = segmentsVertical;
		updateVerticiesAndIndices();
	}
}