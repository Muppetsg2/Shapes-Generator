// PRECOMPILED HEADER
#include "pch.h"

// MY FILES
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Hexagon.h"
#include "Pyramid.h"
#include "Tetrahedron.h"
#include "IcoSphere.h"
#include "templates.h"

Shape* selectedShape = nullptr;

void replace_all(std::string& s, std::string const& toReplace, std::string const& replaceWith)
{
    std::string buf;
    std::size_t pos = 0;
    std::size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}

bool checkForEsc(char ch) {
    if (ch == 27) {  // ASCII code for ESC key
        fmt::print("\n[{}] Program exited – ESC key pressed.\n", fmt::styled("EXIT", fmt::fg(fmt::color::dark_red)));
        if (selectedShape) delete selectedShape;
        exit(EXIT_SUCCESS);  // Exit the program if ESC is pressed
    }
    return false;
}

int getIntInput(const std::string& prompt) {
    std::string input = "";
    char ch;

    fmt::print("{}", prompt);

    while (true) {
        ch = _getch();  // Get a single character from the user

        // Check if ESC key is pressed
        checkForEsc(ch);

        // If Enter is pressed, stop input
        if (ch == '\r') {  // ASCII code for Enter
            if (input.empty()) {
                fmt::print("[{}] Invalid input! Please enter a number.\n{}", fmt::styled("WARNING", fmt::fg(fmt::color::yellow)), prompt);
            }
            else {
                std::cout << '\n';  // Add a new line after input is confirmed
                break;  // Valid input, exit the loop
            }
        }
        // If Backspace is pressed, remove the last character
        else if (ch == '\b' && !input.empty()) {
            std::cout << "\b \b";  // Erase the last character on the console
            input.pop_back();      // Remove last character from string
        }
        // Check if the character is a digit
        else if (isdigit(ch)) {
            input += ch;           // Append the digit to the input string
            std::cout << ch;       // Echo the character to the console
        }
        // Ignore non-digit characters (other than backspace or enter)
    }

    // Convert the collected string to an integer
    int value = std::stoi(input);
    return value;
}

void waitForEnter(const std::string& prompt) {
    char ch;

    fmt::print("{}", prompt);

    while (true) {
        ch = _getch();  // Get a single character from the user

        // Check if ESC key is pressed
        checkForEsc(ch);

        // If Enter is pressed, stop input
        if (ch == '\r') {  // ASCII code for Enter
            std::cout << '\n';  // Add a new line after input is confirmed
            break;  // Valid input, exit the loop
        }
        // Ignore non-digit characters (other than enter)
    }
}

void displayStartWindow() {

    size_t lineLength = std::string("   ____  _                         ____                           _               ").size();
    std::string versionTxt = std::string("Version: ").append(SHAPES_GENERATOR_VERSION_STR);

    size_t spaceCount = (lineLength - versionTxt.size()) / 2ull;
    size_t remainder = (lineLength - versionTxt.size()) % 2ull;

    fmt::print("┌──────────────────────────────────────────────────────────────────────────────────┐\n");
    fmt::print("│                                                                                  │\n");
    fmt::print("│   ____  _                         ____                           _               │\n");
    fmt::print("│  / ___|| |__   __ _ _ __   ___   / ___| ___ _ __   ___ _ __ __ _| |_ ___  _ __   │\n");
    fmt::print("│  \\___ \\| '_ \\ / _` | '_ \\ / _ \\ | |  _ / _ \\ '_ \\ / _ \\ '__/ _` | __/ _ \\| '__|  │\n");
    fmt::print("│   ___) | | | | (_| | |_) |  __/ | |_| |  __/ | | |  __/ | | (_| | || (_) | |     │\n");
    fmt::print("│  |____/|_| |_|\\__,_| .__/ \\___|  \\____|\\___|_| |_|\\___|_|  \\__,_|\\__\\___/|_|     │\n");
    fmt::print("│                    |_|                                                           │\n");
    fmt::print("│                                                                                  │\n");
    fmt::print("│{:<{}}{}{:>{}}│\n", "", spaceCount, versionTxt, "", spaceCount + remainder);
    fmt::print("│                                                                                  │\n");
    fmt::print("│                           Press ESC to exit the program                          │\n");
    fmt::print("│                                                                                  │\n");
    fmt::print("└──────────────────────────────────────────────────────────────────────────────────┘\n");
}

void printInvalidOption(int firstOptionNum, int lastOptionNum) {
    fmt::print("[{}] Invalid option! Enter a number between {} and {} and try again.\n",
        fmt::styled("WARNING", fmt::fg(fmt::color::yellow)), firstOptionNum, lastOptionNum);
}

std::string getInputPrompt(int firstOptionNum, int lastOptionNum) {
    return fmt::format("Enter your choice ({} - {}): ", firstOptionNum, lastOptionNum);
}

PlaneNormalDir getPlaneDirection() {
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

IcoSphereShading getShadingType() {
    int dir_choice;
    do {
        fmt::print("\n> Select the icoSphere shading type:\n1) FLAT\n2) PHONG\n");
        dir_choice = getIntInput(getInputPrompt(1, 2));
        if (dir_choice < 1 || dir_choice > 2) {
            printInvalidOption(1, 2);
        }
    } while (dir_choice < 1 || dir_choice > 2);

    return static_cast<IcoSphereShading>(dir_choice - 1);
}

ValuesRange getValuesRange() {
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

FormatType getFormatType() {
    static const std::vector<std::string> options = {
        "std::vector - Vertices & Indices",
        "C++ array - Vertices & Indices",
        "std::vector - Only Vertices",
        "C++ array - Only Vertices",
        "Save as OBJ file"
    };

    int arr_choice;
    do {
        fmt::print("\n──────────────────────────────────────────────────\n");
        fmt::print("[{}] Select the save format:\n", fmt::styled("FILE", fmt::fg(fmt::color::alice_blue)));
        for (size_t i = 0; i < options.size(); ++i) {
            fmt::print(" {}) {}\n", i + 1, options[i]);
        }
        fmt::print("──────────────────────────────────────────────────\n");

        arr_choice = getIntInput(getInputPrompt(1, (int)options.size()));
        if (arr_choice < 1 || arr_choice > static_cast<int>(options.size())) {
            printInvalidOption(1, (int)options.size());
        }
    } while (arr_choice < 1 || arr_choice > static_cast<int>(options.size()));

    return static_cast<FormatType>(arr_choice - 1);
}

int main(int argc, char** argv) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    std::string exeDirPath = std::filesystem::absolute(argv[0]).parent_path().string();
    ValuesRange range = ValuesRange::HALF_TO_HALF;

    displayStartWindow();

    int choice;
    do {
        fmt::print("\n> Select a shape to create:\n");
        std::cout << "1) Sphere\n2) Plane\n3) Cube\n4) Hexagon\n5) Pyramid\n6) Tetrahedron\n7) IcoSphere\n";
        choice = getIntInput(getInputPrompt(1, 7));
        if (choice < 1 || choice > 7) {
            printInvalidOption(1, 7);
        }
    } while(choice < 1 || choice > 7);

    range = getValuesRange();

    std::chrono::duration<double> elapsed_seconds;
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
            elapsed_seconds = generateShape<Sphere>(selectedShape, horizontal, vertical, range);
            break;
        }
        case 2: {
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
            elapsed_seconds = generateShape<Plane>(selectedShape, rows, columns, dir, range);
            break;
        }
        case 3: elapsed_seconds = generateShape<Cube>(selectedShape, range); break;
        case 4: elapsed_seconds = generateShape<Hexagon>(selectedShape, range); break;
        case 5: elapsed_seconds = generateShape<Pyramid>(selectedShape, range); break;
        case 6: elapsed_seconds = generateShape<Tetrahedron>(selectedShape, range); break;
        case 7: {
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

            IcoSphereShading shade = getShadingType();

            elapsed_seconds = generateShape<IcoSphere>(selectedShape, subs, shade, range);
            break;
        }
    }

    if (!selectedShape) {
        fmt::print("\n[{}] Error: Failed to generate the shape!\n", fmt::styled("ERROR", fmt::fg(fmt::color::red)));
        return EXIT_FAILURE;
    }

    fmt::print("\n[{}] Shape successfully generated in {}s!\n", fmt::styled("INFO", fmt::fg(fmt::color::white)), elapsed_seconds.count());

    FormatType format = getFormatType();
    std::string filePath = exeDirPath + ((format != FormatType::OBJ) ? "\\shape.txt" : "\\shape.obj");

    fmt::print("\n[{}] Start Saving {} to file...\n", fmt::styled("OK", fmt::fg(fmt::color::green)), selectedShape->getObjectClassName());
    auto start = std::chrono::system_clock::now();

    std::ofstream file(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

    if (file) {
        std::string text = selectedShape->toString(format);
        file.write(text.data(), text.size());
        file.close();

        auto end = std::chrono::system_clock::now();

        elapsed_seconds = (end - start);

        std::string path = std::filesystem::absolute(filePath).string();
        replace_all(path, "\\\\", "\\");

        fmt::print("[{}] Shape saved successfully in {}s!\n[{}] File path: {}\n",
            fmt::styled("SAVED", fmt::fg(fmt::color::green)),
            elapsed_seconds.count(),
            fmt::styled("PATH", fmt::fg(fmt::color::white)), path);
    }
    else {
        fmt::print("[{}] Error: Could not save the file!\n", fmt::styled("ERROR", fmt::fg(fmt::color::red)));
    }
    
    waitForEnter("\nPress Enter to exit...");
    delete selectedShape;
    return EXIT_SUCCESS;
}