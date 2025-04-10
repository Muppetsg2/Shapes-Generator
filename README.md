<div align='center'>
   <img src="git_images/icon.png" alt="Logo" width="150" align="center"/>
   
  <div id="toc">
    <ul style="list-style: none;">
      <summary>
        <h1>Shapes Generator 1.2.2</h1>
      </summary>
    </ul>
  </div>
</div>

## 📌 Description
This project is a console-based **Shape Generator** written in **C++**. It allows users to create and save various **3D shapes** such as:

🔴 Sphere | 🔷 IcoSphere | ▭ Plane | 🧊 Cube | 🛢 Cylinder | ⬢ Hexagon | 🍦 Cone | 🔼 Tetrahedron | 🔺 Pyramid | 🍩 Torus

The program dynamically generates the shape based on user input and exports the shape data to a file.
> 🧭 Note: All shapes use the counterclockwise order for listing vertices — this helps keep things consistent and makes rendering easier in most 3D engines.

🛠 **Technologies used:**
- **GLM** (OpenGL Mathematics) for vector operations
- **FMT** for formatted output

## 📋 Table of Contents

- [✨ Features](#-features)
- [📦 Dependencies](#-dependencies)
- [⚙️ How to Build](#%EF%B8%8F-how-to-build)
   - [🖥️ Windows with vcpkg](#%EF%B8%8F-windows-with-vcpkg)
   - [🐧 Linux](#-linux)
   - [🍏 MacOS](#-macos)
- [▶️ Usage](#%EF%B8%8F-usage)
- [📜 Example Output](#-example-output)
- [🛠 Troubleshooting](#-troubleshooting)
- [👨‍💻 Authors](#-authors)
- [📜 License](#-license)

## ✨ Features

✅ **Interactive User Input**
   - Configure shapes through console input.
   - Choose from **ten** different shapes:
     1.  🔴 Sphere
     2.  🔷 IcoSphere
     3.  ▭ Plane
     4.  🧊 Cube
     5.  🛢 Cylinder
     6.  ⬢ Hexagon
     7.  🍦 Cone
     8.  🔼 Tetrahedron
     9.  🔺 Pyramid
     10. 🍩 Torus

✅ **ESC Key Support**: Press **ESC** anytime to exit the program.
<br/>
✅ **File Output**: Generated shapes are saved in `shape.txt` or `shape.obj` for later use. 
<br/>
✅ **Error Handling**: Provides feedback for invalid inputs and applies sensible defaults.

## 📦 Dependencies
- **C++20** or later
- **GLM** Library (OpenGL Mathematics)
- **FMT** Library (Formatted console output)

## ⚙️ How to Build

### 🖥️ Windows with **vcpkg**

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Muppetsg2/Shapes-Generator.git
   ```
2. **Install vcpkg** (if not installed):
   ```bash
   git clone https://github.com/microsoft/vcpkg
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```
   > 💡 **Tip:** If using **Visual Studio 2022**, check if `vcpkg` is already installed. [More info here](https://devblogs.microsoft.com/cppblog/vcpkg-is-now-included-with-visual-studio/).
3. **Integrate vcpkg with MSBuild:**
   ```bash
   .\vcpkg integrate install
   ```
4. **Open the project in Visual Studio** – dependencies are automatically included.

#### ⚡ Alternatively, using `g++` directly:

4. **Install dependencies using `vcpkg`:**
   ```bash
   .\vcpkg install glm fmt
   ```
5. **Ensure correct include paths and link libraries** from `vcpkg`.

### 🐧 Linux

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Muppetsg2/Shapes-Generator.git
   cd Shapes-Generator
   ```
2. **Install dependencies:**
   ```bash
   sudo apt-get install libglm-dev libfmt-dev
   ```
   > 🔹 `libfmt-dev` may not be available in all repositories. If not available check this solution. [Solution](https://askubuntu.com/questions/1205765/c-fmt-library-installation-is-not-working).
3. **Compile the precompiled header (PCH)**
   ```bash
   g++ -std=c++20 -x c++-header pch.h -o pch.h.gch
   ```
4. **Compile source files into object files**
   ```bash
   g++ -std=c++20 -c main.cpp Shape.cpp Sphere.cpp IcoSphere.cpp Plane.cpp Cube.cpp Cylinder.cpp Hexagon.cpp Cone.cpp Tetrahedron.cpp Pyramid.cpp Torus.cpp -I. -o main.o
   ```
5. **Link object files to create the final executable**
   ```bsh
   g++ -std=c++20 main.o -o Shapes-Generator -lfmt -lglm
   ```
6. **Run the program:**
   ```bash
   ./Shapes-Generator
   ```

### 🍏 MacOS

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Muppetsg2/Shapes-Generator.git
   cd Shapes-Generator
   ```
2. **Install dependencies using Homebrew:**
   ```bash
   brew install glm fmt
   ```
3. **Compile the precompiled header (PCH)**
   ```bash
   g++ -std=c++20 -x c++-header pch.h -o pch.h.gch -I/opt/homebrew/include
   ```
4. **Compile source files into object files**
   ```bash
   g++ -std=c++20 -c main.cpp Shape.cpp Sphere.cpp IcoSphere.cpp Plane.cpp Cube.cpp Cylinder.cpp Hexagon.cpp Cone.cpp Tetrahedron.cpp Pyramid.cpp Torus.cpp -I. -I/opt/homebrew/include -o main.o
   ```
5. **Link object files to create the final executable**
   ```bsh
   g++ -std=c++20 main.o -o Shapes-Generator -L/opt/homebrew/lib -lfmt -lglm
   ```
4. **Run the program:**
   ```bash
   ./Shapes-Generator
   ```

## ▶️ Usage
1. **Launch the program** - ASCII art and a menu will appear.
2. **Select a shape** by entering a number (1-10).
3. **Enter the required shape parameters**, if applicable.
4. **The shape is generated**.
5. **Select the file format** for saving.
6. **The file path** will be displayed in the console.
7. **Exit anytime** by pressing the **ESC** key.

## 📜 Example Output

When generating a **Sphere**, the output will look like this:
```mathematica
[OK] Start Generating Sphere...
[OK] Sphere Generated Successfully!

[INFO] Shape successfully generated in 4.51e-05s!

──────────────────────────────────────────────────
[FILE] Select the save format:
 1) std::vector  - Vertices & Indices (struct)
 2) C++ array    - Vertices & Indices (struct)
 3) std::vector  - Only Vertices (struct)
 4) C++ array    - Only Vertices (struct)
 5) std::vector  - Vertices & Indices (float)
 6) C++ array    - Vertices & Indices (float)
 7) std::vector  - Only Vertices (float)
 8) C++ array    - Only Vertices (float)
 9) Save as OBJ file
 ──────────────────────────────────────────────────
Enter your choice (1 - 9): 1

[OK] Start Saving Sphere to file...
[SAVED] Shape saved successfully in 0.0013585s!
[PATH] File path: C:\path\to\folder\shape.txt

Press Enter to exit...
```

## 🛠 Troubleshooting

❌ **Issues? Try this:**
- Ensure **all dependencies** are installed.
- Check **console messages** for errors.
- Verify you're using **C++20** or later.

## 👨‍💻 Authors
📌 **Marceli Antosik (Muppetsg2)**

## 📜 License

This project is **open-source** under the **MIT License**.

📖 **MIT License Overview:**
- ✅ Free to use, modify, and distribute.
- ✅ Can be used in **commercial** and **non-commercial** projects.
- ❗ Must include original license and copyright.

See the [LICENSE](./LICENSE) file for details.
