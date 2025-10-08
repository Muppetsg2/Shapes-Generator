#include "Shape.hpp"
#include "Config.hpp"

float Shape::_map(const float input, const float currStart, const float currEnd, const float expectedStart, const float expectedEnd) const
{
    return expectedStart + ((expectedEnd - expectedStart) / (currEnd - currStart)) * (input - currStart);
}

Vertex Shape::_calcTangentBitangent(const unsigned int vertexIndex) const
{
    Vertex v = _vertices[vertexIndex];

    glm::vec3 tangent = glm::vec3(0.f);
    glm::vec3 bitangent = glm::vec3(0.f);
    unsigned int trianglesIncluded = 0u;

    // Find the triangles that use v
    //  * Loop over every triangle (i + 3)
    for (size_t i = 0ull; i < _indices.size(); i += 3ull) {
        const unsigned int index0 = _indices[i];
        const unsigned int index1 = _indices[i + 1ull];
        const unsigned int index2 = _indices[i + 2ull];

        // Only perform the calculation if one of the indices
        // matches our vertexIndex
        if (index0 == vertexIndex || index1 == vertexIndex || index2 == vertexIndex) {
            Vertex v0 = _vertices[index0];
            Vertex v1 = _vertices[index1];
            Vertex v2 = _vertices[index2];

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

            const float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);

            tangent += (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;

            bitangent += (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r;

            trianglesIncluded += 1u;
        }
    }

    // Average the tangent and bitangents
    if (trianglesIncluded > 0u) {
        tangent /= trianglesIncluded;

        if (fabsf(glm::length(tangent)) >= EPSILON) {
            tangent = glm::normalize(tangent);
        }

        bitangent /= trianglesIncluded;

        if (fabsf(glm::length(bitangent)) >= EPSILON) {
            bitangent = glm::normalize(bitangent);
        }
    }

    // Save the results
    v.Tangent = tangent;
    v.Bitangent = bitangent;

    return v;
}

std::pair<glm::vec3, glm::vec3> Shape::_calcTangentBitangent(const unsigned int t1, const unsigned int t2, const unsigned int t3) const
{
    std::pair<glm::vec3, glm::vec3> TB;

    const Vertex v0 = _vertices[t1];
    const Vertex v1 = _vertices[t2];
    const Vertex v2 = _vertices[t3];

    const glm::vec3 pos0 = v0.Position;
    const glm::vec3 pos1 = v1.Position;
    const glm::vec3 pos2 = v2.Position;

    const glm::vec2 uv0 = v0.TexCoord;
    const glm::vec2 uv1 = v1.TexCoord;
    const glm::vec2 uv2 = v2.TexCoord;

    const glm::vec3 delta_pos1 = pos1 - pos0;
    const glm::vec3 delta_pos2 = pos2 - pos0;

    const glm::vec2 delta_uv1 = uv1 - uv0;
    const glm::vec2 delta_uv2 = uv2 - uv0;

    const float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);

    // Save the results
    TB.first = (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;
    TB.second = (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r;

    return TB;
}

void Shape::_normalizeTangents(const std::vector<unsigned int>& trisNum, const size_t start, const size_t end)
{
    for (size_t i = start; i < end; ++i) {
        _vertices[i].Tangent /= (float)trisNum[i - start];

        if (fabsf(glm::length(_vertices[i].Tangent)) >= EPSILON) {
            _vertices[i].Tangent = glm::normalize(_vertices[i].Tangent);
        }

        _vertices[i].Bitangent /= (float)trisNum[i - start];

        if (fabsf(glm::length(_vertices[i].Bitangent)) >= EPSILON) {
            _vertices[i].Bitangent = glm::normalize(_vertices[i].Bitangent);
        }
    }
}

std::string Shape::_getGeneratedHeader(const std::string commentSign) const
{
    return commentSign + " Shapes Generator " + SHAPES_GENERATOR_VERSION + "\n" + commentSign + " https://github.com/Muppetsg2/Shapes-Generator\n\n";
}

std::string Shape::_getStructDefinition(bool isC99) const
{
    const Config& config = get_config();

    std::string tangentBlock = "";
    if (config.genTangents) {
        tangentBlock = "\tvec3 Tangent;\n\tvec3 Bitangent;\n";
    }

    if (isC99) {
        return  "typedef struct {\n\tfloat x, y, z;\n} vec3;\n\n"
                "typedef struct {\n\tfloat x, y;\n} vec2;\n\n"
                "typedef struct {\n\tvec3 Position;\n\tvec2 TexCoord;\n\tvec3 Normal;\n" + tangentBlock + "} Vertex;\n\n";
    }
    else {
        return  "struct vec3\n{\n\tfloat x, y, z;\n};\n\n"
                "struct vec2\n{\n\tfloat x, y;\n};\n\n"
                "struct Vertex\n{\n\tvec3 Position;\n\tvec2 TexCoord;\n\tvec3 Normal;\n" + tangentBlock + "};\n\n";
    }
}

std::string Shape::_formatFloat(float value, bool delRedundantZeros) const
{
    std::stringstream ss;

    ss << std::fixed << std::setprecision(6) << value;
    std::string str = ss.str();

    if (delRedundantZeros && str.find('.') != std::string::npos) {
        str = str.substr(0, str.find_last_not_of('0') + 1);

        //if (str.find('.') == str.size() - 1) str = str.substr(0, str.size() - 1);
    }

    return str;
}

std::string Shape::_formatVertex(const Vertex& v, bool useFloat) const
{
    const Config& config = get_config();

    std::string formatStr;
    std::vector<std::string> args;
    if (useFloat) {
        formatStr = "\t{}f, {}f, {}f,\t\t\t\t{}f, {}f,\t{}f, {}f, {}f";
        args = {
            unmove(_formatFloat(v.Position.x)), unmove(_formatFloat(v.Position.y)), unmove(_formatFloat(v.Position.z)),
            unmove(_formatFloat(v.TexCoord.x)), unmove(_formatFloat(v.TexCoord.y)),
            unmove(_formatFloat(v.Normal.x)),   unmove(_formatFloat(v.Normal.y)),   unmove(_formatFloat(v.Normal.z))
        };

        if (config.genTangents) {
            formatStr += ",\t\t\t\t{}f, {}f, {}f,\t\t\t\t{}f, {}f, {}f";
            args.insert(args.end(), {
                unmove(_formatFloat(v.Tangent.x)),   unmove(_formatFloat(v.Tangent.y)),   unmove(_formatFloat(v.Tangent.z)),
                unmove(_formatFloat(v.Bitangent.x)), unmove(_formatFloat(v.Bitangent.y)), unmove(_formatFloat(v.Bitangent.z))
            });
        }
    }
    else {
        formatStr = "\t{} {}{}f, {}f, {}f{}, {}{}f, {}f{}, {}{}f, {}f, {}f{}";
        args = {
            "{", "{ ",
            unmove(_formatFloat(v.Position.x)), unmove(_formatFloat(v.Position.y)), unmove(_formatFloat(v.Position.z)),
            " }", "{ ",
            unmove(_formatFloat(v.TexCoord.x)), unmove(_formatFloat(v.TexCoord.y)),
            " }", "{ ",
            unmove(_formatFloat(v.Normal.x)),   unmove(_formatFloat(v.Normal.y)),   unmove(_formatFloat(v.Normal.z)),
            " }"
        };

        if (config.genTangents) {
            formatStr += ", {}{}f, {}f, {}f{}, {}{}f, {}f, {}f{} {}";
            args.insert(args.end(), {
                "{ ",
                unmove(_formatFloat(v.Tangent.x)),   unmove(_formatFloat(v.Tangent.y)),   unmove(_formatFloat(v.Tangent.z)),
                " }",
                "{ ",
                unmove(_formatFloat(v.Bitangent.x)), unmove(_formatFloat(v.Bitangent.y)), unmove(_formatFloat(v.Bitangent.z)),
                " }", "}"
            });
        }
        else {
            formatStr += " {}";
            args.push_back("}");
        }
    }

    fmt::dynamic_format_arg_store<fmt::format_context> store;
    for (const std::string& arg : args) {
        store.push_back(arg);
    }

    return fmt::vformat(fmt::string_view(formatStr), store);
}

std::string Shape::_formatVertices(bool onlyVertices, bool useArray, bool useFloat) const
{
    const Config& config = get_config();

    std::string result;
    std::string type = useFloat ? "float" : "Vertex";
    std::string header = useArray
        ? type + " vertices[" + std::to_string((onlyVertices ? _indices.size() : _vertices.size()) * (useFloat ? 14ull : 1ull)) + "] = {\n"
        : "std::vector<" + type + "> vertices = {\n";

    result += header;

    std::string indent = useFloat ? "\t\t\t\t" : "\t\t\t\t\t";
    std::string tangentBlock = config.genTangents ? indent + "//TANGENT" + indent + "//BITANGENT" : "";

    if (useFloat) result += "\t//POSITION\t\t\t\t\t//TEX COORD\t//NORMAL" + tangentBlock + "\n";
    else result += "\t//POSITION\t\t\t\t\t\t//TEX COORD\t\t//NORMAL" + tangentBlock + "\n";

    size_t count = onlyVertices ? _indices.size() : _vertices.size();
    for (size_t i = 0; i < count; ++i) {
        const Vertex& v = onlyVertices ? _vertices[_indices[i]] : _vertices[i];
        result += _formatVertex(v, useFloat);
        if (i + 1ull < count) result += ",";
        result += "\n";
    }

    result += "};";
    return result;
}

std::string Shape::_formatIndices(bool useArray) const
{
    std::string result;
    std::string header = useArray
        ? "unsigned int indices[" + std::to_string(_indices.size()) + "] = {\n"
        : "std::vector<unsigned int> indices = {\n";

    result += header;

    for (size_t i = 0; i < _indices.size(); i += 3) {
        result += fmt::vformat(
            fmt::string_view("\t{0}, {1}, {2}"),
            fmt::make_format_args(_indices[i], _indices[i + 1], _indices[i + 2])
        );
        if (i + 3 < _indices.size()) result += ",";
        result += "\n";
    }

    result += "};";
    return result;
}

std::string Shape::_toOBJ() const
{
    std::unordered_map<glm::vec3, unsigned int, Vec3Hash, Vec3Equal> vertexMap;
    std::unordered_map<glm::vec2, unsigned int, Vec2Hash, Vec2Equal> texCoordMap;
    std::unordered_map<glm::vec3, unsigned int, Vec3Hash, Vec3Equal> normalMap;

    std::vector<glm::vec3> v;
    std::vector<glm::vec2> vt;
    std::vector<glm::vec3> vn;
    std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> vertIndices;

    v.reserve(_vertices.size());
    vt.reserve(_vertices.size());
    vn.reserve(_vertices.size());
    vertIndices.reserve(_indices.size());

    for (unsigned int i : _indices) {
        Vertex vert = _vertices[i];
        std::tuple<unsigned int, unsigned int, unsigned int> ind = {};

        auto [itV, insertedV] = vertexMap.try_emplace(vert.Position, (unsigned int)v.size() + 1u);
        if (insertedV) v.push_back(vert.Position);
        std::get<0>(ind) = itV->second;

        auto [itT, insertedT] = texCoordMap.try_emplace(vert.TexCoord, (unsigned int)vt.size() + 1u);
        if (insertedT) vt.push_back(vert.TexCoord);
        std::get<1>(ind) = itT->second;

        auto [itN, insertedN] = normalMap.try_emplace(vert.Normal, (unsigned int)vn.size() + 1u);
        if (insertedN) vn.push_back(vert.Normal);
        std::get<2>(ind) = itN->second;

        vertIndices.push_back(ind);
    }

    std::string text = _getGeneratedHeader("#") + "o " + getObjectClassName() + "\n";
    for (const glm::vec3& pos : v) {
        text += fmt::vformat(fmt::string_view("v {} {} {}\n"),
            fmt::make_format_args
            (
                unmove(_formatFloat(pos.x, false)),
                unmove(_formatFloat(pos.y, false)),
                unmove(_formatFloat(pos.z, false))
            )
        );
    }

    for (const glm::vec3& norm : vn) {
        text += fmt::vformat(fmt::string_view("vn {} {} {}\n"),
            fmt::make_format_args
            (
                unmove(_formatFloat(norm.x, false)),
                unmove(_formatFloat(norm.y, false)),
                unmove(_formatFloat(norm.z, false))
            )
        );
    }

    for (const glm::vec2& tex : vt) {
        text += fmt::vformat(fmt::string_view("vt {} {}\n"),
            fmt::make_format_args
            (
                unmove(_formatFloat(tex.x, false)),
                unmove(_formatFloat(tex.y, false))
            )
        );
    }

    text += "s 0\n";

    for (size_t i = 0; i < vertIndices.size(); i += 3ull) {
        text += fmt::vformat(fmt::string_view("f {}/{}/{} {}/{}/{} {}/{}/{}\n"),
            fmt::make_format_args
            (
                std::get<0>(vertIndices[i]), std::get<1>(vertIndices[i]), std::get<2>(vertIndices[i]),
                std::get<0>(vertIndices[i + 1]), std::get<1>(vertIndices[i + 1]), std::get<2>(vertIndices[i + 1]),
                std::get<0>(vertIndices[i + 2]), std::get<1>(vertIndices[i + 2]), std::get<2>(vertIndices[i + 2])
            )
        );
    }

    return text;
}

Shape::~Shape()
{
    _vertices.clear();
    _indices.clear();
}

std::string Shape::toString(FormatType type) const
{
    switch (type) {
        case FormatType::VECTOR_INDICES_STRUCT : {
            return _getGeneratedHeader("//") +
                   "#include <vector>\n\n" +
                   _getStructDefinition(false) +
                   _formatVertices(false, false, false) + "\n\n" +
                   _formatIndices(false);
        }
        case FormatType::ARRAY_INDICES_STRUCT : {
            return _getGeneratedHeader("//") +
                   _getStructDefinition(true) +
                   _formatVertices(false, true, false) + "\n\n" +
                   _formatIndices(true);
        }
        case FormatType::VECTOR_VERTICES_STRUCT: {
            return _getGeneratedHeader("//") +
                   "#include <vector>\n\n" +
                   _getStructDefinition(false)  +
                   _formatVertices(true, false, false);
        }
        case FormatType::ARRAY_VERTICES_STRUCT: {
            return _getGeneratedHeader("//") +
                   _getStructDefinition(true) +
                   _formatVertices(true, true, false);
        }
        case FormatType::VECTOR_INDICES_FLOAT: {
            return _getGeneratedHeader("//") +
                   "#include <vector>\n\n" +
                   _formatVertices(false, false, true) + "\n\n" +
                   _formatIndices(false);
        }
        case FormatType::ARRAY_INDICES_FLOAT: {
            return _getGeneratedHeader("//") +
                   _formatVertices(false, true, true) + "\n\n" +
                   _formatIndices(true);
        }
        case FormatType::VECTOR_VERTICES_FLOAT: {
            return _getGeneratedHeader("//") +
                   "#include <vector>\n\n" +
                   _formatVertices(true, false, true);
        }
        case FormatType::ARRAY_VERTICES_FLOAT: {
            return _getGeneratedHeader("//") +
                   _formatVertices(true, true, true);
        }
        case FormatType::OBJ: {
            return _toOBJ();
        }
    }

    return "";
}

std::string Shape::getClassName()
{
    return "Shape";
}

std::string Shape::getObjectClassName() const
{
    return Shape::getClassName();
}

size_t Shape::getVerticesCount() const
{
    return _vertices.size();
}

size_t Shape::getIndicesCount() const
{
    return _indices.size();
}
