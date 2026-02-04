#pragma once
#include "Vertex.hpp"
#include <string>
#include <utility>
#include <vector>
#include <glm/fwd.hpp>

enum class FormatType {
	VECTOR_INDICES_STRUCT = 0,
	ARRAY_INDICES_STRUCT = 1,
	VECTOR_VERTICES_STRUCT = 2,
	ARRAY_VERTICES_STRUCT = 3,
	VECTOR_INDICES_FLOAT = 4,
	ARRAY_INDICES_FLOAT = 5,
	VECTOR_VERTICES_FLOAT = 6,
	ARRAY_VERTICES_FLOAT = 7,
	JSON_INDICES = 8,
	JSON_VERTICES = 9,
	OBJ = 10
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

struct ShapeConfig
{
	bool genTangents = true;
	bool calcBitangents = true;
	bool tangentHandednessPositive = true;
};

class Shape
{
protected:
	ShapeConfig _shapeConfig;
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;

	float _map(const float input, const float currStart, const float currEnd, const float expectedStart, const float expectedEnd) const;

	glm::vec3 _calcTangent(const unsigned int t1, const unsigned int t2, const unsigned int t3) const;
	void _normalizeTangentAndGenerateBitangent(const unsigned int vertIdx, const unsigned int trisNum = 1);
	// start - inclusive, end - exclusive
	void _normalizeTangentsAndGenerateBitangents(const std::vector<unsigned int>& trisNum, const size_t start, const size_t end);

	std::string _getGeneratedHeader(const std::string commentSign) const;
	std::string _getStructDefinition(bool isC99) const;
	std::string _formatFloat(float value, bool delRedundantZeros=true) const;
	std::string _formatVertex(const Vertex& v, bool useFloat) const;
	std::string _formatVertices(bool onlyVertices, bool useArray, bool useFloat) const;
	std::string _formatIndices(bool useArray) const;
	std::string _toJSON(bool onlyVertices) const;
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