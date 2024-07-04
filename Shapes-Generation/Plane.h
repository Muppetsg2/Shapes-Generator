#pragma once

#include <vector>
#include "MeshStructs.h"
#include <string>

using namespace std;

enum PlaneNormalDir {
	UP = 0,
	FRONT = 1
};

class Plane {
private:
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	void generate(unsigned int rows, unsigned int columns, PlaneNormalDir dir);

	float fmapf(float input, float currStart, float currEnd, float expectedStart, float expectedEnd);

	Vertex calcTangentBitangent(unsigned int vertexIndex);

	pair<glm::vec3, glm::vec3> calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3);

public:
	Plane(unsigned int rows = 2, unsigned int columns = 2, PlaneNormalDir dir = PlaneNormalDir::UP);

	string getPlaneAsString();
};