#pragma once

#include "MeshStructs.h"
#include <vector>
#include <string>

using namespace std;

class Sphere {
private:
	unsigned int _segmentsHorizontal = 2;
	unsigned int _segmentsVertical = 3;

	vector<Vertex> vertices;
	vector<unsigned int> indices;

public:
	Sphere();
	Sphere(unsigned int h, unsigned int v);
	virtual ~Sphere();

	void setSegmentsHorizontal(unsigned int segmentsHorizontal);
	void setSegmentsVertical(unsigned int segmentsVertical);

	string getSphereAsString();

private:
	void updateVerticiesAndIndices();
	std::pair<glm::vec3, glm::vec3> calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3);
};