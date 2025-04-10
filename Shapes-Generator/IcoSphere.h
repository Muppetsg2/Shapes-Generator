#pragma once
#include "Shape.h"

enum class IcoSphereShading {
    FLAT = 0,
    SMOOTH = 1
};

class IcoSphere : public Shape {
private:
    void _generateIcoSahedron(float mult, bool useFlatShading, bool hasSubdivisions);
    void _generate(unsigned int subdivisions, ValuesRange range, bool useFlatShading);

    unsigned int _getMiddlePoint(unsigned int p1, unsigned int p2, float mult);
    unsigned int _getMiddlePointFlatShading(unsigned int p1, unsigned int p2, float mult);
    
    glm::vec2 _getTexCoord(glm::vec3 normal);

    void _defineTangentBitangentFlatShading(std::pair<glm::vec3, glm::vec3> TB, size_t index);

    std::unordered_map<uint64_t, unsigned int> _middlePointCache;

public:
    IcoSphere(unsigned int subdivisions = 0u, IcoSphereShading shading = IcoSphereShading::FLAT, ValuesRange range = ValuesRange::HALF_TO_HALF);
    virtual ~IcoSphere();

    static std::string getClassName();
    std::string getObjectClassName() const override;
};