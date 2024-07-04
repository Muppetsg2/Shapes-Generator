#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Hexagon.h"
#include "Piramid.h"
#include "Tetrahedron.h"
#include <format>

using namespace std;

int main()
{
    cout << "Hello World!\n";

    Sphere s = Sphere(12, 12);
    Plane p = Plane(12, 12, PlaneNormalDir::UP);
    Cube c = Cube();
    Hexagon h = Hexagon();
    Piramid pi = Piramid();
    Tetrahedron t = Tetrahedron();

    fstream file;
    file.open("./wynik.txt", std::ios::out | std::ios::trunc);

    //file << s.getSphereAsString();
    //file << p.getPlaneAsString();
    //file << c.getCubeAsString();
    //file << h.getHexagonAsString();
    //file << pi.getPiramidAsString();
    file << t.getTetrahedronAsString();
    //file << vformat("{} {} {}\n", make_format_args("{", 10, "}"));

    file.close();
}
