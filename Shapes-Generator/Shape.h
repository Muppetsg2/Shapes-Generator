#pragma once
#include "Vertex.h"

enum class FormatType {
	VECTOR_INDICES = 0,
	ARRAY_INDICES = 1,
	VECTOR_VERTICES = 2,
	ARRAY_VERTICES = 3,
	OBJ = 4
};

enum class ValuesRange {
	HALF_TO_HALF = 0,
	ONE_TO_ONE = 1
};

class Shape
{
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	float fmapf(float input, float currStart, float currEnd, float expectedStart, float expectedEnd);

	Vertex calcTangentBitangent(unsigned int vertexIndex);
	std::pair<glm::vec3, glm::vec3> calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3);

	std::string formatFloat(float value, bool delRedundantZeros=true) const;
	std::string toOBJ() const;

public:
	Shape() = default;
	virtual ~Shape();

	std::string toString(FormatType type = FormatType::VECTOR_INDICES) const;

	static std::string getClassName();
	virtual std::string getObjectClassName() const;
	size_t getVerticesCount() const;
	size_t getIndicesCount() const;
};
