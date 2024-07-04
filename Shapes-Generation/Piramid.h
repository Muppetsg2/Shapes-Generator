#pragma once

#include <vector>
#include "MeshStructs.h"
#include <string>

using namespace std;

class Piramid 
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	pair<glm::vec3, glm::vec3> calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3);

public:
	Piramid();

	string getPiramidAsString();
};