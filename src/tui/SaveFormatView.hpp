#pragma once

#pragma region STD_LIBS
#include <chrono>
#include <functional>
#include <string>
#include <ratio>
#include <vector>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#pragma endregion

#pragma region MY_FILES
#include "View.hpp"
#pragma endregion

namespace tui {
    class SaveFormatView : public View {
    public:
        SaveFormatView(std::string& shapeName, std::chrono::duration<double, std::milli>& time, std::function<void(int)> onSave, std::function<void()> onCancel);

        ftxui::Component GetComponent() override;

        bool OnEvent(ftxui::Event event) override;

    private:
        std::string& _shapeName;
        std::chrono::duration<double, std::milli>& _time;

        std::function<void(int)> _onSave;
        std::function<void()> _onCancel;

        int _selectedFormat = 0;
        const std::vector<std::string> _formatOptions = {
            "std::array - Vertices & Indices",
            "C array    - Vertices & Indices",
            "std::array - Vertices only",
            "C array    - Vertices only",

            "std::array - Vertices & Indices",
            "C array    - Vertices & Indices",
            "std::array - Vertices only",
            "C array    - Vertices only",

            "JSON - Vertices & Indices",
            "JSON - Vertices only",
            "OBJ"
        };

        ftxui::Component _formatRadio;
        ftxui::Component _saveButton;
        ftxui::Component _cancelButton;
        ftxui::Component _container;
        ftxui::Component _renderer;
    };
}