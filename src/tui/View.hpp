#pragma once

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#pragma endregion

namespace tui {
    class View {
    public:
        virtual ~View() = default;

        virtual ftxui::Component GetComponent() = 0;

        virtual bool OnEvent(ftxui::Event event) { return false; }
    };
}