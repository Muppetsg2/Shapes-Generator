#pragma once

#include <vector>
#include "MeshStructs.h"
#include <string>

using namespace std;

class Hexagon 
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	pair<glm::vec3, glm::vec3> calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3);

	void generateCircle(unsigned int segments, float y, unsigned int cullFace);

public:
	Hexagon();

	string getHexagonAsString();
};