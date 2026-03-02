#pragma once

#pragma region STD_LIBS
#include <functional>
#include <string>
#include <vector>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>
#pragma endregion

#pragma region MY_FILES
#include "View.hpp"
#pragma endregion

namespace tui {
    class ParametersView : public View {
    public:
        ParametersView(int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack);

        ftxui::Component GetComponent() override;

        bool OnEvent(ftxui::Event event) override;

    protected:
        ftxui::Element RenderBase();

        virtual bool AreAllInputsValid() const;
        virtual ftxui::Element AsciiArt() const;

    protected:
        std::function<void()> _onGenerate;
        std::function<void()> _onBack;

        ftxui::Component _dynamicContent;
        ftxui::Component _dynamicContentContainer;
        bool _dynamicContentNotEmpty = false;

        ftxui::Component _generateButton;
        ftxui::Component _backButton;
        ftxui::Component _rangeRadio;
        ftxui::Component _shadingRadio;

        ftxui::Component _container;
        ftxui::Component _renderer;

        std::string _header = "PARAMETERS";

        const std::vector<std::string> _shadingOptions = { "Flat", "Smooth" };

    private:
        int& _rangeIndex;

        ftxui::Component _generateButtonBase;

        const std::vector<std::string> _rangeOptions = { "[-0.5, 0.5]", "[-1.0, 1.0]" };
    };
}