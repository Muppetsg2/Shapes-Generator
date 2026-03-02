#pragma region PCH
#include "pch.hpp"
#pragma endregion

#pragma region MY_FILES_TU_LIB
#include <ViewHelpers.hpp>
#pragma endregion

#pragma region MY_FILES
#include "TuiApp.hpp"
#pragma endregion

using namespace ftxui;

tui::App::App() : _screen(ScreenInteractive::Fullscreen())
{
    _config = utils::get_config();
    _exeDir = utils::get_executable_path();

    CreateViews();
    BuildUI();
}

void tui::App::Run()
{
    Loop loop(&_screen, _root);
    while (!loop.HasQuitted()) {
        // Request a new frame to be drawn.
        _screen.RequestAnimationFrame();

        // Execute events, and draw the next frame.
        loop.RunOnce();

        // Sleep for a short duration to control the frame rate (60 FPS).
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }
}

int tui::App::RegisterView(std::unique_ptr<View> view)
{
    _viewComponents.push_back(view->GetComponent());
    _views.push_back(std::move(view));
    return static_cast<int>(_views.size()) - 1;
}

void tui::App::CreateViews()
{
    RegisterView(std::make_unique<ShapeSelectView>(
        _shapes,
        _selectedShapeIndex,
        [this]() {
            _sConfig = {
                _config.genTangents,
                _config.calcBitangents,
                _config.tangentHandednessPositive
            };
            // ShapeSelect = 0 and PlaneParams = 1 so +1 maps to params View
            _currentView = static_cast<int>(AppViewType::PlaneParams) + _selectedShapeIndex;
        },
        [this]() { CloseApp(); },
        [this]() {
            _currentView = static_cast<int>(AppViewType::EditConfig);
        }
    ));

    RegisterView(std::make_unique<PlaneParametersView>(
        _intA, _intB, _rangeIndex, _planeDirIndex,
        [this]() {
            GoToGenerate("Plane", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Plane>(_sConfig, true, _intA, _intB, static_cast<PlaneNormalDir>(_planeDirIndex), static_cast<ValuesRange>(_rangeIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<CubeParametersView>(
        _rangeIndex,
        [this]() {
            GoToGenerate("Cube", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Cube>(_sConfig, true, static_cast<ValuesRange>(_rangeIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<TetrahedronParametersView>(
        _rangeIndex,
        [this]() {
            GoToGenerate("Tetrahedron", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Tetrahedron>(_sConfig, true, static_cast<ValuesRange>(_rangeIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<PyramidParametersView>(
        _rangeIndex,
        [this]() {
            GoToGenerate("Pyramid", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Pyramid>(_sConfig, true, static_cast<ValuesRange>(_rangeIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<CylinderParametersView>(
        _intA, _intB, _rangeIndex, _shadingIndex,
        [this]() {
            GoToGenerate("Cylinder", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Cylinder>(_sConfig, true, _intA, _intB, static_cast<ValuesRange>(_rangeIndex), static_cast<Shading>(_shadingIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<ConeParametersView>(
        _intA, _floatA, _floatB, _rangeIndex, _shadingIndex,
        [this]() {
            GoToGenerate("Cone", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Cone>(_sConfig, true, _intA, _floatA, _floatB, static_cast<ValuesRange>(_rangeIndex), static_cast<Shading>(_shadingIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<SphereParametersView>(
        _intA, _intB, _rangeIndex, _shadingIndex,
        [this]() {
            GoToGenerate("Sphere", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Sphere>(_sConfig, true, _intA, _intB, static_cast<ValuesRange>(_rangeIndex), static_cast<Shading>(_shadingIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<IcoSphereParametersView>(
        _intA, _rangeIndex, _shadingIndex,
        [this]() {
            GoToGenerate("IcoSphere", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<IcoSphere>(_sConfig, (_intA < 7), _intA, static_cast<ValuesRange>(_rangeIndex), static_cast<Shading>(_shadingIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<TorusParametersView>(
        _intA, _intB, _floatA, _floatB, _rangeIndex, _shadingIndex,
        [this]() {
            GoToGenerate("Torus", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Torus>(_sConfig, true, _intA, _intB, _floatA, _floatB, static_cast<ValuesRange>(_rangeIndex), static_cast<Shading>(_shadingIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<HexagonParametersView>(
        _intA, _rangeIndex,
        [this]() {
            GoToGenerate("Hexagon", [this]() -> GenerateView::GenerateResult {
                return GenerateShape<Hexagon>(_sConfig, true, _intA, static_cast<ValuesRange>(_rangeIndex));
            });
        },
        [this]() { GoToShapeSelect(); }
    ));

    RegisterView(std::make_unique<GenerateView>(
        _selectedShape, _elapsed,
        [this]() { _shapeName = _selectedShape ? _selectedShape->getObjectClassName() : "Unknown"; _currentView = static_cast<int>(AppViewType::SaveFormat); }
    ));

    RegisterView(std::make_unique<SaveFormatView>(
        _shapeName, _elapsed,
        [this](int selectedFormat) {
            _saveStatus = static_cast<int>(FileSaveStatus::SUCCESS);
            _saveFormat = static_cast<FormatType>(selectedFormat);
            GoToSave(selectedFormat < 8 ? "Text" : (selectedFormat < 10 ? "JSON" : "OBJ"), [this]() -> SaveView::SaveResult {
                SaveView::SaveDuration elapsed;
                if (!utils::check_directory(_config.saveDir.c_str())) {
                    if (!utils::create_directory(_config.saveDir.c_str())) {
                        _saveStatus = static_cast<int>(FileSaveStatus::FILE_EXPLORER_ERROR);
                        return { "" , elapsed };
                    }
                }

                bool isFullPath = std::filesystem::path(_config.saveDir).is_absolute();
                std::string filePath = (isFullPath ? "" : _exeDir + DIRSEP) + _config.saveDir + DIRSEP + get_resolved_file_name(_config, _selectedShape->getObjectClassName()) + Shape::getFormatFileExtension(_saveFormat);
                auto start = std::chrono::high_resolution_clock::now();

                std::ofstream file(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

                if (file.is_open()) {
                    std::string text = _selectedShape->toString(_saveFormat);
                    file.write(text.data(), text.size());
                    file.close();

                    auto end = std::chrono::high_resolution_clock::now();

                    elapsed = std::chrono::duration_cast<SaveView::SaveDuration>(end - start);

                    std::this_thread::sleep_for(std::chrono::milliseconds(30000 / 60));

                    std::string separator = std::string(1, DIRSEP);
                    utils::replace_all(filePath, separator + separator, separator);

                    return { filePath, elapsed };
                }

                _saveStatus = static_cast<int>(FileSaveStatus::OPEN_FILE_ERROR);
                return { "", elapsed };
            });
        },
        [this]() {
            GoToShapeSelect();
        }
    ));

    RegisterView(std::make_unique<SaveView>(
        _selectedShape, _elapsed, _saveLocation,
        [this]() {
            if (_config.openDirOnSave)
            {
                bool res = utils::open_in_file_explorer(_saveLocation.c_str());

                if (!res)
                {
                    _saveStatus = static_cast<int>(FileSaveStatus::FILE_EXPLORER_ERROR);
                }
            }

            _currentView = static_cast<int>(AppViewType::SavedScreen);
        }
    ));

    RegisterView(std::make_unique<SaveResultView>(
        _saveStatus, _exeDir, _saveLocation, _elapsed,
        [this]() { GoToShapeSelect(); },
        [this]() { CloseApp(); }
    ));

    RegisterView(std::make_unique<EditConfigView>(
        _config, _exeDir,
        [this]() { GoToShapeSelect(); }
    ));
}

void tui::App::GoToShapeSelect()
{
    DeleteShape();
    if (!_saveLocation.empty()) std::string().swap(_saveLocation);
    _currentView = static_cast<int>(AppViewType::ShapeSelect);
}

void tui::App::GoToGenerate(std::string shapeName, GenerateView::GenerateTask task)
{
    _currentView = static_cast<int>(AppViewType::Generate);
    auto* gen = dynamic_cast<GenerateView*>(_views[_currentView].get());
    gen->StartTask(shapeName, task);
}

void tui::App::GoToSave(std::string fileType, SaveView::SaveTask task)
{
    _currentView = static_cast<int>(AppViewType::Save);
    auto* gen = dynamic_cast<SaveView*>(_views[_currentView].get());
    gen->StartTask(fileType, task);
}

void tui::App::DeleteShape()
{
    if (_selectedShape != nullptr)
    {
        delete _selectedShape;
        _selectedShape = nullptr;
    }
}

void tui::App::CloseApp()
{
    _screen.Exit();
    DeleteShape();
}

void tui::App::BuildUI()
{
    _tabContainer = Container::Tab(_viewComponents, &_currentView);

    auto selector = Renderer(_tabContainer, [this] {
        return _tabContainer->Render();
    });

    _root = CatchEvent(selector, [this](Event event) {
        if (_currentView >= 0 && _currentView < (int)_views.size()) {
            _views[_currentView]->OnEvent(event);
        }

        return _tabContainer->OnEvent(event);
    });
}