## Step 1：画一个三角形（光栅化）

### 布雷森汉姆直线算法(bresenham's line algorithm)：
- 有两点 $V_0(x_0, y_0)$ 和 $V_1(x_1, y_1)$
- 定义决策变量 $p_k$ 并令 $p_0 = 2 \times dy - dx$，其中 $dy = y_1 - y_0$，$dx = x_1 - x_0$
- 当 $p_k < 0$ 时，$p_{k+1}=p_k+2\times dy$，取下方像素点（$y_{k+1}=y_k$）
- 当 $p_k \geq 0$ 时，$p_{k+1}=p_k+2\times dy-2\times dx$，取上方像素点（$y_{k+1}=y_k+1$）
```C++
void DrawLine(Vector2 v0, Vector2 v1, Color color) 
{
    int32_t dx = v1.x - v0.x;
    int32_t dy = v1.y - v0.y;
    int32_t p = 2 * dy - dx;
    int32_t y = v0.y;

    for (int x = v0.x; x <= v1.x; x++) 
    {
        frameBuffer[x][y] = color;
        if (p >= 0) 
        {
            p -= 2 * dx;
            y++;
        }
        p += 2 * dy;
    }
}
```

### 包围盒：
- 对于一个三角形 ABC
- 取出其每个顶点的坐标的最大值和最小值得到 $V_0(x_{min},y_{min})$ 和 $V_1(x_{max},y_{max})$
- 这两个点所包起来的矩形区域就是三角形的包围盒
```C++
std::vector<Vector2> Bound(Vector2 v1, Vector2 v2, Vector2 v3) 
{
    return {
        Vector2(std::min({v1.x, v2.x, v3.x}), std::min({v1.y, v2.y, v3.y})),
        Vector2(std::max({v1.x, v2.x, v3.x}), std::max({v1.y, v2.y, v3.y}))
    };
}
```

### 叉乘法判断点是否在三角形内：
- 三角形 ABC 有 3 条边向量 $\overrightarrow{AB}$、$\overrightarrow{BC}$、$\overrightarrow{CA}$
- 对于点 $P(x,y)$，与每个顶点结合可得到 3 条向量 $\overrightarrow{PA}$、$\overrightarrow{PB}$、$\overrightarrow{PC}$
- 对于每一组向量（例如 $\overrightarrow{AB}$ 与 $\overrightarrow{PA}$）做叉乘的结果的 $Z$ 轴分量如果正负性相同，则表示这个点在三角形里
- 计算 $Z$ 轴分量可以简单表述为 $AB_x \times PA_y - AB_y \times PA_y$
```C++
int32_t Cross(Vector2 v1, Vector2 v2, Vector2 p) 
{
    int64_t x1 = p2.x - p1.x;
    int64_t y1 = p2.y - p1.y;
    int64_t x2 = p3.x - p1.x;
    int64_t y2 = p3.y - p1.y;
    return x1 * y2 - y1 * x2;
}

bool IsInTriangle(std::vector<Vector2> triangle, Vector2 point) 
{
    int64_t c0 = cross(triangle[0], triangle[1], point);
    int64_t c1 = cross(triangle[1], triangle[2], point);
    int64_t c2 = cross(triangle[2], triangle[0], point);

    bool hasNeg = (c0 < 0) || (c1 < 0) || (c2 < 0);
    bool hasPos = (c0 > 0) || (c1 > 0) || (c2 > 0);

    return !(hasNeg && hasPos);
}
```

---

## Step 2：MVP 变换（3D → 2D）

### Model 变换
Model 变换用于将模型从其自身的坐标空间变换到世界坐标空间，一般来说这个过程是被省略的，因为一般在定义模型时都会再定义一个位置和旋转，这里的定义就已经完成了 Model 变换的操作。

### View 变换
View 变换用于将世界空间里的各个顶点坐标变换到相机坐标系，也可以说是我们规定了一个摄像机需要处于的位置（一般右手系定义目标位置是 $(0,0,0)$，目标朝向是 $(0,0,-1)$，也就是在世界原点看向 $-z$ 方向），但目前摄像机没在那个位置，所以我们就需要对摄像机做 View 变换，把它变换到那个属于它的位置，同时又要保持所有的物体相对不变。具体到变换矩阵上就是先平移回目标点，再旋转到目标方向。（**特别注意，右手系与左手系 View 矩阵结果不一致，这里是右手系下的结果**）

$$M_{View}=M_{rotation}^{-1} \times M_{translation}^{-1}=\begin{bmatrix} right.x & right.y & right.z & -dot(right, position) \\ up.x & up.y & up.z & -dot(up, position) \\ back.x & back.y & back.z & -dot(back, position) \\ 0 & 0 & 0 & 1\end{bmatrix}$$

其中：
- $M_{translation}^{-1}$ 是由目标位置平移到当前摄像机位置的平移变换矩阵
- $M_{rotation}^{-1}$ 是由目标方向旋转到当前摄像机朝向的旋转变换矩阵

### Projection 变换
Projection 变换用于将摄像头视锥里的各个模型投影到一个平面上，这里分为**正交投影**与**透视投影**，透视投影会产生近小远大的效果，而正交投影不会。

核心矩阵：

```text
Model
View
Projection
```

流程：

```text
model space
↓
world space
↓
view space
↓
clip space
↓
screen space
```

公式：

```text
screen_pos = MVP * vertex
```

这一步完成后：

你可以画一个 **旋转的立方体**。

---

## Step 3：深度测试（Z-buffer）

解决遮挡问题。

没有深度测试会出现：

```text
后面的三角形覆盖前面的
```

实现：

```text
depth_buffer[x][y]
```

核心逻辑：

```text
if z < depth_buffer
    更新像素
```

这一步完成后：

**遮挡关系正确。**

---

## Step 4：重心插值（属性插值）

你已经有：

```text
α β γ
```

现在可以插值：

```text
颜色
深度
法线
纹理坐标
```

例如：

```text
color = α*c1 + β*c2 + γ*c3
```

这是 **GPU 的核心思想之一**。

---

## Step 5：Blinn-Phong 光照

实现经典光照模型：

公式：

```text
I = kaIa + kd(L·N) + ks(H·N)^n
```

组成：

```text
环境光 ambient
漫反射 diffuse
高光 specular
```

Blinn-Phong 用：

```text
H = normalize(L + V)
```

优点：

* 比 Phong 更高效

完成后：

你的模型会 **有光照效果**。

---

## Step 6：加载 OBJ 模型

支持真实模型。

实现：

```text
读取 obj 文件
解析：
v  顶点
vn 法线
f  面
```

然后：

```text
三角形 → renderer
```

推荐模型：

```text
teapot
bunny
```

这一步完成后：

你的 renderer 已经像一个 **小型渲染器**。

---

## Step 7：相机控制

实现一个简单 camera。

例如：

```text
鼠标旋转
WASD移动
```

核心：

```text
view matrix
```

这一步完成后：

renderer 就有 **交互能力**。

---

## 完成后的项目结构

最终 renderer 大概有这些模块：

```text
Renderer
 ├─ Rasterizer
 ├─ Shader
 ├─ Camera
 ├─ Math (Vec/Matrix)
 ├─ OBJLoader
 └─ ZBuffer
```

核心代码量：

```text
1500 - 3000 行
```

就已经很不错。

---

## 简历怎么写（重点）

千万不要写：

```text
实现了 Blinn-Phong 光照
```

要写成：

```text
实现一个 CPU Software Renderer，
支持 MVP 变换、三角形光栅化、Z-buffer 深度测试，
并实现基于 Blinn-Phong 的光照模型。
```

如果再加一句：

```text
支持 OBJ 模型加载和相机交互
```

这个项目 **含金量就很高了**。

---

## 一个非常重要的小技巧

很多人做 renderer 只做到：

```text
作业级
```

但如果你再加 **一个小扩展**：

例如：

```text
MSAA 抗锯齿
```

或者：

```text
Texture Mapping
```

项目立刻就变成：

```text
进阶图形项目
```

面试官印象会非常好。

---

## 最后给你一个真实评价

如果你完成这个 renderer：

你的图形学水平大概是：

```text
超过 90% 游戏客户端校招生
```

因为大部分人只：

```text
看课程
```

而没有 **实现 pipeline**。

---

如果你愿意，我可以再给你一份 **“Renderer 项目 GitHub 结构模板”**（很多人照这个结构写，很容易做成一个漂亮的开源项目）。
