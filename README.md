<div align='center'>
   <img alt="Logo" width="150" align="center" src="git_images/icon.png"/>
   
  <div id="toc">
    <ul style="list-style: none;">
      <summary>
        <h1>Shapes Generator 2.0.0</h1>
      </summary>
    </ul>
  </div>
</div>

## 📌 Description

This project is a console-based (CLI and TUI) 3D **shape generator** written in **C++**. It allows users to create and save a variety of **3D shapes** such as:

▭ Plane | 🧊 Cube | 🔼 Tetrahedron | 🔺 Pyramid | 🛢 Cylinder | 🍦 Cone | 🔴 Sphere | 🔷 IcoSphere | 🍩 Torus | ⬢ Hexagon

The program dynamically generates the shape based on user input and exports the shape data to a file.
> 🧭 Note: All shapes use the counterclockwise order for listing vertices, which ensures consistency and simplifies rendering in most 3D engines.

> 🔺 Note: Each shape is constructed using triangles.

🛠 **Technologies used:**
- [**GLM**](https://github.com/g-truc/glm) (OpenGL Mathematics) for vector operations
- [**FMT**](https://github.com/fmtlib/fmt) for formatted output
- [**JSON**](https://github.com/nlohmann/json) for exporting in json format
- [**Catch2**](https://github.com/catchorg/Catch2) for unit tests
- [**FTXUI**](https://github.com/ArthurSonzogni/FTXUI) for terminal user interface
- [**Clip**](https://github.com/dacap/clip) for copying path to clipboard

## 📋 Table of Contents

- [✨ Features](#-features)
- [📦 Dependencies](#-dependencies)
- [⚙️ How to Build](#%EF%B8%8F-how-to-build)
   - [🖥️ Windows](#%EF%B8%8F-windows)
   - [🐧 Linux](#-linux)
   - [🍏 MacOS](#-macos)
- [▶️ Usage](#%EF%B8%8F-usage)
   - [🖥️ TUI Mode (Default)](#%EF%B8%8F-tui-mode-default)
   - [💻 CLI Mode](#%EF%B8%8F-cli-mode)
- [🧾 Configuration](#-configuration)
- [🛠 Troubleshooting](#-troubleshooting)
- [👨‍💻 Authors](#-authors)
- [📜 License](#-license)

## ✨ Features

✅ **Interactive User Input**
   - Configure shapes through console input.
   - Choose from **ten** different shapes:
     1.  ▭ Plane
     2.  🧊 Cube
     3.  🔼 Tetrahedron
     4.  🔺 Pyramid
     5.  🛢 Cylinder
     6.  🍦 Cone
     7.  🔴 Sphere
     8.  🔷 IcoSphere
     9.  🍩 Torus
     10. ⬢ Hexagon

✅ **ESC Key Support**: Press **ESC** anytime to exit the program.
<br/>
✅ **File Output**: Generated shapes are saved in `shape.txt`, `shape.obj` or `shape.json` for later use.
<br/>
✅ **Error Handling**: Provides feedback for invalid inputs and applies sensible defaults.

## 📦 Dependencies

- **C++20** or later
- **CMake 3.30** or later
- [**GLM**](https://github.com/g-truc/glm) Library (OpenGL Mathematics)
- [**FMT**](https://github.com/fmtlib/fmt) Library (Formatted console output)
- [**JSON**](https://github.com/nlohmann/json) Library (Exporting in JSON format)
- [**Catch2**](https://github.com/catchorg/Catch2) Library (Unit tests)
- [**FTXUI**](https://github.com/ArthurSonzogni/FTXUI) Library (Terminal user interface)
- [**Clip**](https://github.com/dacap/clip) Library (Copying to clipboard)

## ⚙️ How to Build

### 🖥️ Windows

1. **Clone the repository:**

   ```bash
   > git clone https://github.com/Muppetsg2/Shapes-Generator.git
   > cd Shapes-Generator
   ```

2. **Open the folder in Visual Studio** (supports CMake natively).
3. CMake will **configure** the project and **download** dependencies automatically via `CPM.cmake`.
4. **Press ▶️** to build and run the app.

### 🐧 Linux

1. **Clone the repository:**

   ```bash
   > git clone https://github.com/Muppetsg2/Shapes-Generator.git
   > cd Shapes-Generator
   ```

2. **Install CMake, compiler and libx11 dependency** (if not installed):

   ```bash
   > sudo apt install cmake g++ ninja-build libx11-dev
   ```

3. **Build the project**:

   ```bash
   > cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
   > cmake --build build
   ```

4. **Run the app**:

   ```bash
   > ./build/bin/Shapes-Generator
   ```

### 🍏 MacOS

1. **Clone the repository:**

   ```bash
   > git clone https://github.com/Muppetsg2/Shapes-Generator.git
   > cd Shapes-Generator
   ```

2. **Install dependencies (CMake, Ninja, compiler)** (if not installed):

   ```bash
   > brew install cmake ninja
   ```

3. **Build the project with app bundle**:

   ```bash
   > cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
   > cmake --build build
   ```

4. **Run the app**:

   ```bash
   > open build/bin/Shapes-Generator.app
   ```

## ▶️ Usage

The application supports two modes:
- **TUI (Terminal User Interface)** – default mode
- **CLI (Command Line Interface)** – text-based sequential mode

> 🖥️ Note: By default, the program starts in **TUI mode**.

### 🖥️ TUI Mode (Default)
When launched without any flags, the program starts in **TUI mode** powered by [**FTXUI**](https://github.com/ArthurSonzogni/FTXUI).

<div align='center'>
   <img alt="TUI gif" height="250" align="center" src="git_images/tui.gif"/>
</div>

**How to use:**
1. **Launch the program** – A banner and an interactive menu with available shapes will appear.
2. *(Optional)* **Edit the configuration file** before generating a shape.
3. **Select a shape** by clicking on it or navigating with the keyboard.
4. **Enter the required shape parameters**, if applicable.
5. **Press the Generate button** to start the generation process.
6. **The shape will be generated**.
7. **Select the file format** for saving the shape.
8. **The file path** will be displayed in the console and copied to the clipboard.
9. *(Optional)* The **file explorer** may open at the saved file location.
10. **Exit at any time** by pressing the **ESC** key or generate another shape.

**Features of TUI mode:**
- Interactive navigation
- Structured menu layout
- Keyboard and mouse support
- Ability to change the selected object at any time

### 💻 CLI Mode

To launch the application in **CLI mode**, use one of the following flags:
```mathematica
-c
```
or
```mathematica
--cli
```

CLI mode provides a classic step-by-step console interaction.

<div align='center'>
   <img alt="CLI gif" height="250" align="center" src="git_images/cli.gif"/>
</div>

**How to use:**
1. **Launch the program with `-c` or `--cli`** - ASCII art and a menu will appear.
2. **Select a shape** by entering a number (1-10).
3. **Enter the required shape parameters**, if applicable.
4. **The shape is generated**.
5. **Select the file format** for saving.
6. **The file path** will be displayed in the console.
7. *(Optional)* **File explorer** will pop up with file location.
8. **Exit anytime** by pressing the **ESC** key.

**Features of CLI mode:**
- Lightweight sequential input
- Clear text prompts

## 🧾 Configuration

The application allows you to configure the default output path and filename using the `shapes.config` file.

**Example contents** of `shapes.config`:

   ```bash
   generateTangents: true
   calculateBitangents: true
   tangentHandednessPositive: true
   saveDir: C:\my\custom\output\
   fileName: my_${TYPE}-%H-%M-%S
   openDirOnSave: true
   ```

- **generateTangents**: Tells the generator whether to compute and include tangent vectors for each vertex (useful for normal mapping and advanced shading).
If `calculateBitangents` is enabled, bitangent vectors will also be generated for each vertex.
- **calculateBitangents**: Determines whether bitangent vectors should be calculated and included in the saved file.
- **tangentHandednessPositive**: Defines which handedness convention should be used when calculating bitangents or when saving tangents to the file.
- **saveDir**: Sets the directory where shape files will be saved. Can be absolute or relative to application directory.
- **fileName**: Defines the pattern for the output file name. You can use **standard time format markers**
compatible with the C++ function **strftime**, as well as a custom placeholder `${TYPE}`, 
which will be replaced with the name of the generated 3D object (e.g., `"Cube"`, `"Cone"`, `"Sphere"`).
- **openDirOnSave**: If enabled, instructs the application to open the system file explorer at the location of the saved file after generation.

### 🧩 Custom markers

| Marker | Description | Example |
| ------ | ----------- | ------- |
| `${TYPE}` | Name of the 3D object being generated | `Sphere` |

> 💡 If the config file is missing or malformed, defaults will be used.\
:information_source: The path will be different depending on your operating system. Here is an example of the path for Windows \
⏱️ List of all supported time marks you can find [here](https://cplusplus.com/reference/ctime/strftime/)

## 🛠 Troubleshooting

❌ **Issues? Try this:**
- Ensure **all dependencies** are installed.
- Check **console messages** for errors.
- Verify you're using **C++20** or later.

> :information_source: If you cannot see or understand the error, [open an issue](https://github.com/Muppetsg2/Shapes-Generator/issues).

## 👨‍💻 Authors

📌 **Marceli Antosik (Muppetsg2)**

## 📜 License

This project is **open-source** under the **MIT License**.

📖 **MIT License Overview:**
- ✅ Free to use, modify, and distribute.
- ✅ Can be used in **commercial** and **non-commercial** projects.
- ❗ Must include original license and copyright.

See the [LICENSE](./LICENSE) file for details.
