#include "Shape.h"
#include <format>
#include <sstream>
#include <iomanip>

template<typename T>
const T& unmove(T&& x)
{
    return x;
}

Vertex Shape::calcTangentBitangent(unsigned int vertexIndex)
{
    Vertex v = vertices[vertexIndex];

    glm::vec3 tangent = glm::vec3(0.f);
    glm::vec3 bitangent = glm::vec3(0.f);
    unsigned int trianglesIncluded = 0;

    // Find the triangles that use v
    //  * Loop over every triangle (i + 3)
    for (unsigned int i = 0; i < indices.size(); i += 3) {
        unsigned int index0 = indices[i];
        unsigned int index1 = indices[i + 1];
        unsigned int index2 = indices[i + 2];

        // Only perform the calculation if one of the indices
        // matches our vertexIndex
        if (index0 == vertexIndex || index1 == vertexIndex || index2 == vertexIndex) {
            Vertex v0 = vertices[index0];
            Vertex v1 = vertices[index1];
            Vertex v2 = vertices[index2];

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

            tangent += (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;

            bitangent += (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r;

            trianglesIncluded += 1;
        }

    }

    // Average the tangent and bitangents
    if (trianglesIncluded > 0) {
        tangent /= trianglesIncluded;

        if (glm::length(tangent) != 0.f) {
            tangent = glm::normalize(tangent);
        }

        bitangent /= trianglesIncluded;

        if (glm::length(bitangent) != 0.f) {
            bitangent = glm::normalize(bitangent);
        }
    }

    // Save the results
    v.Tangent = tangent;
    v.Bitangent = bitangent;

    return v;
}

std::pair<glm::vec3, glm::vec3> Shape::calcTangentBitangent(unsigned int t1, unsigned int t2, unsigned int t3)
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

std::string Shape::formatFloat(float value) const
{
    std::stringstream ss;

    ss << std::fixed << std::setprecision(6) << value;
    std::string str = ss.str();

    if (str.find('.') != std::string::npos) {
        str = str.substr(0, str.find_last_not_of('0') + 1);

        //if (str.find('.') == str.size() - 1) str = str.substr(0, str.size() - 1);
    }

    return str;
}

Shape::~Shape()
{
    vertices.clear();
    indices.clear();
}

std::string Shape::toString(ArrayType type) const
{
    std::string text;
    switch (type) {
        case ArrayType::VECTOR: {
            text = "std::vector<Vertex> vertices = {\n";

            for (int i = 0; i < vertices.size(); ++i) {
                Vertex v = vertices[i];

                text += std::vformat(std::string_view("\t{} {}{}f, {}f, {}f{}, {}{}f, {}f{}, {}{}f, {}f, {}f{}, {}{}f, {}f, {}f{}, {}{}f, {}f, {}f{} {}"),
                    std::make_format_args
                    (
                        "{", ".Position = glm::vec3(",
                        unmove(formatFloat(v.Position.x)), unmove(formatFloat(v.Position.y)), unmove(formatFloat(v.Position.z)),
                        ")", ".TexCoords = glm::vec2(",
                        unmove(formatFloat(v.TexCoord.x)), unmove(formatFloat(v.TexCoord.y)),
                        ")", ".Normal = glm::vec3(",
                        unmove(formatFloat(v.Normal.x)), unmove(formatFloat(v.Normal.y)), unmove(formatFloat(v.Normal.z)),
                        ")", ".Tangent = glm::vec3(",
                        unmove(formatFloat(v.Tangent.x)), unmove(formatFloat(v.Tangent.y)), unmove(formatFloat(v.Tangent.z)),
                        ")", ".Bitangent = glm::vec3(",
                        unmove(formatFloat(v.Bitangent.x)), unmove(formatFloat(v.Bitangent.y)), unmove(formatFloat(v.Bitangent.z)),
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
                text += std::vformat
                (
                    std::string_view("\t{0}, {1}, {2}"),
                    std::make_format_args
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
            break;
        }
        case ArrayType::ARRAY: {
            text = "float vertices[" + std::to_string(vertices.size() * 14) + "] = {\n";
            text += "\t//POSITION\t\t\t\t\t//TEX COORDS\t//NORMALS\t\t\t\t//TANGENT\t\t\t\t//BITANGENT\n";

            for (int i = 0; i < vertices.size(); ++i) {
                Vertex v = vertices[i];

                text += std::vformat(std::string_view("\t{}f, {}f, {}f,\t\t\t\t{}f, {}f,\t{}f, {}f, {}f,\t\t\t\t{}f, {}f, {}f,\t\t\t\t{}f, {}f, {}f"),
                    std::make_format_args
                    (
                        unmove(formatFloat(v.Position.x)), unmove(formatFloat(v.Position.y)), unmove(formatFloat(v.Position.z)),
                        unmove(formatFloat(v.TexCoord.x)), unmove(formatFloat(v.TexCoord.y)),
                        unmove(formatFloat(v.Normal.x)), unmove(formatFloat(v.Normal.y)), unmove(formatFloat(v.Normal.z)),
                        unmove(formatFloat(v.Tangent.x)), unmove(formatFloat(v.Tangent.y)), unmove(formatFloat(v.Tangent.z)),
                        unmove(formatFloat(v.Bitangent.x)), unmove(formatFloat(v.Bitangent.y)), unmove(formatFloat(v.Bitangent.z))
                    )
                );

                if (i + 1 < vertices.size()) {
                    text += ",\n";
                }
                else {
                    text += "\n";
                }
            }

            text += "};\n\nunsigned int indices[" + std::to_string(indices.size()) + "] = {\n";

            for (int i = 0; i < indices.size(); i += 3) {
                text += std::vformat
                (
                    std::string_view("\t{0}, {1}, {2}"),
                    std::make_format_args
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
            break;
        }
    }

    return text;
}

size_t Shape::getVerticesCount() const
{
    return vertices.size();
}

size_t Shape::getIndicesCount() const
{
    return indices.size();
}
