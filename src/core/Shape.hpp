#pragma once
#include "Vertex.hpp"

enum class FormatType {
	VECTOR_INDICES_STRUCT = 0,
	ARRAY_INDICES_STRUCT = 1,
	VECTOR_VERTICES_STRUCT = 2,
	ARRAY_VERTICES_STRUCT = 3,
	VECTOR_INDICES_FLOAT = 4,
	ARRAY_INDICES_FLOAT = 5,
	VECTOR_VERTICES_FLOAT = 6,
	ARRAY_VERTICES_FLOAT = 7,
	OBJ = 8
};

enum class ValuesRange {
	HALF_TO_HALF = 0,
	ONE_TO_ONE = 1
};

template<typename T>
static const T& unmove(T&& x)
{
	return x;
}

class Shape
{
protected:
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;

	float _map(const float input, const float currStart, const float currEnd, const float expectedStart, const float expectedEnd) const;

	Vertex _calcTangentBitangent(const unsigned int vertexIndex) const;
	std::pair<glm::vec3, glm::vec3> _calcTangentBitangent(const unsigned int t1, const unsigned int t2, const unsigned int t3) const;
	// start - inclusive, end - exclusive
	void _normalizeTangents(const std::vector<unsigned int>& trisNum, const size_t start, const size_t end);

	std::string _getGeneratedHeader(const std::string commentSign) const;
	std::string _getStructDefinition(bool isC99) const;
	std::string _formatFloat(float value, bool delRedundantZeros=true) const;
	std::string _formatVertex(const Vertex& v, bool useFloat) const;
	std::string _formatVertices(bool onlyVertices, bool useArray, bool useFloat) const;
	std::string _formatIndices(bool useArray) const;
	std::string _toOBJ() const;

public:
	Shape() = default;
	virtual ~Shape();

	std::string toString(FormatType type = FormatType::VECTOR_INDICES_STRUCT) const;

	static std::string getClassName();
	virtual std::string getObjectClassName() const;
	size_t getVerticesCount() const;
	size_t getIndicesCount() const;
};
