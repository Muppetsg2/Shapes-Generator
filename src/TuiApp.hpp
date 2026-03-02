#pragma once

#pragma region STB_LIBS
#include <chrono>
#include <memory>
#include <ratio>
#include <string>
#include <vector>
#pragma endregion

#pragma region FXTUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#pragma endregion

#pragma region MY_FILES_CORE_LIB
#include <Shape.hpp>
#pragma endregion

#pragma region MY_FILES_TUI_LIB
#include <GenerateView.hpp>
#include <SaveView.hpp>
#pragma endregion

#pragma region MY_FILES_UTILS_LIB
#include <Config.hpp>
#pragma endregion

namespace tui
{
    class App {
    public:
        App();
        void Run();

    private:
        int RegisterView(std::unique_ptr<View> view);
        void CreateViews();

        void GoToShapeSelect();
        void GoToGenerate(std::string shapeName, GenerateView::GenerateTask task);
        void GoToSave(std::string fileType, SaveView::SaveTask task);

        void DeleteShape();

        void CloseApp();

        void BuildUI();

    private:
        enum class AppViewType : int {
            ShapeSelect = 0,
            PlaneParams,
            CubeParams,
            TetrahedronParams,
            PyramidParams,
            CylinderParams,
            ConeParams,
            SphereParams,
            IcoSphereParams,
            TorusParams,
            HexagonParams,
            Generate,
            SaveFormat,
            Save,
            SavedScreen,
            EditConfig
        };

        enum class FileSaveStatus : int {
            SUCCESS = 0,
            FILE_EXPLORER_ERROR,
            FOLDER_ERROR,
            OPEN_FILE_ERROR,
        };

        // Root
        ftxui::ScreenInteractive _screen;
        ftxui::Component _root;

        // State
        int _currentView = static_cast<int>(AppViewType::ShapeSelect);
        int _selectedShapeIndex = 0;

        // Data
        const std::vector<std::string> _shapes = {
            "Plane", "Cube", "Tetrahedron", "Pyramid",
            "Cylinder", "Cone", "Sphere", "IcoSphere",
            "Torus", "Hexagon"
        };

        // Values
        int _intA = 0;
        int _intB = 0;
        float _floatA = 0;
        float _floatB = 0;

        int _planeDirIndex = 0;
        int _rangeIndex = 0;
        int _shadingIndex = 0;

        int _saveStatus = static_cast<int>(FileSaveStatus::SUCCESS);

        Shape* _selectedShape = nullptr;
        std::string _shapeName = "Unknown";
        std::string _saveLocation = "";
        std::string _exeDir;
        std::chrono::duration<double, std::milli> _elapsed;
        ShapeConfig _sConfig{};
        utils::Config _config{};
        FormatType _saveFormat = FormatType::CPP_ARRAY_INDICES_STRUCT;

        // Views
        std::vector<std::unique_ptr<View>> _views;
        std::vector<ftxui::Component> _viewComponents;

        ftxui::Component _tabContainer;
    };
}