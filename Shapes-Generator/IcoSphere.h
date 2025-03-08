#pragma once
#include "Shape.h"

enum class IcoSphereShading {
    FLAT = 0,
    PHONG = 1
};

class IcoSphere : public Shape {
private:
    void generateIcoSahedron(float mult, bool useFlatShading, bool hasSubdivisions);
    void generate(unsigned int subdivisions, ValuesRange range, bool useFlatShading);

    unsigned int getMiddlePoint(unsigned int p1, unsigned int p2, float mult);
    unsigned int getMiddlePointFlatShading(unsigned int p1, unsigned int p2, float mult);
    
    glm::vec2 getTexCoord(glm::vec3 normal);

    void defineTangentBitangentFlatShading(std::pair<glm::vec3, glm::vec3> TB, size_t index);

    std::unordered_map<uint64_t, unsigned int> middlePointCache;

public:
    IcoSphere(unsigned int subdivisions = 0u, IcoSphereShading shading = IcoSphereShading::FLAT, ValuesRange range = ValuesRange::HALF_TO_HALF);
    virtual ~IcoSphere();

    static std::string getClassName();
    std::string getObjectClassName() const override;
};