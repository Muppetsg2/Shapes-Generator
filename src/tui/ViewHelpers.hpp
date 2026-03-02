#pragma once

#pragma region STD_LIBS
#include <chrono>
#include <cstdint>
#include <ctype.h>
#include <ratio>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#pragma endregion

class Shape;
struct ShapeConfig;

namespace tui
{
    enum class Check : uint8_t
    {
        NO_EQUAL = 0,
        EQUAL    = 1 << 0,
        LESS     = 1 << 1,
        GREATER  = 1 << 2,

        LESS_EQUAL    = LESS    | EQUAL,
        GREATER_EQUAL = GREATER | EQUAL
    };

    class CheckFlags
    {
    public:
        CheckFlags() = default;
        CheckFlags(uint8_t flags) : _flags(flags) {}
        CheckFlags(Check flag) : _flags(static_cast<uint8_t>(flag)) {}

        CheckFlags& operator=(Check flag) {
            _flags = static_cast<uint8_t>(flag);
            return *this;
        }

        CheckFlags operator|(Check flag) const
        {
            return CheckFlags(_flags | static_cast<uint8_t>(flag));
        }

        CheckFlags operator|(CheckFlags flags) const
        {
            return CheckFlags(_flags | flags._flags);
        }

        bool operator==(uint8_t flags) {
            return _flags == flags;
        }

        bool operator==(Check flag) {
            return _flags == static_cast<uint8_t>(flag);
        }

        bool operator==(CheckFlags flags) {
            return _flags == flags._flags;
        }

        bool HasFlag(Check flag) const
        {
            return (_flags & static_cast<uint8_t>(flag)) != 0;
        }

    private:
        uint8_t _flags = 0;
    };

    static bool CanBeConvertedToInt(const std::string& str, int& number) {
        if (str.empty())
            return false;

        bool canBeConverted = true;

        for (size_t i = 0; i < str.size(); ++i) {
            char ch = str[i];
            if (!(isdigit(ch) || (ch == '-' && i == 0 && str.size() != 1))) {
                canBeConverted = false;
                break;
            }
        }

        if (canBeConverted) {
            try {
                number = std::stoi(str);
            }
            catch (...) {
                return false;
            }
        }

        return canBeConverted;
    }

    static void ValidateInt(const std::string& str, int valueToCheck, int& outValue, bool& isValid, CheckFlags checkFlags = Check::EQUAL)
    {
        isValid = false;
        int parsed = 0;

        if (!CanBeConvertedToInt(str, parsed)) { return; }

        bool conditionMet = false;

        if (checkFlags == Check::NO_EQUAL)
        {
            conditionMet = (parsed != valueToCheck);
        }
        else
        {
            if (checkFlags.HasFlag(Check::EQUAL))
            {
                conditionMet |= (parsed == valueToCheck);
            }

            if (checkFlags.HasFlag(Check::LESS))
            {
                conditionMet |= (parsed < valueToCheck);
            }

            if (checkFlags.HasFlag(Check::GREATER))
            {
                conditionMet |= (parsed > valueToCheck);
            }
        }

        if (conditionMet)
        {
            outValue = parsed;
            isValid = true;
        }
    }

    static void ValidateIntInRange(const std::string& str, int minValueInclusive, int maxValueInclusive, int& outValue, bool& isValid)
    {
        ValidateInt(str, minValueInclusive, outValue, isValid, Check::GREATER_EQUAL);

        if (isValid)
        {
            isValid &= (outValue <= maxValueInclusive);
        }
    }

    static bool CanBeConvertedToFloat(const std::string& str, float& number) {
        if (str.empty())
            return false;

        bool hasDot = false;
        bool canBeConverted = true;

        for (size_t i = 0; i < str.size(); ++i) {
            char ch = str[i];
            if (!(isdigit(ch) || (str.size() > 1 && ((ch == '-' && i == 0) || (ch == '.' && !hasDot))))) {
                canBeConverted = false;
                break;
            }

            if (ch == '.') hasDot = true;
        }

        if (canBeConverted) {
            try {
                number = std::stof(str);
            }
            catch (...) {
                return false;
            }
        }

        return canBeConverted;
    }

    static void ValidateFloat(const std::string& str, float valueToCheck, float& outValue, bool& isValid, CheckFlags checkFlags = Check::EQUAL)
    {
        isValid = false;
        float parsed = 0.0f;

        if (!CanBeConvertedToFloat(str, parsed)) { return; }

        bool conditionMet = false;

        if (checkFlags == Check::NO_EQUAL)
        {
            conditionMet = (parsed != valueToCheck);
        }
        else
        {
            if (checkFlags.HasFlag(Check::EQUAL))
            {
                conditionMet |= (parsed == valueToCheck);
            }

            if (checkFlags.HasFlag(Check::LESS))
            {
                conditionMet |= (parsed < valueToCheck);
            }

            if (checkFlags.HasFlag(Check::GREATER))
            {
                conditionMet |= (parsed > valueToCheck);
            }
        }

        if (conditionMet)
        {
            outValue = parsed;
            isValid = true;
        }
    }

    static void ValidateFloatInRange(const std::string& str, float minValueInclusive, float maxValueInclusive, float& outValue, bool& isValid)
    {
        ValidateFloat(str, minValueInclusive, outValue, isValid, Check::GREATER_EQUAL);

        if (isValid)
        {
            isValid &= (outValue <= maxValueInclusive);
        }
    }

    static ftxui::Element ColorValueLabel(std::string basePart, std::string colorPart, bool valid) {
        using namespace ftxui;

        auto base = text(basePart);
        auto minPart = text(colorPart);

        if (valid)
            minPart = minPart | dim;
        else
            minPart = minPart | bold | color(Color::Red);

        return hbox({ base, minPart, text(": ") });
    }

    static ftxui::Element RadioWithLabel(std::string label, ftxui::Component radio) {
        using namespace ftxui;
        return vbox({ text(label) | bold | color(Color::BlueLight), radio->Render() });
    }

    static ftxui::Element StyleKey(std::string label)
    {
        using namespace ftxui;
        return text("[" + label + "]") | bold | color(Color::YellowLight);
    }

    struct KeyInfo
    {
        std::string keyName;
        std::string label;
    };

    static ftxui::Element ControlsInfo(std::vector<KeyInfo> keys)
    {
        using namespace ftxui;
        Elements keyElements;
        for (size_t i = 0; i < keys.size(); ++i)
        {
            const KeyInfo& key = keys[i];
            keyElements.push_back(StyleKey(key.keyName));
            keyElements.push_back(text(" " + key.label + "   ") | dim);

            if (i + 1 != keys.size())
            {
                keyElements.push_back(text("| ") | dim);
            }
        }

        return vbox({ separator(), hbox(keyElements) | hcenter });
    }

    template<class ShapeType, class... Args>
    static std::pair<Shape*, std::chrono::duration<double, std::milli>> GenerateShape(const ShapeConfig& config, bool isFast, const Args&... args) {
        auto start = std::chrono::high_resolution_clock::now();

        Shape* shape = new ShapeType(config, args...);

        auto end = std::chrono::high_resolution_clock::now();

        if (isFast)
        {
            // It is so fast that i didn't saw animation
            std::this_thread::sleep_for(std::chrono::milliseconds(50000 / 60));
        }
        
        return {
            shape,
            std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start)
        };
    }
}