# CPU Software Renderer

一个基于 C++ 和 SDL2 的小型 CPU 渲染器练习项目。当前版本聚焦在最基础的 2D 光栅化能力上，通过 CPU 维护帧缓冲区，再将像素数据上传到 SDL 纹理并显示到窗口中。

项目目标不是调用现成图形 API 去画三角形，而是从像素写入、直线绘制、三角形光栅化开始，逐步补齐一个软件渲染器所需的核心 pipeline。

## 项目定位

- 使用 CPU 完成像素级绘制和光栅化
- 使用 SDL2 负责窗口创建、纹理更新和屏幕展示
- 以最小可运行工程为基础，逐步扩展到 3D 渲染流程

## 当前状态

当前代码已经具备这些基础能力：

- 基于 `frameBuffer` 的软件像素输出
- `SetPixel` 像素写入
- Bresenham 直线绘制
- 三角形线框绘制
- 三角形包围盒扫描填充
- SDL2 窗口创建、纹理更新与显示

当前 `main.cpp` 中已经可以直接绘制一个填充三角形，用来验证基础光栅化逻辑。

## 技术选型

- 语言标准：C++20
- 工程类型：Visual Studio C++ 项目（`.vcxproj`）
- 渲染方式：CPU Software Rasterization
- 窗口与展示层：SDL2
- 开发环境：Windows + MSVC

选择 SDL2 的原因很直接：它只负责跨平台窗口和像素展示，足够轻量，不会替代掉软件渲染器本身的核心逻辑。

## 依赖

项目当前依赖很少：

- SDL2

仓库内已经包含 SDL2 相关文件：

- 头文件目录：`libs/SDL2/include`
- x64 库目录：`libs/SDL2/lib/x64`
- 运行时动态库：`libs/SDL2/SDL2.dll`

## 构建环境

当前工程配置以 Windows 本地开发为主，推荐环境：

- Visual Studio 2022
- MSVC 工具链
- Windows 10/11 SDK

项目文件中启用了：

- `stdcpp20`
- `PlatformToolset=v145`

## 构建方式

1. 使用 Visual Studio 打开 `CPU-Software-Renderer.vcxproj`
2. 选择 `Debug | x64`
3. 直接构建并运行

说明：

- 当前工程的 SDL2 include/lib 路径写在 `.vcxproj` 里
- 现有配置里 `Debug | x64` 已配置 SDL2 头文件和链接目录
- 如果你移动了仓库位置，可能需要同步调整工程中的绝对路径

## 当前工程结构

```text
CPU-Software-Renderer/
├─ main.cpp                # 程序入口，SDL 初始化与显示循环
├─ Rasterization.h/.cpp    # 帧缓冲、画线、三角形绘制
├─ StructType.h            # 向量、三角形、颜色等基础结构
├─ Color.h/.cpp            # 颜色相关封装
├─ libs/SDL2/              # SDL2 头文件、库和运行时文件
└─ TODO.md                 # 后续功能路线图
```

## 已实现模块

### 1. Frame Buffer

渲染结果先写入 CPU 侧的 `std::vector<uint32_t>` 帧缓冲区，再通过 `SDL_UpdateTexture` 更新到纹理。

### 2. Rasterizer

当前已经实现：

- `DrawLine`
- `DrawWireTriangle`
- `DrawFillTriangle`

其中直线绘制使用 Bresenham 思路，填充三角形使用包围盒扫描配合点在三角形内测试。

### 3. 基础数学类型

当前工程已经定义了：

- `Vector2Int`
- `Vector2`
- `Vector3Int`
- `Vector3`
- `Triangle`
- `Color`

这些结构会作为后续扩展 MVP 变换、深度测试、法线和属性插值的基础。

## 后续计划

1. 完成稳定的三角形光栅化基础
2. 加入 MVP 变换，实现 3D 顶点到屏幕空间的投影
3. 加入 Z-buffer，解决遮挡关系
4. 实现重心坐标属性插值
5. 实现 Blinn-Phong 光照
6. 支持 OBJ 模型加载
7. 实现相机控制

如果后续继续扩展，比较值得加入的方向还有：

- Texture Mapping
- MSAA 抗锯齿
- 更完整的数学库
- Shader 结构拆分

## 这个项目最终会变成什么

按当前路线推进后，这个项目会逐步具备一个小型软件渲染器的基本结构：

```text
Renderer
├─ Rasterizer
├─ Shader
├─ Camera
├─ Math
├─ OBJLoader
└─ ZBuffer
```

最终目标是实现一个不依赖 GPU 光栅化管线、但具备基本 3D 渲染能力的 CPU Renderer。

## 适合用来做什么

这个项目比较适合：

- 学习软件渲染器的基本实现方式
- 理解三角形光栅化、像素写入和屏幕空间坐标
- 作为后续学习图形学、渲染 pipeline、光照和模型加载的实验场
- 用于整理个人图形学项目经历

## 说明

当前项目仍在早期阶段，很多功能还处于逐步实现中。README 中的“后续计划”部分表示目标路线，不代表这些能力已经全部完成。
