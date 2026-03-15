# CPU Software Renderer

一个基于 C++20 和 SDL2 的 CPU 软光栅化练习项目。

这个仓库当前还处在早期阶段，重点不是调用现成图形 API 画三角形，而是自己维护帧缓冲、自己做基础光栅化，并逐步搭起一个小型 software renderer 的骨架。

## 当前状态

目前已经完成或具备雏形的部分：

- 使用 SDL2 创建窗口、纹理和显示循环
- 在 CPU 侧维护 `FrameBuffer`
- 像素写入接口 `set_pixel`
- 基础直线光栅化模块 `Rasterizer`
- 2D 三角形、包围盒、颜色等基础数据结构
- 独立的 `TriangleRasterizer` 模块，用于后续填充三角形

当前 `main.cpp` 主要用于验证基础链路，程序启动后会在窗口中绘制三条彩色线段组成的三角形轮廓。

还没有完成的核心能力：

- 完整接入实心三角形渲染流程
- MVP 变换
- 深度测试
- 重心坐标插值
- 光照
- OBJ 加载
- 可交互相机

## 当前结构

```text
CPU-Software-Renderer/
├─ CPU-Software-Renderer/
│  ├─ main.cpp
│  ├─ Core/
│  │  ├─ FrameBuffer.h/.cpp
│  │  ├─ DepthBuffer.h/.cpp
│  │  └─ Renderer.h/.cpp
│  ├─ Math/
│  │  ├─ Vector2.h
│  │  ├─ Vector3.h
│  │  ├─ Vector4.h
│  │  ├─ Matrix4x4.h
│  │  └─ MathUtil.h
│  ├─ RenderData/
│  │  ├─ Color.h
│  │  ├─ BoundingBox.h
│  │  └─ Triangle.h
│  ├─ Rasterizer/
│  │  ├─ Rasterizer.h/.cpp
│  │  └─ TriangleRasterizer.h/.cpp
│  ├─ Scene/
│  │  ├─ Vertex.h
│  │  ├─ Mesh.h
│  │  ├─ Transform.h
│  │  ├─ Model.h
│  │  └─ Camera.h
│  ├─ Asset/
│  │  └─ ObjLoader.h/.cpp
│  ├─ Shading/
│  │  ├─ ShaderTypes.h
│  │  └─ BlinnPhongShader.h/.cpp
│  ├─ libs/SDL2/
│  └─ TODO.md
└─ README.md
```

## 模块说明

### Core

- `FrameBuffer`：管理 CPU 侧颜色缓冲
- `DepthBuffer`：为后续深度测试预留
- `Renderer`：为后续统一调度渲染流程预留

### Math

- 放通用数学类型
- 当前以 `Vector2`、`Vector2Int`、`Vector3` 为主
- `Vector4` 和 `Matrix4x4` 目前还是占位

### RenderData

- 放渲染领域数据，而不是底层数学类型
- 当前包含 `Color`、`BoundingBox2D`、`Triangle2D`

### Rasterizer

- `Rasterizer`：负责基础线段光栅化
- `TriangleRasterizer`：负责三角形填充相关逻辑

### Scene / Asset / Shading

这些目录已经预留出来了，方便后面扩展到 3D 渲染流程，但当前多数还是占位类。

## 构建环境

- Windows
- Visual Studio 2022
- MSVC
- C++20
- SDL2

工程文件是 Visual Studio 的 `.vcxproj`。

## 依赖

仓库内已经包含 SDL2：

- 头文件：`CPU-Software-Renderer/libs/SDL2/include`
- Win32 库：`CPU-Software-Renderer/libs/SDL2/lib/x86`
- x64 库：`CPU-Software-Renderer/libs/SDL2/lib/x64`

## 构建与运行

1. 用 Visual Studio 打开 `CPU-Software-Renderer/CPU-Software-Renderer.vcxproj`
2. 选择 `Debug` 或 `Release`
3. 选择 `Win32` 或 `x64`
4. 构建并运行

说明：

- 工程里已经分别配置了 Win32 和 x64 的 SDL2 include/lib 路径
- 运行时需要让程序找到与平台匹配的 `SDL2.dll`
- 如果你移动了仓库位置，可能需要同步检查 `.vcxproj` 里的绝对路径

## 设计取向

当前代码结构遵循一个比较简单的原则：

- 纯数据类型放在 `Math` 和 `RenderData`
- 做事的模块放在 `Core`、`Rasterizer`、`Asset`、`Shading`
- 先把 2D 基础链路理顺，再逐步加 3D pipeline

这意味着现阶段更强调“结构和边界先理清”，而不是一次性把所有功能堆进去。

## 后续计划

按 `TODO.md` 的路线，后续会逐步补齐：

1. 稳定的三角形填充
2. MVP 变换
3. Z-buffer
4. 属性插值
5. Blinn-Phong 光照
6. OBJ 模型加载
7. 相机控制

如果后续继续扩展，还可以考虑：

- Texture Mapping
- MSAA
- 更完整的数学库
- 更清晰的 shader 输入输出结构

## 说明

这个项目现在更接近“正在搭骨架的 renderer”，不是一个已经完成的 3D 渲染器。

README 中提到的很多模块和目录，目的是反映当前的架构方向；其中一部分已经有初步实现，另一部分还只是为后续阶段预留。
