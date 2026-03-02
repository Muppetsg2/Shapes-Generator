#pragma once

#pragma region STD_LIBS
#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <ratio>
#include <string>
#include <utility>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#pragma endregion

#pragma region MY_FILES
#include "View.hpp"
#pragma endregion

class Shape;

namespace tui {
    class GenerateView : public View {
    public:
        using GenerateDuration = std::chrono::duration<double, std::milli>;
        using GenerateResult = std::pair<Shape*, GenerateDuration>;
        using GenerateTask = std::function<GenerateResult()>;

        GenerateView(Shape*& outShape, GenerateDuration& outTime, std::function<void()> onFinish);

        void StartTask(std::string shapeName, GenerateTask task);

        ftxui::Component GetComponent() override;

        bool OnEvent(ftxui::Event event) override;

    private:
        void StartWorker();

    private:
        GenerateTask _task;
        std::function<void()> _onFinish;

        Shape*& _outShape;
        GenerateDuration& _outTime;
        std::future<GenerateResult> _future;

        std::string _shapeStr = "shape";

        size_t _frameIndex = 0;
        ftxui::Component _renderer;
    };
}