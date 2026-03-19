# CPU Software Renderer TODO

## 目标

把当前项目从“能显示线框立方体”的状态，逐步推进到“具备基本 3D 渲染管线能力的 CPU Software Renderer”。

当前已经具备的基础：

- SDL 窗口、纹理和显示循环
- `FrameBuffer`
- 像素写入
- 直线光栅化
- 三角形包围盒遍历和点内测试雏形
- `Model -> View -> Projection -> NDC -> Viewport`
- 简单的线框绘制与面可见性判断

下面的 TODO 按建议实现顺序展开。

---

## Step 1. 稳定 2D 光栅基础

目标：把当前 2D 线段 / 三角形基础打稳，为后续 3D 面渲染做准备。

### 1.1 线段光栅化收尾

- 确认 Bresenham 在所有斜率下都正确
- 验证水平线、垂直线、45 度线、反向输入点
- 统一线段接口的输入输出约定
- 为调试准备简单测试图形

完成标准：

- 任意两个屏幕点都能稳定画线
- 没有明显断裂、漏点、方向依赖问题

### 1.2 三角形 2D 填充稳定化

- 校验 `BoundingBox` 的边界是否正确
- 校验点在三角形内判定的顶点顺序依赖
- 明确像素采样点采用像素中心还是整数坐标
- 明确边界像素规则，避免共享边重复或漏画

完成标准：

- 任意顺序输入的三角形都能被正确填充
- 边界行为稳定，没有明显裂缝

### 1.3 TriangleRasterizer 接入主流程前的准备

- 让 `TriangleRasterizer` 支持直接接收屏幕空间三角形
- 明确光栅阶段输入结构
- 为之后接 `DepthBuffer` 预留每像素深度写入点

---

## Step 2. MVP 与视口变换

目标：把 3D 顶点稳定变换到屏幕空间。

### 2.1 Model / View / Projection 约定固定

- 统一矩阵乘法约定
- 明确当前使用列向量还是行向量
- 明确当前 view space 是右手系还是左手系
- 明确相机前方是 `-Z`
- 明确 `nearPlane` 和 `farPlane` 使用正距离

完成标准：

- `M * V * P` 的方向清晰
- 相机朝向、投影矩阵、裁剪空间定义互相一致

### 2.2 NDC 与 Viewport 约定固定

- 明确 NDC 使用 `[-1, 1]`
- 明确屏幕原点为左上角
- 明确 `x` 向右、`y` 向下
- 明确 viewport 使用 `width - 1` / `height - 1`

完成标准：

- `ProjectToScreen()` 或等价流程不再分散写逻辑
- `Camera::get_viewport_matrix()` 成为唯一屏幕映射入口

### 2.3 从 main 中抽离变换逻辑

- 把 `clip -> ndc -> viewport` 从 `main.cpp` 抽到专门模块
- 明确输入是 object vertex 还是 assembled vertex
- 为后续 mesh / triangle pipeline 准备统一入口

建议新增模块：

- `Renderer` 或 `Pipeline`
- `ProjectVertex`
- `AssembleTriangle`

---

## Step 3. 三角形装配与实心面渲染

目标：从“线框”进入“实心三角形渲染”。

### 3.1 三角形装配

- 从 mesh 顶点和索引装配三角形
- 不再手写 cube edge 作为主流程
- 统一输入为 triangle list

完成标准：

- 主流程以“三角形”为核心，而不是“边”

### 3.2 背面剔除

- 统一顶点绕序约定
- 明确在 view space 还是 screen space 做剔除
- 验证法线方向与绕序一致

完成标准：

- 可稳定剔除背面
- 不会因为模型顶点顺序混乱而随机闪面

### 3.3 实心立方体跑通

- 用三角形填充代替线框
- 先可不加光照，只显示纯色面
- 验证旋转时每个面都正确显示

完成标准：

- 可以正确绘制一个实心旋转立方体

---

## Step 4. 深度测试

目标：解决遮挡关系。

### 4.1 DepthBuffer 基础

- 初始化深度缓冲
- 明确默认值是最远深度
- 每帧清空深度缓冲
- 和 `FrameBuffer` 保持相同分辨率

### 4.2 深度写入与比较

- 明确深度值使用哪个空间
- 推荐先使用 NDC z 或映射后的深度值
- 明确比较方向
- 在通过测试时更新颜色和深度

完成标准：

- 前面的三角形能正确遮挡后面的三角形

### 4.3 深度约定补全

- 明确 `near -> 0 / far -> 1` 还是其他映射
- 明确 viewport 是否处理 z
- 明确是否保留 `w` 供后续透视校正使用

这是当前 TODO 中原本写得比较粗的部分，建议补成文档。

---

## Step 5. 重心坐标与属性插值

目标：从“只会画面”走到“会在片元阶段传递数据”。

### 5.1 重心坐标

- 在三角形内部计算每个像素的重心坐标
- 输出 `alpha / beta / gamma`
- 先用于深度插值

### 5.2 线性属性插值

- 插值颜色
- 插值深度
- 插值法线
- 插值世界坐标
- 插值 UV

### 5.3 透视校正插值

这一项建议显式加入 TODO，原文没有写清楚。

需要补上的内容：

- 保存每个顶点的 `1 / w`
- 对属性做 `attr / w` 插值
- 最后再除以插值后的 `1 / w`

完成标准：

- 纹理坐标和法线在透视投影下不发生明显拉伸错误

---

## Step 6. 裁剪

目标：正确处理部分进入视锥的几何体。

这一项当前代码和原 TODO 都不够明确，但它是完整渲染器必须补的。

### 6.1 近平面裁剪

- 不要简单按顶点是否在 NDC 内直接丢弃
- 支持一个三角形部分穿过近平面
- 处理裁剪后生成 1 个或 2 个新三角形

### 6.2 视锥裁剪

- 支持左右上下近平远平面
- 最开始至少要处理 near plane
- 后续再扩展到完整 frustum clipping

完成标准：

- 靠近相机的大三角形不会突然破碎或整块消失

---

## Step 7. Shader 数据流

目标：让渲染流程从“硬编码逻辑”过渡到“清晰的着色阶段”。

这一项 README 提过方向，但 TODO 里不够明确，建议单独成阶段。

### 7.1 顶点输出结构

- 定义 vertex shader 输出
- 至少包含：
  - clip position
  - world position
  - normal
  - color / uv

### 7.2 片元输入结构

- 定义 rasterizer 传给 fragment 的输入
- 区分线性插值字段与透视校正字段

### 7.3 Shader 接口整理

- 不要把光照直接写死在 `main`
- 把着色逻辑集中到 `Shading/`
- 明确 shader 输入输出边界

完成标准：

- 后续增加 Lambert / Blinn-Phong / 纹理采样时不需要重写主流程

---

## Step 8. Blinn-Phong 光照

目标：实现基本光照模型。

### 8.1 光照输入准备

- 法线插值
- 世界空间或观察空间位置
- 相机方向
- 光源方向

### 8.2 Blinn-Phong 模型

- ambient
- diffuse
- specular
- half vector `H = normalize(L + V)`

### 8.3 法线空间与归一化

- 明确法线在 world 还是 view space 计算
- 每像素重新归一化法线

完成标准：

- 模型出现稳定高光和明暗面变化

---

## Step 9. OBJ 加载与通用模型渲染

目标：从内置立方体过渡到真实模型。

### 9.1 OBJ 基础加载

- 读取 `v`
- 读取 `vn`
- 读取 `vt`
- 读取 `f`

### 9.2 Mesh 数据整理

- 统一索引格式
- 处理 OBJ 中 position / uv / normal 分离索引
- 生成项目自己的 mesh 数据结构

### 9.3 模型接入渲染管线

- 用真实 mesh 走完整 triangle pipeline
- 不再依赖 demo 专用几何体

完成标准：

- 能渲染 teapot、bunny 或其他基础模型

---

## Step 10. 相机控制

目标：让 renderer 具备交互能力。

### 10.1 平移控制

- `WASD`
- 上下移动

### 10.2 旋转控制

- 鼠标控制 yaw / pitch
- 限制 pitch 防止翻转

### 10.3 运行时参数调整

- FOV
- near / far plane
- 光照参数

完成标准：

- 能在场景中自由观察模型

---

## Step 11. 结构收口

目标：让代码不再长期堆在 `main.cpp`。

### 11.1 Renderer 主流程落地

- 把渲染调度逻辑移入 `Renderer`
- `main.cpp` 只保留：
  - 初始化
  - 场景创建
  - 输入处理
  - 每帧调用 renderer

### 11.2 数据结构分层

- `Scene`: 相机、模型、变换
- `RenderData`: 光栅输入输出数据
- `Rasterizer`: 三角形覆盖与插值
- `Shading`: 顶点 / 片元着色
- `Core`: framebuffer / depthbuffer / renderer

### 11.3 调试与测试工具

- 增加简单数学单元测试
- 增加投影和视口断言
- 增加线框 / 深度 / 法线可视化模式

---

## 可选增强

这些不影响基础 renderer 完成，但能显著提升项目质量。

### A. Texture Mapping

- 纹理采样
- UV 插值
- 可先做 nearest，再做 bilinear

### B. MSAA

- 子像素采样
- Resolve 到最终 framebuffer

### C. 阴影或简单后处理

- Shadow map
- Gamma 校正
- Tone mapping

### D. 性能优化

- 降低无效像素遍历
- 更高效的 depth / color 写入
- SIMD 或分块光栅化

---

## 里程碑

### Milestone 1

- 线框立方体稳定显示
- MVP 与 viewport 正确

### Milestone 2

- 实心立方体
- 背面剔除
- Z-buffer

### Milestone 3

- 属性插值
- Blinn-Phong
- OBJ 模型

### Milestone 4

- 相机交互
- 结构整理
- 可选增强项

---

## 当前最建议优先做的 5 件事

1. 把主流程从线框切到实心三角形
2. 接入 `DepthBuffer`
3. 实现重心坐标和深度插值
4. 明确并实现透视校正插值
5. 把 `main.cpp` 的渲染逻辑收口到 `Renderer`
