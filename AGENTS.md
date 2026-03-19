# Repository Guidelines

## Project Structure & Module Organization
The repository is a single Visual Studio C++ project under `CPU-Software-Renderer/`. `main.cpp` is the current executable entry point and demo loop. Core renderer pieces live in `Core/` (`FrameBuffer`, `DepthBuffer`, `Renderer`), math types in `Math/`, rasterization logic in `Rasterizer/`, scene data in `Scene/`, asset loading in `Asset/`, and shader code in `Shading/`. SDL2 is vendored in `CPU-Software-Renderer/libs/SDL2/`. Planning notes live in `CPU-Software-Renderer/TODO.md`. Build outputs appear in `Debug/`, `Release/`, and `x64/`; do not commit those artifacts.

## Build, Test, and Development Commands
Use Visual Studio 2022 or MSBuild, not `dotnet build`, because the project depends on Visual C++ targets.

```powershell
MSBuild.exe CPU-Software-Renderer\CPU-Software-Renderer.vcxproj /p:Configuration=Debug /p:Platform=x64
```

Builds the debug x64 executable at `CPU-Software-Renderer/x64/Debug/CPU-Software-Renderer.exe`.

```powershell
MSBuild.exe CPU-Software-Renderer\CPU-Software-Renderer.vcxproj /p:Configuration=Release /p:Platform=x64
```

Builds the optimized release binary. In Visual Studio, open `CPU-Software-Renderer.slnx` or the `.vcxproj`, then run the selected `Debug|x64` or `Release|x64` target.

## Coding Style & Naming Conventions
Follow the existing C++20/MSVC style: tabs for indentation, braces on the next line for functions and control blocks, and include headers with quoted local paths such as `"FrameBuffer.h"`. Types and namespaces use `PascalCase` (`FrameBuffer`, `Math::Vector3`), methods use `snake_case` (`set_pixel`, `set_position`), and local variables use descriptive lowercase names. Keep modules narrow: math stays in `Math/`, render-domain structs in `RenderData/`, and behavior in `Core/` or `Rasterizer/`.

## Testing Guidelines
There is no automated test project yet. Validate changes by building `Debug|x64`, running the executable, and checking the rendered output or interaction path you changed. When adding nontrivial math or rasterization behavior, prefer small, isolated helper functions so a future test target can cover them easily.

## Commit & Pull Request Guidelines
Recent history uses short, imperative commit subjects, sometimes in English and sometimes in Chinese, for example `add TODO.md` and `完善 SDL 主循环并收紧三角形光栅化边界处理`. Keep the subject concise and focused on one change. Pull requests should describe the rendering behavior changed, list the build configuration used for verification, and include a screenshot or short note when the visual output changes.

Use UTF-8 encoding when reading and writing.