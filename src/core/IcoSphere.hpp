#pragma once

#pragma region STD_LIBS
#include <cstdint>
#include <string>
#include <unordered_map>
#pragma endregion

#pragma region GLM_LIB
#include <glm/fwd.hpp>
#pragma endregion

#pragma region MY_FILES
#include "Shape.hpp"
#pragma endregion

class IcoSphere : public Shape {
private:
    void _generateIcoSahedron(const float mult, const bool useFlatShading, const bool hasSubdivisions);
    void _generate(const unsigned int subdivisions, const ValuesRange range, const bool useFlatShading);

    unsigned int _getMiddlePoint(const unsigned int p1, const unsigned int p2, const float mult);
    unsigned int _getMiddlePointFlatShading(const unsigned int p1, const unsigned int p2, const float mult);
    
    glm::vec2 _getTexCoord(const glm::vec3 normal) const;

    void _defineTangentBitangentFlatShading(const glm::vec3 tangent, const size_t index);

    std::unordered_map<uint64_t, unsigned int> _middlePointCache;

public:
    IcoSphere(const ShapeConfig& config, const unsigned int subdivisions = 0u, const ValuesRange range = ValuesRange::HALF_TO_HALF, const Shading shading = Shading::FLAT);
    virtual ~IcoSphere();

    static std::string getClassName();
    std::string getObjectClassName() const override;
};