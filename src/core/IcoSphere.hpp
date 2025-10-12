#pragma once
#include "Shape.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

enum class IcoSphereShading {
    FLAT = 0,
    SMOOTH = 1
};

class IcoSphere : public Shape {
private:
    void _generateIcoSahedron(const float mult, const bool useFlatShading, const bool hasSubdivisions);
    void _generate(const unsigned int subdivisions, const ValuesRange range, const bool useFlatShading);

    unsigned int _getMiddlePoint(const unsigned int p1, const unsigned int p2, const float mult);
    unsigned int _getMiddlePointFlatShading(const unsigned int p1, const unsigned int p2, const float mult);
    
    glm::vec2 _getTexCoord(const glm::vec3 normal) const;

    void _defineTangentBitangentFlatShading(const std::pair<glm::vec3, glm::vec3> TB, const size_t index);

    std::unordered_map<uint64_t, unsigned int> _middlePointCache;

public:
    IcoSphere(const unsigned int subdivisions = 0u, const IcoSphereShading shading = IcoSphereShading::FLAT, const ValuesRange range = ValuesRange::HALF_TO_HALF);
    virtual ~IcoSphere();

    static std::string getClassName();
    std::string getObjectClassName() const override;
};