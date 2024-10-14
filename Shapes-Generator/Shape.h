#pragma once

#include <vector>
#include "Vertex.h"
#include <string>

using namespace std;

class Shape
{
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Vertex calcTangentBitangent(unsigned int vertexIndex);
	pair<glm::vec3, glm::vec3> calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3);

public:
	Shape() = default;
	virtual ~Shape();

	string toString() const;

	size_t getVerticesCount() const;

	size_t getIndicesCount() const;
};
