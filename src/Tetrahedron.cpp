#include "Tetrahedron.hpp"

void Tetrahedron::_generate(ValuesRange range)
{
	float mult = range == ValuesRange::HALF_TO_HALF ? .5f : 1.f;

	unsigned int segments = 3u;
	float h = 2.f / 3.f;
	float r = (float)M_SQRT3 / 3.f;

	std::vector<unsigned int> trisNum;

	float angleXZDiff = 2.f * (float)M_PI / (float)segments;

	// CIRCLE BOTTOM
	// VERTICES AND TEX COORDS
	float y = -h * 0.5f;
	float angleXZ = 0.f;
	for (unsigned int j = 0u; j < segments; ++j) {
		float z = cosf(angleXZ);
		float x = sinf(angleXZ);
		_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { fabsf(.5f - (float)((int)((float)j * 1.5f)) * .5f), (j == 0u ? 1.f : 0.f) }, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1u);
		angleXZ += angleXZDiff;
	}

	_indices.push_back(0u);
	_indices.push_back(2u);
	_indices.push_back(1u);

	std::pair<glm::vec3, glm::vec3> TB = _calcTangentBitangent(0u, 2u, 1u);

	_vertices[0ull].Tangent += TB.first;
	_vertices[0ull].Bitangent += TB.second;

	_vertices[2ull].Tangent += TB.first;
	_vertices[2ull].Bitangent += TB.second;

	_vertices[1ull].Tangent += TB.first;
	_vertices[1ull].Bitangent += TB.second;

	// CONE
	// VERTICES AND TEX COORDS
	size_t start = _vertices.size();
	angleXZ = 0.f;
	float cos_cone = r / sqrtf(r * r + h * h);
	float sin_cone = h / sqrtf(r * r + h * h);
	for (unsigned int j = 0u; j < segments; ++j) {
		float x_n = cos_cone * (sinf(angleXZ) + sinf(angleXZ + angleXZDiff)) * .5f;
		float z_n = cos_cone * (cosf(angleXZ) + cosf(angleXZ + angleXZDiff)) * .5f;

		glm::vec3 norm = glm::normalize(glm::vec3(x_n, sin_cone, z_n));

		for (int i = 0; i < 2; ++i) {
			float angle = angleXZ + (float)i * angleXZDiff;
			float z = cosf(angle);
			float x = sinf(angle);

			_vertices.push_back({ glm::normalize(glm::vec3(x * r, y, z * r)) * mult, { (float)i, 1.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
			trisNum.push_back(1u);
		}

		// sqrt_3 * sqrtf(2) * .5f - .5f it came out from previous normalization of vertices.
		// In shortcut it is height of Tetrahedron minus y value of normalized vertex
		_vertices.push_back({ glm::vec3(0.f, (M_SQRT3 * M_SQRT2 * .5f) - .5f, 0.f) * mult, {.5f, 0.f}, norm, glm::vec3(0.f), glm::vec3(0.f) });
		trisNum.push_back(1u);

		angleXZ += angleXZDiff;
	}

	// INDICES
	for (unsigned int i = 0u; i < segments; ++i) {
		size_t left = start + (size_t)i * 3ull;
		size_t right = start + (size_t)i * 3ull + 1ull;
		size_t top = start + (size_t)i * 3ull + 2ull;

		_indices.push_back((unsigned int)left);
		_indices.push_back((unsigned int)right);
		_indices.push_back((unsigned int)top);

		std::pair<glm::vec3, glm::vec3> TB = _calcTangentBitangent((unsigned int)left, (unsigned int)right, (unsigned int)top);

		_vertices[left].Tangent += TB.first;
		_vertices[left].Bitangent += TB.second;

		_vertices[right].Tangent += TB.first;
		_vertices[right].Bitangent += TB.second;

		_vertices[top].Tangent += TB.first;
		_vertices[top].Bitangent += TB.second;
	}

	_normalizeTangents(trisNum, 0ull, _vertices.size());

	trisNum.clear();
}

Tetrahedron::Tetrahedron(ValuesRange range)
{
	_vertices.clear();
	_indices.clear();
	_generate(range);
}

Tetrahedron::~Tetrahedron() {}

std::string Tetrahedron::getClassName()
{
	return "Tetrahedron";
}

std::string Tetrahedron::getObjectClassName() const
{
	return Tetrahedron::getClassName();
}