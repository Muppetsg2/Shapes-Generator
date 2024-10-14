# Shapes-Generator

## Overview
This project is a console-based Shape Generator program written in C++. It allows the user to create and save a variety of 3D shapes like spheres, planes, cubes, hexagons, pyramids, and tetrahedrons. The program takes user input for specific shape parameters, dynamically generates the shape, and outputs the shape data to a text file.

This project uses the **GLM** (OpenGL Mathematics) library for vector operations, and **FMT** for formatted output.

## Features
- **Interactive User Input**: 
    - The program accepts user input through the console to configure shapes.
    - The user can choose from six different shapes to generate:
        1. Sphere
        2. Plane
        3. Cube
        4. Hexagon
        5. Pyramid
        6. Tetrahedron
- **ESC Key Support**: Pressing the ESC key exits the program at any time.
- **File Output**: Once a shape is generated, it is saved in a text file (`shape.txt`) for later use.
- **Error Handling**: Provides feedback for invalid input and ensures sensible defaults for shape parameters.

## Dependencies
- **C++20** or later
- **GLM** Library (OpenGL Mathematics) - for vector operations
- **FMT** Library (for formatted console output)

## How to Build

### Windows with VS 2019 **vcpkg**

1. Clone the repository:
   ```bash
   git clone https://github.com/Muppetsg2/Shapes-Generator.git
   cd Shapes-Generator
   ```
2. Install vcpkg (if you haven't already):
   ```bash
   git clone https://github.com/microsoft/vcpkg
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```
3. Install GLM and FMT using vcpkg:
   ```bash
   .\vcpkg install glm fmt
   ```
4. Integrate vcpkg with your MSBuild:
   ```bash
   .\vcpkg integrate install
   ```
5. Open the project in Visual Studio. Visual Studio will automatically include the necessary dependencies from vcpkg. Alternatively, if using g++ directly, ensure you specify the correct include paths and link libraries provided by vcpkg.

### Windows with VS 2022 **vcpkg**

1. Just check if you have installed vcpkg in your VS 2022 installation. You can find help on how to find if you have already installed vcpkg [here](https://devblogs.microsoft.com/cppblog/vcpkg-is-now-included-with-visual-studio/).

### Linux/MacOS

1. Clone the repository:
   ```bash
   git clone https://github.com/Muppetsg2/Shapes-Generator.git
   cd Shapes-Generator
   ```
2. Install the required dependencies (**GLM** and **FMT**):
   ```bash
   sudo apt-get install libglm-dev libfmt-dev
   ```
   > **Note**: The libfmt-dev package may not be available in all repositories or might have a different name. If you encounter issues, check your official repository or consider using vcpkg for dependency management.
3. Compile the project using g++:
   ```bash
   g++ -std=c++20 main.cpp Shape.cpp Sphere.cpp Plane.cpp Cube.cpp Hexagon.cpp Pyramid.cpp Tetrahedron.cpp -o Shapes-Generator -lfmt
   ```
4. Run the program:
   ```bash
   ./Shapes-Generator
   ```
   
## Usage
1. Launch the program. You'll see the start window with ASCII art and a prompt to choose a shape.
2. Select a shape by entering a number between 1 and 6.
3. If the shape requires parameters (e.g., a sphere or plane), enter the requested values.
4. Once the shape is generated, it will be saved to `shape.txt` in the program's directory, and you'll see how long the shape took to generate.
5. The file path of the saved shape will be displayed on the console.
6. **Exiting the Program:** You can exit the program at any time by pressing the **ESC** key.

## Example Output

When generating a sphere:

```mathematica
Choose a shape:
1. Sphere
2. Plane
3. Cube
4. Hexagon
5. Pyramid
6. Tetrahedron
Your choice: 1

Enter sphere number of horizontal segments (>= 2): 3
Enter sphere number of vertical segments (>= 3): 4

Start Generating Sphere!
Shape Generated in 0.00234s!
Shape saved to file: C:\path\to\project\shape.txt
```
## File Output Example (`shape.txt`)

The generated shape is saved to a file (`shape.txt`). For example, generating a sphere will produce the following kind of output:

```cpp
std::vector<Vertex> vertices {
	{ .Position = glm::vec3(0.0f, 1.0f, 0.0f), .TexCoords = glm::vec2(0.5f, 0.0f), .Normal = glm::vec3(0.0f, 1.0f, 0.0f), .Tangent = glm::vec3(-1.0f, 0.0f, 0.0f), .Bitangent = glm::vec3(0.0f, -0.5f, -0.9f) },
	{ .Position = glm::vec3(0.0f, 0.5f, 0.9f), .TexCoords = glm::vec2(0.0f, 0.3f), .Normal = glm::vec3(0.0f, 0.5f, 0.9f), .Tangent = glm::vec3(0.7f, 0.0f, -0.7f), .Bitangent = glm::vec3(0.7f, -0.6f, -0.4f) },
	{ .Position = glm::vec3(0.9f, 0.5f, -0.0f), .TexCoords = glm::vec2(0.2f, 0.3f), .Normal = glm::vec3(0.9f, 0.5f, -0.0f), .Tangent = glm::vec3(0.2f, 0.0f, -1.0f), .Bitangent = glm::vec3(0.4f, -0.9f, -0.4f) },
	{ .Position = glm::vec3(-0.0f, 0.5f, -0.9f), .TexCoords = glm::vec2(0.5f, 0.3f), .Normal = glm::vec3(-0.0f, 0.5f, -0.9f), .Tangent = glm::vec3(-1.0f, 0.0f, -0.2f), .Bitangent = glm::vec3(-0.0f, -0.9f, -0.4f) },
	{ .Position = glm::vec3(-0.9f, 0.5f, 0.0f), .TexCoords = glm::vec2(0.8f, 0.3f), .Normal = glm::vec3(-0.9f, 0.5f, 0.0f), .Tangent = glm::vec3(-0.2f, 0.0f, 1.0f), .Bitangent = glm::vec3(-0.4f, -0.9f, -0.4f) },
	{ .Position = glm::vec3(0.0f, 0.5f, 0.9f), .TexCoords = glm::vec2(1.0f, 0.3f), .Normal = glm::vec3(0.0f, 0.5f, 0.9f), .Tangent = glm::vec3(0.7f, 0.0f, 0.7f), .Bitangent = glm::vec3(-0.5f, -0.8f, -0.3f) },
	{ .Position = glm::vec3(0.0f, -0.5f, 0.9f), .TexCoords = glm::vec2(0.0f, 0.7f), .Normal = glm::vec3(0.0f, -0.5f, 0.9f), .Tangent = glm::vec3(0.7f, 0.0f, -0.7f), .Bitangent = glm::vec3(-0.5f, -0.8f, 0.3f) },
	{ .Position = glm::vec3(0.9f, -0.5f, -0.0f), .TexCoords = glm::vec2(0.2f, 0.7f), .Normal = glm::vec3(0.9f, -0.5f, -0.0f), .Tangent = glm::vec3(-0.2f, 0.0f, -1.0f), .Bitangent = glm::vec3(-0.4f, -0.9f, 0.4f) },
	{ .Position = glm::vec3(-0.0f, -0.5f, -0.9f), .TexCoords = glm::vec2(0.5f, 0.7f), .Normal = glm::vec3(-0.0f, -0.5f, -0.9f), .Tangent = glm::vec3(-1.0f, 0.0f, 0.2f), .Bitangent = glm::vec3(0.0f, -0.9f, 0.4f) },
	{ .Position = glm::vec3(-0.9f, -0.5f, 0.0f), .TexCoords = glm::vec2(0.8f, 0.7f), .Normal = glm::vec3(-0.9f, -0.5f, 0.0f), .Tangent = glm::vec3(0.2f, 0.0f, 1.0f), .Bitangent = glm::vec3(0.4f, -0.9f, 0.4f) },
	{ .Position = glm::vec3(0.0f, -0.5f, 0.9f), .TexCoords = glm::vec2(1.0f, 0.7f), .Normal = glm::vec3(0.0f, -0.5f, 0.9f), .Tangent = glm::vec3(0.7f, 0.0f, 0.7f), .Bitangent = glm::vec3(0.7f, -0.6f, 0.4f) },
	{ .Position = glm::vec3(0.0f, -1.0f, 0.0f), .TexCoords = glm::vec2(0.5f, 1.0f), .Normal = glm::vec3(0.0f, -1.0f, 0.0f), .Tangent = glm::vec3(-0.4f, 0.0f, -0.9f), .Bitangent = glm::vec3(0.0f, -0.5f, 0.9f) }
};

std::vector<unsigned int> indices = {
	2, 0, 1,
	7, 6, 11,
	3, 0, 2,
	8, 7, 11,
	4, 0, 3,
	9, 8, 11,
	5, 0, 4,
	10, 9, 11,
	2, 1, 6,
	7, 2, 6,
	3, 2, 7,
	8, 3, 7,
	4, 3, 8,
	9, 4, 8,
	5, 4, 9,
	10, 5, 9
};
```

Where the Vertex structure is defined as:
```cpp
struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
```

## Troubleshooting

If you encounter issues:
  - Ensure that you have installed all dependencies correctly.
  - Check your console for any error messages related to input.
  - Make sure that you are using a compatible version of C++ (C++20 or later).

## License

This project is open-source and available under the MIT License.

> **MIT License Overview:** The MIT License is a permissive free software license that allows users to use, modify, and distribute the software as they wish. Users can incorporate the code into their own projects, both commercial and non-commercial, as long as the original copyright notice and license are included in all copies or substantial portions of the software.

See the LICENSE file for more details.
