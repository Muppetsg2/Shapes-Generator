//
//   ____  _                              ____                           _             
//  / ___|| |__   __ _ _ __   ___  ___   / ___| ___ _ __   ___ _ __ __ _| |_ ___  _ __ 
//  \___ \| '_ \ / _` | '_ \ / _ \/ __| | |  _ / _ \ '_ \ / _ \ '__/ _` | __/ _ \| '__|
//   ___) | | | | (_| | |_) |  __/\__ \ | |_| |  __/ | | |  __/ | | (_| | || (_) | |   
//  |____/|_| |_|\__,_| .__/ \___||___/  \____|\___|_| |_|\___|_|  \__,_|\__\___/|_|   
//                    |_|                                                                
//
// Version: 1.4.0
// Author: Marceli Antosik (Muppetsg2)
// Last Update: 04.02.2025

#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#pragma endregion

#pragma region FMT_LIB
#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#pragma endregion

#pragma region PLATFORM_LIBS
#if defined(_WIN32)
#define NOGDI
#define NOATOM
#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#include <consoleapi.h>
#include <processenv.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#pragma endregion

#pragma region MY_FILES_CORE_LIB
#include <Cone.hpp>
#include <Cube.hpp>
#include <Cylinder.hpp>
#include <Hexagon.hpp>
#include <IcoSphere.hpp>
#include <Plane.hpp>
#include <Pyramid.hpp>
#include <Shape.hpp>
#include <Sphere.hpp>
#include <SystemFunctions.hpp>
#include <Tetrahedron.hpp>
#include <Torus.hpp>
#pragma endregion

#pragma region MY_FILES
#include "ConsoleTemplates.hpp"
#include "Config.hpp"
#pragma endregion

Shape* selectedShape = nullptr;

#pragma region STRING_OPERATORS
static void replace_all(std::string& s, std::string const& toReplace, std::string const& replaceWith)
{
    std::string buf;
    size_t pos = 0;
    size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos) break;
        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}
#pragma endregion

#pragma region CONSOLE_OUTPUT_FUNCTIONS
static void printInvalidOption(int firstOptionNum, int lastOptionNum)
{
    fmt::print("[{}] Invalid option! Enter a number between {} and {} and try again.\n",
        fmt::styled("WARNING", fmt::fg(fmt::color::yellow)), firstOptionNum, lastOptionNum);
}

static std::string getInputPrompt(int firstOptionNum, int lastOptionNum)
{
    return fmt::format("Enter your choice ({} - {}): ", firstOptionNum, lastOptionNum);
}

static void printBannerLineColored(std::string text, fmt::color color)
{
    fmt::print("│");
    fmt::print("{}", fmt::styled(text, fmt::fg(color)));
    fmt::print("│\n");
}

static void displayStartWindow()
{
    size_t lineLength = std::string("   ____  _                              ____                           _               ").size();
    std::string versionTxt = std::string("Version: ").append(SHAPES_GENERATOR_VERSION);
    std::string escTxt = std::string("Press ESC to exit the program");

    size_t verSpaceCount = (lineLength - versionTxt.size()) / 2ull;
    size_t verRemainder = (lineLength - versionTxt.size()) % 2ull;

    size_t escSpaceCount = (lineLength - escTxt.size()) / 2ull;
    size_t escRemainder = (lineLength - escTxt.size()) % 2ull;

    fmt::color color = fmt::color::cyan;

    fmt::print("┌───────────────────────────────────────────────────────────────────────────────────────┐\n");
    fmt::print("│                                                                                       │\n");
    printBannerLineColored("   ____  _                              ____                           _               ", color);
    printBannerLineColored("  / ___|| |__   __ _ _ __   ___  ___   / ___| ___ _ __   ___ _ __ __ _| |_ ___  _ __   ", color);
    printBannerLineColored("  \\___ \\| '_ \\ / _` | '_ \\ / _ \\/ __| | |  _ / _ \\ '_ \\ / _ \\ '__/ _` | __/ _ \\| '__|  ", color);
    printBannerLineColored("   ___) | | | | (_| | |_) |  __/\\__ \\ | |_| |  __/ | | |  __/ | | (_| | || (_) | |     ", color);
    printBannerLineColored("  |____/|_| |_|\\__,_| .__/ \\___||___/  \\____|\\___|_| |_|\\___|_|  \\__,_|\\__\\___/|_|     ", color);
    printBannerLineColored("                    |_|                                                                ", color);
    fmt::print("│                                                                                       │\n");
    fmt::print("│{:<{}}{}{:>{}}│\n", "", verSpaceCount, versionTxt, "", verSpaceCount + verRemainder);
    fmt::print("│                                                                                       │\n");
    fmt::print("│{:<{}}{}{:>{}}│\n", "", escSpaceCount, escTxt, "", escSpaceCount + escRemainder);
    fmt::print("│                                                                                       │\n");
    fmt::print("└───────────────────────────────────────────────────────────────────────────────────────┘\n");
}
#pragma endregion

#pragma region INPUT_FUNCTIONS
static char getChar() {
#if defined(_WIN32)
    return _getch();
#else
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);                 // Get current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);               // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);        // Apply new settings
    ssize_t bytesRead = read(STDIN_FILENO, &ch, 1); // Read one character
    if (bytesRead != 1) {
        // Handle error or unexpected input length
        fmt::print("\n[{}] Error: Failed to read input stream! Expected 1 character got {}.\n", fmt::styled("ERROR", fmt::fg(fmt::color::red)), bytesRead);
        exit(EXIT_FAILURE);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);        // Restore original settings
    return ch;
#endif
}

static bool checkForEsc(char ch)
{
    if (ch == 27) {  // ASCII code for ESC key
        fmt::print("\n[{}] Program exited – ESC key pressed.\n", fmt::styled("EXIT", fmt::fg(fmt::color::dark_red)));
        if (selectedShape) delete selectedShape;
        exit(EXIT_SUCCESS);  // Exit the program if ESC is pressed
    }
    return false;
}

static int getIntInput(const std::string& prompt)
{
    std::string input = "";
    char ch;

    fmt::print("{}", prompt);
#if !defined(_WIN32)
    std::cout << std::flush;
#endif

    while (true) {
        ch = getChar();  // Get a single character from the user

        // Check if ESC key is pressed
        checkForEsc(ch);

        // If Enter is pressed, stop input
#if defined(_WIN32)
        if (ch == '\r') {
#else
        if (ch == '\n') {
#endif
            if (input.empty() || input == "-") {
                fmt::print("\n[{}] Invalid input! Please enter a number.\n{}", fmt::styled("WARNING", fmt::fg(fmt::color::yellow)), prompt);
#if !defined(_WIN32)
                std::cout << std::flush;
#endif
                input.clear();
            }
            else {
                std::cout << '\n';  // Add a new line after input is confirmed
                break;  // Valid input, exit the loop
            }
        }
        // If Backspace is pressed, remove the last character
        else if (ch == '\b' && !input.empty()) {
            std::cout << "\b \b";  // Erase the last character on the console
#if !defined(_WIN32)
            std::cout << std::flush;
#endif
            input.pop_back();      // Remove last character from string
        }
        // Handling the '-' character at the beginning of a number
        else if (ch == '-' && input.empty()) {
            input += ch;
            std::cout << ch;
#if !defined(_WIN32)
            std::cout << std::flush;
#endif
        }
        // Check if the character is a digit
        else if (isdigit(ch)) {
            input += ch;           // Append the digit to the input string
            std::cout << ch;       // Echo the character to the console
#if !defined(_WIN32)
            std::cout << std::flush;
#endif
        }
        // Ignore non-digit characters (other than backspace or enter)
    }

    // Convert the collected string to an integer
    int value = std::stoi(input);
    return value;
}

static float getFloatInput(const std::string& prompt)
{
    std::string input = "";
    char ch;
    bool hasDot = false;  // Flag to check if user has already entered a dot

    fmt::print("{}", prompt);
#if !defined(_WIN32)
        std::cout << std::flush;
#endif

    while (true) {
        ch = getChar();  // Get a single character from the user

        // Check if ESC key is pressed
        checkForEsc(ch);

        // If Enter is pressed, stop input
#if defined(_WIN32)
        if (ch == '\r') {
#else
        if (ch == '\n') {
#endif
            if (input.empty() || input == "-") {
                fmt::print("\n[{}] Invalid input! Please enter a number.\n{}", fmt::styled("WARNING", fmt::fg(fmt::color::yellow)), prompt);
#if !defined(_WIN32)
                std::cout << std::flush;
#endif
                input.clear();
            }
            else {
                std::cout << '\n';  // Add a new line after input is confirmed
                break;
            }
        }
        // If Backspace is pressed, remove the last character
        else if (ch == '\b' && !input.empty()) {
            if (input.back() == '.') {
                hasDot = false;  // Removing the dot resets the flag
            }
            std::cout << "\b \b";
#if !defined(_WIN32)
            std::cout << std::flush;
#endif
            input.pop_back();
        }
        // Handling the '-' character at the beginning of a number
        else if (ch == '-' && input.empty()) {
            input += ch;
            std::cout << ch;
#if !defined(_WIN32)
            std::cout << std::flush;
#endif
        }
        // Check if the character is a digit
        else if (isdigit(ch)) {
            input += ch;
            std::cout << ch;
#if !defined(_WIN32)
            std::cout << std::flush;
#endif
        }
        // Decimal point handling (one time)
        else if (ch == '.' && !hasDot && !input.empty() && input != "-") {
            input += ch;
            hasDot = true;
            std::cout << ch;
#if !defined(_WIN32)
            std::cout << std::flush;
#endif
        }
        // Ignore non-digit characters (other than backspace or enter)
    }

    // Convert the collected string to an float
    float value = std::stof(input);
    return value;
}

static void waitForEnter(const std::string& prompt)
{
    char ch;

    fmt::print("{}", prompt);
#if !defined(_WIN32)
    std::cout << std::flush;
#endif

    while (true) {
        ch = getChar();  // Get a single character from the user

        // Check if ESC key is pressed
        checkForEsc(ch);

        // If Enter is pressed, stop input
#if defined(_WIN32)
        if (ch == '\r') {
#else
        if (ch == '\n') {
#endif
            std::cout << '\n';  // Add a new line after input is confirmed
            break;  // Valid input, exit the loop
        }
        // Ignore non-digit characters (other than enter)
    }
}

static int intChooseInput(int min, int max)
{
    return getIntInput(getInputPrompt(min, max));
}
#pragma endregion

#pragma region SHAPE_FUNCTIONS
static PlaneNormalDir getPlaneDirection()
{
    int dir_choice;
    do {
        fmt::print("\n> Select the plane orientation:\n1) UP\n2) FRONT\n");
        dir_choice = getIntInput(getInputPrompt(1, 2));
        if (dir_choice < 1 || dir_choice > 2) {
            printInvalidOption(1, 2);
        }
    } while (dir_choice < 1 || dir_choice > 2);

    return static_cast<PlaneNormalDir>(dir_choice - 1);
}

static ValuesRange getValuesRange()
{
    int dir_choice;
    do {
        fmt::print("\n> Select the value range for object vertices:\n1) [-0.5, 0.5]\n2) [-1.0, 1.0]\n");
        dir_choice = getIntInput(getInputPrompt(1, 2));
        if (dir_choice < 1 || dir_choice > 2) {
            printInvalidOption(1, 2);
        }
    } while (dir_choice < 1 || dir_choice > 2);

    return static_cast<ValuesRange>(dir_choice - 1);
}

static FormatType getFormatType()
{
    static const std::vector<std::string> options {
        "std::vector — Vertices & Indices (struct)",
        "C array     — Vertices & Indices (struct)",
        "std::vector — Vertices only (struct)",
        "C array     — Vertices only (struct)",

        "std::vector — Vertices & Indices (float)",
        "C array     — Vertices & Indices (float)",
        "std::vector — Vertices only (float)",
        "C array     — Vertices only (float)",

        "Export — JSON — Vertices & Indices",
        "Export — JSON — Vertices only",
        "Export — OBJ"
    };

    static const size_t optSize = options.size();

    int arr_choice;
    do {
        fmt::print("\n──────────────────────────────────────────────────\n");
        fmt::print("[{}] Select the save format:\n", fmt::styled("FILE", fmt::fg(fmt::color::alice_blue)));
        for (size_t i = 0ull; i < optSize; ++i) {
            fmt::print(" {}) {}\n", i + 1ull, options[i]);
        }
        fmt::print("──────────────────────────────────────────────────\n");

        arr_choice = getIntInput(getInputPrompt(1, static_cast<int>(optSize)));
        if (arr_choice < 1 || arr_choice > static_cast<int>(optSize)) {
            printInvalidOption(1, static_cast<int>(optSize));
        }
    } while (arr_choice < 1 || arr_choice > static_cast<int>(optSize));

    return static_cast<FormatType>(arr_choice - 1);
}
#pragma endregion

#pragma region FILE_FUNCTIONS
static std::string getFormatFileExtension(const FormatType& format)
{
    std::string ext = ".txt";

    if (format == FormatType::OBJ)
    {
        ext = ".obj";
    }
    else if (format == FormatType::JSON_INDICES || format == FormatType::JSON_VERTICES)
    {
        ext = ".json";
    }

    return ext;
}
#pragma endregion

using namespace config;
using namespace system_functions;

int main(int argc, char** argv)
{
#pragma region INIT
#if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    std::string exeDirPath = get_executable_path();
    ValuesRange range = ValuesRange::HALF_TO_HALF;
    const Config& config = get_config(exeDirPath);

    displayStartWindow();
#pragma endregion

#pragma region SELECT_SHAPE
    int choice;
    do {
        fmt::print("\n> Select a shape to create:\n");
        std::cout << "1) Sphere\n2) IcoSphere\n3) Plane\n4) Cube\n5) Cylinder\n6) Hexagon\n7) Cone\n8) Tetrahedron\n9) Pyramid\n10) Torus\n";
        choice = intChooseInput(1, 10);
        if (choice < 1 || choice > 10) {
            printInvalidOption(1, 10);
        }
    } while (choice < 1 || choice > 10);
#pragma endregion

#pragma region SHAPE_GENERATION
    range = getValuesRange();
    std::chrono::duration<double> elapsed_seconds;
    ShapeConfig shapeConfig = {
        config.genTangents,
        config.calcBitangents,
        config.tangentHandednessPositive
    };

    switch (choice) {
        case 1: {
            fmt::print("\n> Enter sphere parameters:\n");
            int horizontal = getIntInput("   - Number of horizontal segments (min: 2): ");
            if (horizontal < 2) {
                fmt::print("[{}] Adjusted to minimum of 2 horizontal segments.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                horizontal = 2;
            }

            int vertical = getIntInput("   - Number of vertical segments (min: 3): ");
            if (vertical < 3) {
                fmt::print("[{}] Adjusted to minimum of 3 vertical segments.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                vertical = 3;
            }

            SphereShading shade = getShadingType<SphereShading>("sphere", intChooseInput, printInvalidOption);

            elapsed_seconds = generateShape<Sphere>(selectedShape, shapeConfig, horizontal, vertical, shade, range);
            break;
        }
        case 2: {
            fmt::print("\n> Enter icoSphere parameters:\n");
            int subs = getIntInput("   - Number of subdivisions (0 - 8): ");
            if (subs < 0) {
                fmt::print("[{}] Adjusted to minimum of 0 subdivisions.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                subs = 0;
            }
            else if (subs > 8) {
                fmt::print("[{}] Adjusted to maximum of 8 subdivisions.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                subs = 8;
            }

            IcoSphereShading shade = getShadingType<IcoSphereShading>("icoSphere", intChooseInput, printInvalidOption);

            elapsed_seconds = generateShape<IcoSphere>(selectedShape, shapeConfig, subs, shade, range);
            break;
        }
        case 3: {
            fmt::print("\n> Enter plane dimensions:\n");
            int rows = getIntInput("   - Number of rows (min: 2): ");
            if (rows < 2) {
                fmt::print("[{}] Adjusted to minimum of 2 rows.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                rows = 2;
            }

            int columns = getIntInput("   - Number of columns (min: 2): ");
            if (columns < 2) {
                fmt::print("[{}] Adjusted to minimum of 2 columns.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                columns = 2;
            }

            PlaneNormalDir dir = getPlaneDirection();

            elapsed_seconds = generateShape<Plane>(selectedShape, shapeConfig, rows, columns, dir, range);
            break;
        }
        case 4: elapsed_seconds = generateShape<Cube>(selectedShape, shapeConfig, range); break;
        case 5: {
            fmt::print("\n> Enter cylinder parameters:\n");
            int verticalSegments = getIntInput("   - Number of vertical segments (min: 3): ");
            if (verticalSegments < 3) {
                fmt::print("[{}] Adjusted to minimum of 3 vertical segments.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                verticalSegments = 3;
            }
            int horizontalSegments = getIntInput("   - Number of horizontal segments (min: 1): ");
            if (horizontalSegments < 1) {
                fmt::print("[{}] Adjusted to minimum of 1 horizontal segments.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                horizontalSegments = 1;
            }

            CylinderShading shade = getShadingType<CylinderShading>("cylinder", intChooseInput, printInvalidOption);

            elapsed_seconds = generateShape<Cylinder>(selectedShape, shapeConfig, horizontalSegments, verticalSegments, shade, range);
            break;
        }
        case 6: {
            fmt::print("\n> Enter hexagon parameters:\n");
            int horizontalSegments = getIntInput("   - Number of horizontal segments (min: 1): ");
            if (horizontalSegments < 1) {
                fmt::print("[{}] Adjusted to minimum of 1 horizontal segments.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                horizontalSegments = 1;
            }

            elapsed_seconds = generateShape<Hexagon>(selectedShape, shapeConfig, horizontalSegments, range);
            break;
        }
        case 7: {
            fmt::print("\n> Enter cone parameters:\n");
            int segments = getIntInput("   - Number of segments (min: 3): ");
            if (segments < 3) {
                fmt::print("[{}] Adjusted to minimum of 3 segments.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                segments = 3;
            }

            float height = getFloatInput("   - Height (greater than: 0.0): ");
            if (height <= 0.0f) {
                fmt::print("[{}] Adjusted height to 1.0.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                height = 1.0f;
            }

            float radius = getFloatInput("   - Radius (greater than: 0.0): ");
            if (radius <= 0.0f) {
                fmt::print("[{}] Adjusted radius to 1.0.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                radius = 1.0f;
            }

            ConeShading shade = getShadingType<ConeShading>("cone", intChooseInput, printInvalidOption);

            elapsed_seconds = generateShape<Cone>(selectedShape, shapeConfig, segments, height, radius, shade, range);
            break;
        }
        case 8: elapsed_seconds = generateShape<Tetrahedron>(selectedShape, shapeConfig, range); break;
        case 9: elapsed_seconds = generateShape<Pyramid>(selectedShape, shapeConfig, range); break;
        case 10: {
            fmt::print("\n> Enter torus parameters:\n");
            int segments = getIntInput("   - Number of segments for the main axis of the circle (min: 3): ");
            if (segments < 3) {
                fmt::print("[{}] Adjusted to minimum of 3 segments.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                segments = 3;
            }

            int cs_segments = getIntInput("   - Number of segments for the inner circle of revolution (min: 3): ");
            if (cs_segments < 3) {
                fmt::print("[{}] Adjusted to minimum of 3 segments.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                cs_segments = 3;
            }

            float radius = getFloatInput("   - Radius of the main circle (greater than: 0.0): ");
            if (radius <= 0.0f) {
                fmt::print("[{}] Adjusted radius to 1.0.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                radius = 1.0f;
            }

            float cs_radius = getFloatInput("   - Radius of the circle of revolution (greater than: 0.0): ");
            if (cs_radius <= 0.0f) {
                fmt::print("[{}] Adjusted radius to 1.0.\n", fmt::styled("INFO", fmt::fg(fmt::color::white)));
                cs_radius = 1.0f;
            }

            TorusShading shade = getShadingType<TorusShading>("torus", intChooseInput, printInvalidOption);

            elapsed_seconds = generateShape<Torus>(selectedShape, shapeConfig, segments, cs_segments, radius, cs_radius, shade, range);
            break;
        }
    }
#pragma endregion

#pragma region SHAPE_GENERATED_INFO
    if (!selectedShape) {
        fmt::print("\n[{}] Error: Failed to generate shape!\n", fmt::styled("ERROR", fmt::fg(fmt::color::red)));
        return EXIT_FAILURE;
    }

    fmt::print("\n[{}] Shape successfully generated in {}s!\n", fmt::styled("INFO", fmt::fg(fmt::color::white)), elapsed_seconds.count());
#pragma endregion

#pragma region SAVE_SHAPE
    FormatType format = getFormatType();

    if (!check_directory(config.saveDir.c_str())) {
        if (create_directory(config.saveDir.c_str())) {
            fmt::print("\n[{}] Directory created successfully: {}\n", fmt::styled("CREATED", fmt::fg(fmt::color::green)), config.saveDir);
        }
        else {
            fmt::print("\n[{}] Error: Could not create directory: {}\n", fmt::styled("ERROR", fmt::fg(fmt::color::red)), config.saveDir);
            delete selectedShape;
            return EXIT_FAILURE;
        }
    }

    std::string filePath = config.saveDir + DIRSEP + get_resolved_file_name(config, selectedShape->getObjectClassName()) + getFormatFileExtension(format);

    fmt::print("\n[{}] Start Saving {} to file...\n", fmt::styled("OK", fmt::fg(fmt::color::green)), selectedShape->getObjectClassName());
    auto start = std::chrono::system_clock::now();

    std::ofstream file(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

    if (file.is_open()) {
        std::string text = selectedShape->toString(format);
        file.write(text.data(), text.size());
        file.close();

        auto end = std::chrono::system_clock::now();

        elapsed_seconds = (end - start);

        std::string separator = std::string(1, DIRSEP);
        replace_all(filePath, separator + separator, separator);

        fmt::print("[{}] Shape saved successfully in {}s!\n[{}] File path: {}\n",
            fmt::styled("SAVED", fmt::fg(fmt::color::green)),
            elapsed_seconds.count(),
            fmt::styled("PATH", fmt::fg(fmt::color::white)), filePath);

        if (config.openDirOnSave)
        {
            bool res = open_in_file_explorer(filePath.c_str());

            if (!res)
            {
                fmt::print("[{}] Error: Could not open file explorer!\n", fmt::styled("ERROR", fmt::fg(fmt::color::red)));
            }
        }
    }
    else {
        fmt::print("[{}] Error: Could not save the file!\n", fmt::styled("ERROR", fmt::fg(fmt::color::red)));
    }
#pragma endregion
    
#pragma region EXIT
    waitForEnter("\nPress Enter to exit...");
    delete selectedShape;
    return EXIT_SUCCESS;
#pragma endregion
}