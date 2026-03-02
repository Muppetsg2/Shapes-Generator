#pragma once

#pragma region STD_LIBS
#include <functional>
#include <string>
#include <vector>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>
#pragma endregion

#pragma region MY_FILES
#include "View.hpp"
#pragma endregion

namespace tui {
    class ShapeSelectView : public View {
    public:
        ShapeSelectView(const std::vector<std::string>& shapes, int& selectedShapeIndex, std::function<void()> onConfirm, std::function<void()> onExit, std::function<void()> onEditConfig);

        ftxui::Component GetComponent() override;
        bool OnEvent(ftxui::Event event) override;

    private:
        static ftxui::Element RenderBanner();

    private:
        const std::vector<std::string>& _shapes;
        int& _selectedShapeIndex;
        std::function<void()> _onConfirm;
        std::function<void()> _onExit;
        std::function<void()> _onEditConfig;

        bool _mouseBtnPressed = false;

        ftxui::Box _menuBox;

        ftxui::Component _menu;
        ftxui::Component _renderer;
    };
}