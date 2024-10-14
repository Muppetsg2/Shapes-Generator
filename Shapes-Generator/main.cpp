// STANDARD LIBS
#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <conio.h>
#include <cstdlib>
#include <cctype>

#define FMT_HEADER_ONLY
#include <fmt/core.h>

// MY FILES
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Hexagon.h"
#include "Pyramid.h"
#include "Tetrahedron.h"

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
        std::cout << "\nProgram exited by ESC key.\n";
        exit(0);  // Exit the program if ESC is pressed
    }
    return false;
}

int getIntInput(const std::string& prompt) {
    std::string input = "";
    char ch;

    std::cout << prompt;

    while (true) {
        ch = _getch();  // Get a single character from the user

        // Check if ESC key is pressed
        checkForEsc(ch);

        // If Enter is pressed, stop input
        if (ch == '\r') {  // ASCII code for Enter
            if (input.empty()) {
                std::cout << "\nInvalid input! Please enter a number.\n" << prompt;
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

void displayStartWindow() {
    fmt::print("┌──────────────────────────────────────────────────────────────────────────────────┐\n");
    fmt::print("│                                                                                  │\n");
    fmt::print("│   ____  _                         ____                           _               │\n");
    fmt::print("│  / ___|| |__   __ _ _ __   ___   / ___| ___ _ __   ___ _ __ __ _| |_ ___  _ __   │\n");
    fmt::print("│  \\___ \\| '_ \\ / _` | '_ \\ / _ \\ | |  _ / _ \\ '_ \\ / _ \\ '__/ _` | __/ _ \\| '__|  │\n");
    fmt::print("│   ___) | | | | (_| | |_) |  __/ | |_| |  __/ | | |  __/ | | (_| | || (_) | |     │\n");
    fmt::print("│  |____/|_| |_|\\__,_| .__/ \\___|  \\____|\\___|_| |_|\\___|_|  \\__,_|\\__\\___/|_|     │\n");
    fmt::print("│                    |_|                                                           │\n");
    fmt::print("│                                                                                  │\n");
    fmt::print("│                             Choose a shape to create                             │\n");
    fmt::print("│                                                                                  │\n");
    fmt::print("│                           Press ESC to exit the program                          │\n");
    fmt::print("│                                                                                  │\n");
    fmt::print("└──────────────────────────────────────────────────────────────────────────────────┘\n");
}

PlaneNormalDir getPlaneDirection() {
    int dir_choice;
    while (true) {
        std::cout << "Choose a direction:\n";
        std::cout << "1. UP\n";
        std::cout << "2. FRONT\n";
        dir_choice = getIntInput("Your choice: ");
        if (dir_choice >= 1 && dir_choice <= 2) {
            return static_cast<PlaneNormalDir>(dir_choice - 1);
        }
        else {
            std::cout << "Invalid direction! Please choose a number between 1 and 2.\n";
        }
    }
}

int main()
{
    Shape* selectedShape = nullptr;

    displayStartWindow();

    int choice = 0;
    // Loop to check for a valid shape choice
    while (true) {
        std::cout << "\nChoose a shape:\n";
        std::cout << "1. Sphere\n";
        std::cout << "2. Plane\n";
        std::cout << "3. Cube\n";
        std::cout << "4. Hexagon\n";
        std::cout << "5. Pyramid\n";
        std::cout << "6. Tetrahedron\n";

        choice = getIntInput("Your choice: ");
        if (choice >= 1 && choice <= 6) {
            break;  // Valid choice, exit loop
        }
        else {
            std::cout << "Invalid choice! Please choose a number between 1 and 6.\n";
        }
    }

    std::chrono::duration<double> elapsed_seconds;
    switch (choice) {
        case 1: {
            int horizontal = getIntInput("Enter sphere number of horizontal segments (>= 2): ");
            int vertical = getIntInput("Enter sphere number of vertical segments (>= 3): ");

            if (horizontal < 2) std::cout << "Number of horizontal segments was lower than 2, so 2 segments were used during generation.\n";
            if (vertical < 3) std::cout << "Number of vertical segments was lower than 3, so 3 segments were used during generation.\n";

            std::cout << "Start Generating Sphere!\n";
            auto start = std::chrono::system_clock::now();
            selectedShape = new Sphere(horizontal, vertical);
            auto end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            break;
        }
        case 2: {
            int rows = getIntInput("Enter plane number of rows (>= 2): ");
            int columns = getIntInput("Enter plane number of columns (>= 2): ");
            PlaneNormalDir dir = getPlaneDirection();

            if (rows < 2) std::cout << "Number of rows was lower than 2, so 2 rows were used during generation.\n";
            if (columns < 2) std::cout << "Number of columns was lower than 2, so 2 columns were used during generation.\n";

            std::cout << "Start Generating Plane!\n";
            auto start = std::chrono::system_clock::now();
            selectedShape = new Plane(rows, columns, dir);
            auto end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            break;
        }
        case 3: {
            std::cout << "Start Generating Cube!\n";
            auto start = std::chrono::system_clock::now();
            selectedShape = new Cube();
            auto end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            break;
        }
        case 4: {
            std::cout << "Start Generating Hexagon!\n";
            auto start = std::chrono::system_clock::now();
            selectedShape = new Hexagon();
            auto end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            break;
        }
        case 5: {
            std::cout << "Start Generating Pyramid!\n";
            auto start = std::chrono::system_clock::now();
            selectedShape = new Pyramid();
            auto end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            break;
        }
        case 6: {
            std::cout << "Start Generating Tetrahedron!\n";
            auto start = std::chrono::system_clock::now();
            selectedShape = new Tetrahedron();
            auto end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            break;
        }
    }

    if (selectedShape == nullptr) {
        return 0;
    }

    std::cout << "Shape Generated in " << elapsed_seconds.count() << "s!\n";

    fstream file;
    file.open("./shape.txt", std::ios::out | std::ios::trunc);

    file << selectedShape->toString();

    file.close();

    std::string path = std::filesystem::absolute("./shape.txt").string();
    replace_all(path, "\\\\", "\\");

    std::cout << "Shape saved to file: " << path << "\n";

    delete selectedShape;

    return 0;
}