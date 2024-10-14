#pragma once

#include <vector>
#include "Vertex.h"
#include <string>
#include <utility>

enum ArrayType {
	VECTOR = 0,
	ARRAY = 1
};

class Shape
{
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Vertex calcTangentBitangent(unsigned int vertexIndex);
	std::pair<glm::vec3, glm::vec3> calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3);

public:
	Shape() = default;
	virtual ~Shape();

	std::string toString(ArrayType type = ArrayType::VECTOR) const;

	size_t getVerticesCount() const;

	size_t getIndicesCount() const;
};
