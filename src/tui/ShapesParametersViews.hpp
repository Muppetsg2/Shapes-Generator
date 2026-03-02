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
#pragma endregion

#pragma region MY_FILES
#include "ParametersView.hpp"
#pragma endregion

namespace tui
{
    // Plane
    class PlaneParametersView : public ParametersView {
    public:
        PlaneParametersView(int& rows, int& columns, int& rangeIndex, int& planeDirIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        bool AreAllInputsValid() const override;

        ftxui::Element AsciiArt() const override;

    private:
        int& _rows;
        int& _columns;
        int& _planeDirIndex;

        std::string _rowsStr = "2";
        std::string _columnsStr = "2";

        bool _rowsValid = true;
        bool _columnsValid = true;

        const std::vector<std::string> _planeDirOptions = { "Up", "Front" };

        ftxui::Component _rowsInput;
        ftxui::Component _columnsInput;
        ftxui::Component _planeDirRadio;
    };

    // Cube
    class CubeParametersView : public ParametersView {
    public:
        CubeParametersView(int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        ftxui::Element AsciiArt() const override;
    };

    // Tetrahedron
    class TetrahedronParametersView : public ParametersView {
    public:
        TetrahedronParametersView(int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        ftxui::Element AsciiArt() const override;
    };

    // Pyramid
    class PyramidParametersView : public ParametersView {
    public:
        PyramidParametersView(int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        ftxui::Element AsciiArt() const override;
    };

    // Cylinder
    class CylinderParametersView : public ParametersView {
    public:
        CylinderParametersView(int& hor, int& vert, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        bool AreAllInputsValid() const override;

        ftxui::Element AsciiArt() const override;

    private:
        int& _hor;
        int& _vert;
        int& _shadingIndex;

        std::string _horStr = "1";
        std::string _vertStr = "3";

        bool _horValid = true;
        bool _vertValid = true;

        ftxui::Component _horInput;
        ftxui::Component _vertInput;
        ftxui::Component _shadingRadio;
    };

    // Cone
    class ConeParametersView : public ParametersView {
    public:
        ConeParametersView(int& segments, float& height, float& radius, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        bool AreAllInputsValid() const override;

        ftxui::Element AsciiArt() const override;

    private:
        int& _segments;
        float& _height;
        float& _radius;
        int& _shadingIndex;

        std::string _segmentsStr = "3";
        std::string _heightStr = "1.0";
        std::string _radiusStr = "1.0";

        bool _segmentsValid = true;
        bool _heightValid = true;
        bool _radiusValid = true;

        ftxui::Component _segmentsInput;
        ftxui::Component _heightInput;
        ftxui::Component _radiusInput;
        ftxui::Component _shadingRadio;
    };

    // Sphere
    class SphereParametersView : public ParametersView {
    public:
        SphereParametersView(int& hor, int& vert, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        bool AreAllInputsValid() const override;

        ftxui::Element AsciiArt() const override;

    private:
        int& _hor;
        int& _vert;
        int& _shadingIndex;

        std::string _horStr = "2";
        std::string _vertStr = "3";

        bool _horValid = true;
        bool _vertValid = true;

        ftxui::Component _horInput;
        ftxui::Component _vertInput;
        ftxui::Component _shadingRadio;
    };

    // IcoSphere
    class IcoSphereParametersView : public ParametersView {
    public:
        IcoSphereParametersView(int& subdivisions, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        bool AreAllInputsValid() const override;

        ftxui::Element AsciiArt() const override;

    private:
        int& _subs;
        int& _shadingIndex;

        std::string _subsStr = "0";

        bool _subsValid = true;

        int _maxSubs = 10;

        ftxui::Component _subsInput;
        ftxui::Component _shadingRadio;
    };

    // Torus
    class TorusParametersView : public ParametersView {
    public:
        TorusParametersView(int& maSegments, int& miSegments, float& maCircleRadius, float& miCircleRadius, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        bool AreAllInputsValid() const override;

        ftxui::Element AsciiArt() const override;

    private:
        int& _maSegments;
        int& _miSegments;
        float& _maCircleRadius;
        float& _miCircleRadius;
        int& _shadingIndex;

        std::string _maSegmentsStr = "3";
        std::string _miSegmentsStr = "3";
        std::string _maCircleRadiusStr = "1.0";
        std::string _miCircleRadiusStr = "1.0";

        bool _maSegmentsValid = true;
        bool _miSegmentsValid = true;
        bool _maCircleRadiusValid = true;
        bool _miCircleRadiusValid = true;

        ftxui::Component _maSegmentsInput;
        ftxui::Component _miSegmentsInput;
        ftxui::Component _maCircleRadiusInput;
        ftxui::Component _miCircleRadiusInput;
        ftxui::Component _shadingRadio;
    };

    // Hexagon
    class HexagonParametersView : public ParametersView {
    public:
        HexagonParametersView(int& hor, int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack);

    protected:
        bool AreAllInputsValid() const override;

        ftxui::Element AsciiArt() const override;

    private:
        int& _hor;

        std::string _horStr = "1";

        bool _horValid = true;

        ftxui::Component _horInput;
    };
}