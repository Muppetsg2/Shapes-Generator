#pragma once

#pragma region STD_LIBS
#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <string>
#include <ratio>
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
    class SaveView : public View {
    public:
        using SaveDuration = std::chrono::duration<double, std::milli>;
        using SaveResult = std::pair<std::string, SaveDuration>;
        using SaveTask = std::function<SaveResult()>;

        SaveView(Shape*& shapeToSave, SaveDuration& outTime, std::string& saveLocation, std::function<void()> onFinish);

        void StartTask(std::string fileType, SaveTask task);

        ftxui::Component GetComponent() override;

        bool OnEvent(ftxui::Event event) override;

    private:
        void StartWorker();

    private:
        SaveTask _task;
        std::function<void()> _onFinish;

        Shape*& _shapeToSave;
        SaveDuration& _outTime;
        std::string& _saveLocation;
        std::future<SaveResult> _future;

        std::string _fileTypeStr = "Text";

        size_t _frameIndex = 0;
        ftxui::Component _renderer;
    };
}