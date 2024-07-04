#include "Tetrahedron.h"
#include <format>

pair<glm::vec3, glm::vec3> Tetrahedron::calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3)
{
	std::pair<glm::vec3, glm::vec3> TB;

	Vertex v0 = vertices[t1];
	Vertex v1 = vertices[t2];
	Vertex v2 = vertices[t3];

	glm::vec3 pos0 = v0.Position;
	glm::vec3 pos1 = v1.Position;
	glm::vec3 pos2 = v2.Position;

	glm::vec2 uv0 = v0.TexCoord;
	glm::vec2 uv1 = v1.TexCoord;
	glm::vec2 uv2 = v2.TexCoord;

	glm::vec3 delta_pos1 = pos1 - pos0;
	glm::vec3 delta_pos2 = pos2 - pos0;

	glm::vec2 delta_uv1 = uv1 - uv0;
	glm::vec2 delta_uv2 = uv2 - uv0;

	float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);

	// Save the results
	TB.first = (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;
	TB.second = (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r;

	return TB;
}

Tetrahedron::Tetrahedron()
{
	//float sqrt_3 = 1.732050808;
	float sqrt_3 = sqrtf(3.f);

	float segments = 3;
	float h = 2.f / 3.f;
	float r = sqrt_3 / 3.f;

	std::vector<unsigned int> trisNum;

	float angleXZDiff = 360.f / (float)segments;

	// CIRCLE BOTTOM
	// VERTICES AND TEX COORDS
	unsigned int start = vertices.size();
	float y = -h / 2.f;

	float angleXZ = 0.f;
	for (unsigned int j = 0; j < segments; ++j) {
		float radiansXZ = glm::radians(angleXZ);
		float z = cosf(radiansXZ);
		float x = sinf(radiansXZ);
		vertices.push_back({ { x * r, y, z * r }, { fabsf(.5f - (float)((int)(j * 1.5f)) * .5f), (j == 0 ? 1.f : 0.f) }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);
		angleXZ += angleXZDiff;
	}

	indices.push_back(start);
	indices.push_back(start + 2);
	indices.push_back(start + 1);

	std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(start, start + 2, start + 1);

	vertices[start].Tangent += TB.first;
	vertices[start].Bitangent += TB.second;

	vertices[start + 2].Tangent += TB.first;
	vertices[start + 2].Bitangent += TB.second;

	vertices[start + 1].Tangent += TB.first;
	vertices[start + 1].Bitangent += TB.second;

	// CONE
	// VERTICES AND TEX COORDS
	start = vertices.size();
	angleXZ = 0.f;
	float cos_cone = r / sqrtf(r * r + h * h);
	float sin_cone = h / sqrtf(r * r + h * h);
	for (unsigned int j = 0; j < segments; ++j) {

		float x_n = cos_cone * (sinf(glm::radians(angleXZ)) + sinf(glm::radians(angleXZ + angleXZDiff))) / 2.f;
		float z_n = cos_cone * (cosf(glm::radians(angleXZ)) + cosf(glm::radians(angleXZ + angleXZDiff))) / 2.f;

		glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

		for (int i = 0; i < 2; ++i) {
			float angle = angleXZ + i * angleXZDiff;
			float radiansXZ = glm::radians(angle);
			float z = cosf(radiansXZ);
			float x = sinf(radiansXZ);

			vertices.push_back({ { x * r, y, z * r }, { (float)i, 1.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
			trisNum.push_back(1);
		}

		vertices.push_back({ { 0.f, -y, 0.f }, { .5f, 0.f }, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1);

		angleXZ += angleXZDiff;
	}

	// INDICES
	for (unsigned int i = 0; i < segments; ++i) {

		int left = start + i * 3;
		int right = start + i * 3 + 1;
		int top = start + i * 3 + 2;

		indices.push_back(left);
		indices.push_back(right);
		indices.push_back(top);

		std::pair<glm::vec3, glm::vec3> TB = calcTangentBitangent(left, right, top);

		vertices[left].Tangent += TB.first;
		vertices[left].Bitangent += TB.second;

		vertices[right].Tangent += TB.first;
		vertices[right].Bitangent += TB.second;

		vertices[top].Tangent += TB.first;
		vertices[top].Bitangent += TB.second;
	}

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		vertices[i].Tangent /= (float)trisNum[i];
		vertices[i].Tangent = glm::normalize(vertices[i].Tangent);

		vertices[i].Bitangent /= (float)trisNum[i];
		vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
	}

	trisNum.clear();
}

string Tetrahedron::getTetrahedronAsString()
{
	string text = "std::vector<Vertex> vertices {\n";

	for (int i = 0; i < vertices.size(); ++i) {
		Vertex v = vertices[i];

		text += vformat(string_view("\t{} {}{:f}f, {:f}f, {:f}f{}, {}{:f}f, {:f}f{}, {}{:f}f, {:f}f, {:f}f{}, {}{:f}f, {:f}f, {:f}f{}, {}{:f}f, {:f}f, {:f}f{} {}"),
			make_format_args
			(
				"{", ".Position = glm::vec3(",
				v.Position.x, v.Position.y, v.Position.z,
				")", ".TexCoords = glm::vec2(",
				v.TexCoord.x, v.TexCoord.y,
				")", ".Normal = glm::vec3(",
				v.Normal.x, v.Normal.y, v.Normal.z,
				")", ".Tangent = glm::vec3(",
				v.Tangent.x, v.Tangent.y, v.Tangent.z,
				")", ".Bitangent = glm::vec3(",
				v.Bitangent.x, v.Bitangent.y, v.Bitangent.z,
				")", "}"
			)
		);

		if (i + 1 < vertices.size()) {
			text += ",\n";
		}
		else {
			text += "\n";
		}
	}

	text += "};\n\nstd::vector<unsigned int> indices = {\n";

	for (int i = 0; i < indices.size(); i += 3) {
		text += vformat
		(
			string_view("\t{0}, {1}, {2}"),
			make_format_args
			(
				indices[i],
				indices[i + 1],
				indices[i + 2]
			)
		);

		if (i + 3 < indices.size()) {
			text += ",\n";
		}
		else {
			text += "\n";
		}
	}

	text += "};";

	return text;
}
