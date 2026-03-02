#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region STD_LIBS
#include <functional>
#include <string>
#include <type_traits>
#pragma endregion

#pragma region FTXUI_LIB
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#pragma endregion

#pragma region MY_FILES
#include "ShapesParametersViews.hpp"
#include "ViewHelpers.hpp"
#pragma endregion

using namespace ftxui;

// Plane
tui::PlaneParametersView::PlaneParametersView(int& rows, int& columns, int& rangeIndex, int& planeDirIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack), 
    _rows(rows), _columns(columns), _planeDirIndex(planeDirIndex), _rowsInput(Input(&_rowsStr, "int")), _columnsInput(Input(&_columnsStr, "int"))
{
    _header = "PLANE";

    _planeDirRadio = Radiobox(&_planeDirOptions, &_planeDirIndex);

    _dynamicContentContainer = Container::Vertical({
        _rowsInput,
        _columnsInput,
        _planeDirRadio,
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            hbox(ColorValueLabel("Number of rows ", "(min: 2)", _rowsValid), _rowsInput->Render()) | focus,
            hbox(ColorValueLabel("Number of columns ", "(min: 2)", _columnsValid), _columnsInput->Render()),
            separator(),
            RadioWithLabel("Plane Direction", _planeDirRadio),
            separatorEmpty(),
            RadioWithLabel("Value Range", _rangeRadio)
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] {
        ValidateInt(_rowsStr, 2, _rows, _rowsValid, Check::GREATER_EQUAL);
        ValidateInt(_columnsStr, 2, _columns, _columnsValid, Check::GREATER_EQUAL);
        return RenderBase();
    });
}

bool tui::PlaneParametersView::AreAllInputsValid() const
{
    return _rowsValid && _columnsValid;
}

Element tui::PlaneParametersView::AsciiArt() const
{
    return vbox({
        text("   +--------+"),
        text("  /        / "),
        text(" /        /  "),
        text("+--------+   "),
    }) | hcenter;
}

// Cube
tui::CubeParametersView::CubeParametersView(int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack)
{
    _header = "CUBE";

    _dynamicContentContainer = Container::Vertical({
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            RadioWithLabel("Value Range", _rangeRadio) | focus
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] { return RenderBase(); });
}

Element tui::CubeParametersView::AsciiArt() const
{
    return vbox({
        text("  +------+"),
        text(" /      /|"),
        text("+------+ |"),
        text("|      | +"),
        text("|      |/ "),
        text("+------+  "),
    }) | hcenter;
}

// Tetrahedron
tui::TetrahedronParametersView::TetrahedronParametersView(int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack)
{
    _header = "TETRAHEDRON";

    _dynamicContentContainer = Container::Vertical({
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            RadioWithLabel("Value Range", _rangeRadio) | focus
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] { return RenderBase(); });
}

Element tui::TetrahedronParametersView::AsciiArt() const
{
    return vbox({
        text("   /\\   "),
        text("  /  \\  "),
        text(" /    \\ "),
        text("/______\\"),
    }) | hcenter;
}

// Pyramid
tui::PyramidParametersView::PyramidParametersView(int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack)
{
    _header = "PYRAMID";

    _dynamicContentContainer = Container::Vertical({
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            RadioWithLabel("Value Range", _rangeRadio) | focus
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] { return RenderBase(); });
}

Element tui::PyramidParametersView::AsciiArt() const
{
    return vbox({
        text("    /\\    "),
        text("   /\\ \\   "),
        text("  /  \\ \\ "),
        text(" /    \\ \\"),
        text("/______\\/  "),
    }) | hcenter;
}

// Cylinder
tui::CylinderParametersView::CylinderParametersView(int& hor, int& vert, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack),
    _hor(hor), _vert(vert), _shadingIndex(shadingIndex), _horInput(Input(&_horStr, "int")), _vertInput(Input(&_vertStr, "int"))
{
    _header = "CYLINDER";

    _shadingRadio = Radiobox(&_shadingOptions, &_shadingIndex);

    _dynamicContentContainer = Container::Vertical({
        _horInput,
        _vertInput,
        _shadingRadio,
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            hbox(ColorValueLabel("Horizontal Segments ", "(min: 1)", _horValid), _horInput->Render()) | focus,
            hbox(ColorValueLabel("Vertical Segments ", "(min: 3)", _vertValid), _vertInput->Render()),
            separator(),
            RadioWithLabel("Shading", _shadingRadio),
            separatorEmpty(),
            RadioWithLabel("Value Range", _rangeRadio)
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] {
        ValidateInt(_horStr, 1, _hor, _horValid, Check::GREATER_EQUAL);
        ValidateInt(_vertStr, 3, _vert, _vertValid, Check::GREATER_EQUAL);
        return RenderBase();
    });
}

bool tui::CylinderParametersView::AreAllInputsValid() const
{
    return _horValid && _vertValid;
}

Element tui::CylinderParametersView::AsciiArt() const
{
    return vbox({
        text(".──────."),
        text("|‾‾‾‾‾‾|"),
        text("|      |"),
        text("|______|"),
        text("'──────'"),
    }) | hcenter;
}

// Cone
tui::ConeParametersView::ConeParametersView(int& segments, float& height, float& radius, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack),
    _segments(segments), _height(height), _radius(radius), _shadingIndex(shadingIndex), _segmentsInput(Input(&_segmentsStr, "int")), _heightInput(Input(&_heightStr, "float")), _radiusInput(Input(&_radiusStr, "float"))
{
    _header = "CONE";

    _shadingRadio = Radiobox(&_shadingOptions, &_shadingIndex);

    _dynamicContentContainer = Container::Vertical({
        _segmentsInput,
        _heightInput,
        _radiusInput,
        _shadingRadio,
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            hbox(ColorValueLabel("Segments ", "(min: 3)", _segmentsValid), _segmentsInput->Render()) | focus,
            hbox(ColorValueLabel("Height ", "(greater than: 0.0)", _heightValid), _heightInput->Render()),
            hbox(ColorValueLabel("Radius ", "(greater than: 0.0)", _radiusValid), _radiusInput->Render()),
            separator(),
            RadioWithLabel("Shading", _shadingRadio),
            separatorEmpty(),
            RadioWithLabel("Value Range", _rangeRadio)
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] {
        ValidateInt(_segmentsStr, 3, _segments, _segmentsValid, Check::GREATER_EQUAL);
        ValidateFloat(_heightStr, 0.0f, _height, _heightValid, Check::GREATER);
        ValidateFloat(_radiusStr, 0.0f, _radius, _radiusValid, Check::GREATER);
        return RenderBase();
    });
}

bool tui::ConeParametersView::AreAllInputsValid() const
{
    return _segmentsValid && _heightValid && _radiusValid;
}

Element tui::ConeParametersView::AsciiArt() const
{
    return vbox({
        text("   /\\   "),
        text("  /  \\  "),
        text(" /    \\ "),
        text("(‾‾‾‾‾‾)"),
        text(" ‾‾‾‾‾‾ "),
    }) | hcenter;
}

// Sphere
tui::SphereParametersView::SphereParametersView(int& hor, int& vert, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack),
    _hor(hor), _vert(vert), _shadingIndex(shadingIndex), _horInput(Input(&_horStr, "int")), _vertInput(Input(&_vertStr, "int"))
{
    _header = "SPHERE";

    _shadingRadio = Radiobox(&_shadingOptions, &_shadingIndex);

    _dynamicContentContainer = Container::Vertical({
        _horInput,
        _vertInput,
        _shadingRadio,
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            hbox(ColorValueLabel("Horizontal Segments ", "(min: 2)", _horValid), _horInput->Render()) | focus,
            hbox(ColorValueLabel("Vertical Segments ", "(min: 3)", _vertValid), _vertInput->Render()),
            separator(),
            RadioWithLabel("Shading", _shadingRadio),
            separatorEmpty(),
            RadioWithLabel("Value Range", _rangeRadio)
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] {
        ValidateInt(_horStr, 2, _hor, _horValid);
        ValidateInt(_vertStr, 3, _vert, _vertValid);
        return RenderBase();
    });
}

bool tui::SphereParametersView::AreAllInputsValid() const
{
    return _horValid && _vertValid;
}

Element tui::SphereParametersView::AsciiArt() const
{
    return vbox({
        text(" .‾‾‾‾‾. "),
        text("/       \\"),
        text("\\       /"),
        text(" '_____' "),
    }) | hcenter;
}

// IcoSphere
tui::IcoSphereParametersView::IcoSphereParametersView(int& subdivisions, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack),
    _subs(subdivisions), _shadingIndex(shadingIndex), _subsInput(Input(&_subsStr, "int"))
{
    _header = "ICOSPHERE";

    _shadingRadio = Radiobox(&_shadingOptions, &_shadingIndex);

    _dynamicContentContainer = Container::Vertical({
        _subsInput,
        _shadingRadio,
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            hbox(ColorValueLabel("Subdivisions ", "(0 - " + std::to_string(_maxSubs) + ")", _subsValid), _subsInput->Render()) | focus,
            separator(),
            RadioWithLabel("Shading", _shadingRadio),
            separatorEmpty(),
            RadioWithLabel("Value Range", _rangeRadio)
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] {
        ValidateIntInRange(_subsStr, 0, _maxSubs, _subs, _subsValid);
        return RenderBase();
    });
}

bool tui::IcoSphereParametersView::AreAllInputsValid() const
{
    return _subsValid;
}

Element tui::IcoSphereParametersView::AsciiArt() const
{
    return vbox({
        text(" .‾‾‾/\\‾‾‾. "),
        text("/ \\ /__\\ / \\"),
        text("\\ / \\  / \\ /"),
        text(" '___\\/___' "),
    }) | hcenter;
}

// Torus
tui::TorusParametersView::TorusParametersView(int& maSegments, int& miSegments, float& maCircleRadius, float& miCircleRadius, int& rangeIndex, int& shadingIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack),
    _maSegments(maSegments), _miSegments(miSegments), _maCircleRadius(maCircleRadius), _miCircleRadius(miCircleRadius), _shadingIndex(shadingIndex),
    _maSegmentsInput(Input(&_maSegmentsStr, "int")), _miSegmentsInput(Input(&_miSegmentsStr, "int")), _maCircleRadiusInput(Input(&_maCircleRadiusStr, "float")), _miCircleRadiusInput(Input(&_miCircleRadiusStr, "float"))
{
    _header = "TORUS";

    _shadingRadio = Radiobox(&_shadingOptions, &_shadingIndex);

    _dynamicContentContainer = Container::Vertical({
        _maSegmentsInput,
        _miSegmentsInput,
        _maCircleRadiusInput,
        _miCircleRadiusInput,
        _shadingRadio,
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            hbox(ColorValueLabel("Major segments ", "(min: 3)", _maSegmentsValid), _maSegmentsInput->Render()) | focus,
            hbox(ColorValueLabel("Minor (Tube) segments ", "(min: 3)", _miSegmentsValid), _miSegmentsInput->Render()),
            hbox(ColorValueLabel("Major radius ", "(greater than: 0.0)", _maCircleRadiusValid), _maCircleRadiusInput->Render()),
            hbox(ColorValueLabel("Minor (Tube) radius ", "(greater than: 0.0)", _miCircleRadiusValid), _miCircleRadiusInput->Render()),
            separator(),
            RadioWithLabel("Shading", _shadingRadio),
            separatorEmpty(),
            RadioWithLabel("Value Range", _rangeRadio)
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] {
        ValidateInt(_maSegmentsStr, 3, _maSegments, _maSegmentsValid, Check::GREATER_EQUAL);
        ValidateInt(_miSegmentsStr, 3, _miSegments, _miSegmentsValid, Check::GREATER_EQUAL);
        ValidateFloat(_maCircleRadiusStr, 0.0f, _maCircleRadius, _maCircleRadiusValid, Check::GREATER);
        ValidateFloat(_miCircleRadiusStr, 0.0f, _miCircleRadius, _miCircleRadiusValid, Check::GREATER);
        return RenderBase();
    });
}

bool tui::TorusParametersView::AreAllInputsValid() const
{
    return _maSegmentsValid && _miSegmentsValid && _maCircleRadiusValid && _miCircleRadiusValid;
}

Element tui::TorusParametersView::AsciiArt() const
{
    return vbox({
        text(" .‾‾‾‾‾‾. "),
        text("/  .‾‾.  \\"),
        text("\\  '__'  /"),
        text(" '______' "),
    }) | hcenter;
}

// Hexagon
tui::HexagonParametersView::HexagonParametersView(int& hor, int& rangeIndex, std::function<void()> onGenerate, std::function<void()> onBack)
    : ParametersView(rangeIndex, onGenerate, onBack), _hor(hor), _horInput(Input(&_horStr, "int"))
{
    _header = "HEXAGON";

    _dynamicContentContainer = Container::Vertical({
        _horInput,
        _rangeRadio
    });

    _dynamicContent = Renderer(_dynamicContentContainer, [this] {
        return vbox({
            hbox(ColorValueLabel("Horizontal Segments ", "(min: 1)", _horValid), _horInput->Render()) | focus,
            separator(),
            RadioWithLabel("Value Range", _rangeRadio)
        });
    });

    _dynamicContentNotEmpty = true;

    _container = Container::Vertical({
        _backButton,
        _dynamicContent,
        _generateButton
    });

    _renderer = Renderer(_container, [this] {
        ValidateInt(_horStr, 1, _hor, _horValid);
        return RenderBase();
    });
}

bool tui::HexagonParametersView::AreAllInputsValid() const
{
    return _horValid;
}

Element tui::HexagonParametersView::AsciiArt() const
{
    return vbox({
        text(" /‾‾\\ "),
        text("|\\__/|"),
        text("|    |"),
        text("|/‾‾\\|"),
        text(" \\__/ ")
    }) | hcenter;
}