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

## Step 2：MVP 与视口变换

### Model 变换
Model 变换用于将模型从其自身的坐标空间变换到世界坐标空间，一般来说这个过程是被省略的，因为一般在定义模型时都会再定义一个位置和旋转，这里的定义就已经完成了 Model 变换的操作。

### View 变换
View 变换用于将世界空间里的各个顶点坐标变换到相机坐标系，也可以说是我们规定了一个摄像机需要处于的位置（一般右手系定义目标位置是 $(0,0,0)$，目标朝向是 $(0,0,-1)$，也就是在世界原点看向 $-z$ 方向），但目前摄像机没在那个位置，所以我们就需要对摄像机做 View 变换，把它变换到那个属于它的位置，同时又要保持所有的物体相对不变。具体到变换矩阵上就是先平移回目标点，再旋转到目标方向。（**特别注意，右手系与左手系 View 矩阵结果不一致，这里是右手系下的结果**）

$$M_{View}=M_{rotation}^{-1} \times M_{translation}^{-1}=\begin{bmatrix} right.x & right.y & right.z & -dot(right, position) \\ up.x & up.y & up.z & -dot(up, position) \\ back.x & back.y & back.z & -dot(back, position) \\ 0 & 0 & 0 & 1\end{bmatrix}$$

其中：
- $M_{translation}^{-1}$ 是由目标位置平移到当前摄像机位置的平移变换矩阵
- $M_{rotation}^{-1}$ 是由目标方向旋转到当前摄像机朝向的旋转变换矩阵

### Projection 变换
Projection 变换用于将摄像头视锥里的各个模型投影到一个平面上，这里分为**正交投影**与**透视投影**，透视投影会产生近小远大的效果，而正交投影不会。用视锥来解释的话，正交投影下视锥会退化为长方体，近处与远处的东西一样大；透视投影下的视锥就是一个四棱台。要投影的平面是中心在坐标原点，边长为 2 的正方形，也就是说范围在 $[-1,1]^2$ 内，知道这个后面的缩放就好做了。

我们可以定义视锥的各项参数：
- 近平面距离（离坐标原点 xy 平面的距离，为负值）：$near$（n）
- 远平面距离（离坐标原点 xy 平面的距离，为负值）：$far$（f）
- 近平面左边（离坐标原点 yz 平面的距离）：$left$（l）
- 近平面右边（离坐标原点 yz 平面的距离）：$right$（r）
- 近平面上边（离坐标原点 xz 平面的距离）：$top$（t）
- 近平面下边（离坐标原点 xz 平面的距离）：$bottom$（b）

1. 正交投影（Orthographic Projection）：直接做平移缩放成 $[-1,1]^2$ 就行了

$$M_{orth}=\begin{bmatrix} \frac{2}{r-l} & 0 & 0 & -\frac{r + l}{r - l} \\ 0 & \frac{2}{t - b} & 0 & -\frac{t + b}{t - b} \\ 0 & 0 & \frac{2}{n - f} & -\frac{n + f}{f - n} \\ 0 & 0 & 0 & 1\end{bmatrix}$$

2. 透视投影（Perspective Projection）：相较于正交投影会麻烦的多，因为需要处理 z 轴的变形来产生近大远小的效果。但是我们可以在正交投影矩阵的基础上再应用一个矩阵乘法就可以得到最终的透视投影矩阵，整个推导过程需要通过在视锥的 yz 平面做一个剖面，构造相似三角形取特殊值来解方程

$$M_{perp}=\begin{bmatrix} \frac{1}{aspectRatio \times \tan(fov / 2)} & 0 & 0 & 0 \\ 0 & \frac{1}{\tan(fov / 2)} & 0 & 0 \\ 0 & 0 & -\frac{f + n}{f - n} & -\frac{2 \times f \times n}{f - n} \\ 0 & 0 & -1 & 0\end{bmatrix}$$

其中：
- $aspectRatio$ 宽高比：$width / height$
- $fov$ 垂直可视角：$\tan(fov/2)=\frac{height/2}{-n}$

### Viewport 变换
经过 Projection 变换后我们的三维坐标就会被映射到二维的 $[-1, 1]^2$ 平面中，Viewport（视口）变换就是用来将这个边长为 2 的正方形变换到屏幕空间中，这个过程需要两步，一是将平面缩放到屏幕分辨率（比如 1920*1080），然后就是平移到正确的位置。

$$M_{Viewport}=\begin{bmatrix} \frac{width-1}{2} & 0 & 0 & \frac{width-1}{2} \\ 0 & -\frac{height-1}{2} & 0 & \frac{height-1}{2} \\ 0 & 0 & 1 & 0 \\ 0 & 0 & 0 & 1\end{bmatrix}$$

其中：
- $width$ 与 $height$ 分别表示目标分辨率的宽和高
- $-\frac{height}{2}$ 的负号：这是由像素坐标起点决定的，这里以屏幕左上方作为坐标起点

---

## Step 4. 深度测试

### 重心插值（属性插值）
重心插值是为了解决三角形内部各点属性的取值问题，我们认为三角形的各项属性是定义在顶点上的，那么三角形内部的点的属性就需要根据三个顶点计算得到，而重心坐标就是一个比较好的计算方法。

三角形的重心是指三角形中的一个点，将这个三角形分成三个面积相等的三角形，也就是 $$\frac{1}{3}S_{\triangle{ABC}}=S_{\triangle{PAB}}=S_{\triangle{PBC}}=S_{\triangle{PCA}}$$ 稍微变形可以得到 $$S_{\triangle{ABC}}=S_{\triangle{PAB}}+S_{\triangle{PBC}}+S_{\triangle{PCA}}$$ 现在这三个三角形占总面积的比例都是 $\frac{1}{3}$，也就是说 $(\lambda_1, \lambda_2, \lambda_3) = (\frac{1}{3}, \frac{1}{3}, \frac{1}{3})$，我们只要保证三个 $\lambda$ 之和为 1，就可以将重心坐标推广到所有的坐标上，只要计算重心坐标在那个点时各个三角形所占的面积比重，就可以得到这个点受三个顶点属性的影响程度。

```c++
/// <summary>
/// 给定屏幕像素坐标，输出该点的面积坐标
/// </summary>
/// <param name="pos">要计算的屏幕像素点</param>
/// <param name="w0">面积坐标的 x 分量（引用）</param>
/// <param name="w1">面积坐标的 y 分量（引用）</param>
/// <param name="w2">面积坐标的 z 分量（引用）</param>
/// <returns>是否计算成功</returns>
bool get_barycentric(const Math::Vector2& p, float& w0, float& w1, float& w2) const
{
	const float x0 = v0.position.x;
	const float y0 = v0.position.y;
	const float x1 = v1.position.x;
	const float y1 = v1.position.y;
	const float x2 = v2.position.x;
	const float y2 = v2.position.y;

	const float square2D = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
	if (std::abs(square2D) < 1e-6f)
	{
		return false;
	}

	w0 = ((y1 - y2) * (p.x - x2) + (x2 - x1) * (p.y - y2)) / square2D;
	w1 = ((y2 - y0) * (p.x - x2) + (x0 - x2) * (p.y - y2)) / square2D;
	w2 = 1.0f - w0 - w1;
	return true;
}
```

这里的计算是投影后面积坐标的计算，也就是在二维空间下计算，仍然能用于判断任一像素点是否在三角形内，以及做深度信息的插值，但在做其他信息的插值时还需要做额外的恢复处理

### 使用 depthBuffer
在光栅化写入 frameBuffer 时，我们需要先判断是否能够写入，也就是将该点的深度信息与 depthBuffer 中的信息进行比对，如果比 depthBuffer 里的数据更小（也可以是更大，主要看项目约定），就可以写入 frameBuffer 并更新 depthBuffer，每个点的深度信息都可以通过插值来得到。

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
