# Code Questions - Detailed Exam Answers

这份文档用于准备 Computer Graphics final project 的代码答辩。每个问题都按以下逻辑回答：

- 是什么：概念定义。
- 原理：它在图形学或 Vulkan 渲染流程中的作用。
- 项目实现：本项目中对应的代码位置和实现方式。
- 困难点与解决办法：实现时容易出错的点，以及本项目如何处理。
- 考试回答模板：可以直接口头复述的短答案。

主要参考文件：

- `source/src/main.cpp`
- `source/shaders/tavern_lit.vert`
- `source/shaders/tavern_lit.frag`
- `source/assets/scenes/scene.json`
- `0.Documentation/ExamReview.md`
- `0.Documentation/DevelopmentLog/2026-06-29-037-document-shader-pipeline.md`

## Vertex Format

### [x] Vertex Format 是什么？在 CG 中有什么作用？

**是什么**

Vertex Format，也就是顶点格式，定义了每个顶点在内存中包含哪些属性，以及这些属性如何被 GPU 读取。一个顶点不一定只有空间位置，它通常还会包含 normal、UV、color、tangent 等信息。

在本项目中，顶点格式是：

```cpp
struct Vertex {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 UV;
};
```

也就是每个顶点有：

- position
- normal
- texture coordinates

**原理**

GPU 渲染模型时，会从 vertex buffer 中逐个读取顶点。问题是，GPU 需要知道一段二进制数据里哪些字节代表 position，哪些字节代表 normal，哪些字节代表 UV。Vertex Format 就是这份说明书。

在 Vulkan 中，这部分由 vertex input binding 和 vertex input attribute 描述：

- binding 描述每个顶点整体占多少字节，以及是 per-vertex 还是 per-instance。
- attribute 描述 shader 中某个 `layout(location = X)` 输入从顶点结构的哪个 offset 读取。

**项目实现**

在 `source/src/main.cpp` 中，`VD.init(...)` 定义了 vertex descriptor：

```cpp
VD.init(this, {
      {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
    }, {
      {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
             sizeof(glm::vec3), POSITION},
      {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
             sizeof(glm::vec3), NORMAL},
      {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
             sizeof(glm::vec2), UV}
    });
```

它对应 vertex shader 中的输入：

```glsl
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
```

这说明：

- C++ 的 `pos` 对应 shader 的 `inPosition`。
- C++ 的 `norm` 对应 shader 的 `inNormal`。
- C++ 的 `UV` 对应 shader 的 `inUV`。

**困难点与解决办法**

困难点 1：C++ struct 的内存布局必须和 shader location 对上。

解决方法：使用 `offsetof(Vertex, pos)`、`offsetof(Vertex, norm)`、`offsetof(Vertex, UV)`，不要手写字节偏移。这样即使 struct 内存布局有 padding，也能得到正确 offset。

困难点 2：如果没有 normal，只能做无光照或假光照。

解决方法：项目从早期的 position + UV 格式升级成 position + normal + UV，使 fragment shader 可以基于真实 normal 计算 diffuse 和 specular lighting。

困难点 3：模型文件中的 vertex data 也必须匹配这个格式。

解决方法：`scene.json` 中所有当前模型都使用 `VDposNormUV`，并且 C++ 中通过：

```cpp
VDRs[0].init("VDposNormUV", &VD);
```

把这个名字和实际 vertex descriptor 绑定起来。

**考试回答模板**

Vertex Format 是 GPU 读取 vertex buffer 的说明书。它告诉 Vulkan 每个顶点有哪些属性、每个属性在内存中的位置，以及它们对应 shader 的哪个 input location。本项目使用 position、normal、UV 三个属性，足够完成模型定位、光照计算和纹理采样。

### [x] Vertex struct 里的 `pos`、`norm`、`UV` 都代表什么意思，用来干什么的？

**是什么**

`pos` 是顶点在模型局部坐标系中的位置。

`norm` 是顶点法线，表示表面朝向。

`UV` 是纹理坐标，用来从 2D texture image 中采样颜色。

**原理**

`pos` 会经过 Model、View、Projection 变换，最终变成屏幕上的位置。

`norm` 用于光照。比如 diffuse lighting 的核心是：

```glsl
max(dot(normal, lightDir), 0.0)
```

如果 normal 面向光源，点积较大，表面更亮。如果 normal 背向光源，点积接近 0，表面更暗。

`UV` 通常范围是 `[0, 1]`，表示纹理图片上的横纵坐标。fragment shader 使用：

```glsl
texture(albedoMap, fragUV)
```

读取纹理颜色。

**项目实现**

在 vertex shader 中：

```glsl
gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
fragPos = (ubo.mMat * vec4(inPosition, 1.0)).xyz;
fragNormal = normalize(mat3(transpose(inverse(ubo.mMat))) * inNormal);
fragUV = inUV;
```

这里：

- `inPosition` 用来计算 `gl_Position` 和 `fragPos`。
- `inNormal` 被转换成 world-space normal，传给 fragment shader。
- `inUV` 直接传给 fragment shader。

在 fragment shader 中：

```glsl
vec3 N = normalize(fragNormal);
vec3 albedo = pow(texture(albedoMap, fragUV).rgb, vec3(2.2));
```

这里：

- `fragNormal` 参与光照。
- `fragUV` 用来采样 texture。

**困难点与解决办法**

困难点 1：normal 不能像 position 一样直接用 model matrix 简单变换，尤其当模型有非均匀缩放时。

解决方法：vertex shader 使用 inverse-transpose normal matrix：

```glsl
mat3(transpose(inverse(ubo.mMat)))
```

困难点 2：UV 错误会导致纹理错位、拉伸或采样到错误区域。

解决方法：项目主要使用 course-provided `.MGCG` asset 和统一 texture atlas，模型自带正确 UV。自制 OBJ panel 只用于简单墙面/天花板辅助几何。

**考试回答模板**

`pos` 决定顶点在哪里，经过 MVP 矩阵后变成屏幕位置；`norm` 表示表面朝向，用于 diffuse 和 specular 光照；`UV` 是纹理坐标，用来从图片中取颜色。三者组合让模型既能出现在正确位置，也能有纹理和光照效果。

## Uniform Buffers

### [x] Uniform Buffers 是什么？在 CG 中有什么作用？

**是什么**

Uniform Buffer 是 CPU 传给 GPU shader 的一块只读数据缓冲区。它通常存放每帧或每个物体都会用到，但对 shader 中许多顶点或像素来说保持一致的数据。

例如：

- camera matrix
- model matrix
- light position
- light color
- material parameters

**原理**

shader 在 GPU 上执行，不能直接访问 C++ 普通变量。CPU 必须通过 Vulkan buffer、descriptor set 等机制把数据传给 shader。Uniform Buffer 就是其中一种常用方式。

Uniform 的特点是：

- 对一次 draw call 中的许多 shader invocation 来说是统一的。
- 适合放矩阵、光源、材质参数等小到中等大小的数据。
- 比把这些数据写进每个 vertex 更合理，因为矩阵和光源通常不属于单个顶点。

**项目实现**

本项目有两个 uniform buffer struct：

```cpp
struct UniformBufferObject {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
};
```

这个是 local uniform，按物体实例变化。

```cpp
struct GlobalUniformBufferObject {
    alignas(16) glm::vec4 eyePos;
    alignas(16) glm::vec4 ambientColor;
    alignas(16) glm::vec4 directionalLightDir;
    alignas(16) glm::vec4 directionalLightColor;
    alignas(16) glm::vec4 pointLightPositions[6];
    alignas(16) glm::vec4 pointLightColors[6];
    alignas(16) glm::vec4 materialParams;
    alignas(16) glm::ivec4 lightCounts;
};
```

这个是 global uniform，按 frame 更新，主要控制光照和 camera。

在 `updateUniformBuffer(...)` 中，CPU 每帧填充这些 struct，并通过：

```cpp
DSglobal.map(currentImage, &gubo, 0);
SC.TI[0].I[instanceId].DS[0][0]->map(currentImage, &gubo, 0);
SC.TI[0].I[instanceId].DS[0][1]->map(currentImage, &ubo, 0);
```

把数据传到 GPU。

**困难点与解决办法**

困难点 1：CPU 和 GPU 的内存布局必须一致。

解决方法：使用 `alignas(16)`，并且在 shader 中使用对应的 `vec4`、`mat4`，避免 alignment 不匹配。

困难点 2：哪些数据应该 local，哪些应该 global 容易混淆。

解决方法：本项目用两个 descriptor set 清晰分开：

- global：camera 和 lights，整帧共享。
- local：object transform 和 texture，每个实例不同。

**考试回答模板**

Uniform Buffer 是 CPU 向 shader 传递全局或每物体参数的缓冲区。本项目用 local uniform 传 MVP 和 model matrix，用 global uniform 传 camera position、ambient light、directional light、point lights 和材质参数。这样 shader 就能根据每帧状态计算正确的位置和光照。

### [x] `UniformBufferObject` 中的 `mvpMat` 和 `mMat` 都在干什么？

**是什么**

`mvpMat` 是 Model-View-Projection 矩阵的组合。

`mMat` 是 Model matrix，也就是模型矩阵。

**原理**

一个模型顶点最开始处于 object/local space。为了显示到屏幕上，需要经过多个坐标空间：

```text
Object Space -> World Space -> View/Camera Space -> Clip Space -> Screen
```

其中：

- Model matrix：Object Space -> World Space
- View matrix：World Space -> Camera Space
- Projection matrix：Camera Space -> Clip Space

组合起来就是：

```text
Projection * View * Model
```

在本项目中：

```cpp
ubo.mMat = SC.TI[0].I[instanceId].Wm;
ubo.mvpMat = ViewPrj * ubo.mMat;
```

而 `ViewPrj` 已经是：

```cpp
ViewPrj = Prj * View;
```

所以：

```text
mvpMat = Projection * View * Model
```

**项目实现**

vertex shader 使用 `mvpMat` 输出裁剪空间位置：

```glsl
gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
```

它使用 `mMat` 计算 world-space position：

```glsl
fragPos = (ubo.mMat * vec4(inPosition, 1.0)).xyz;
```

并用 `mMat` 计算 world-space normal：

```glsl
fragNormal = normalize(mat3(transpose(inverse(ubo.mMat))) * inNormal);
```

**为什么两个都要有**

如果只有 `mvpMat`，可以把模型画到屏幕上，但 fragment shader 不知道这个 fragment 在世界中的实际位置，也不方便计算点光源方向和距离。

如果只有 `mMat`，可以知道世界坐标，但不能直接完成最终投影。

所以：

- `mvpMat` 用于 rasterization 位置。
- `mMat` 用于 world-space lighting。

**困难点与解决办法**

困难点：早期很多项目只传 MVP，后来加 point light 时发现 shader 还需要 world position。

解决方法：保留 `mvpMat`，同时添加 `mMat`。vertex shader 输出 `fragPos` 给 fragment shader，使 fragment shader 能计算：

```glsl
gubo.pointLightPositions[index].xyz - fragPos
```

**考试回答模板**

`mvpMat` 负责把顶点从模型空间一路变换到 clip space，最终用于屏幕显示；`mMat` 只负责从模型空间到世界空间。本项目需要 `mMat` 是因为 fragment shader 要用 world position 和 world normal 计算点光源方向、距离和光照强度。

### [x] 游戏中的物体坐标是如何投影到屏幕坐标的？

**是什么**

物体坐标投影到屏幕，是指一个模型顶点从 3D 世界中的局部坐标，经过一系列矩阵变换，最后成为屏幕上的 2D 像素位置。

**原理**

完整流程是：

```text
Local/Object Space
    -> Model Matrix
World Space
    -> View Matrix
Camera/View Space
    -> Projection Matrix
Clip Space
    -> Perspective Divide
NDC
    -> Viewport Transform
Screen Pixels
```

其中：

- Model matrix 决定物体在世界中的位置、旋转、缩放。
- View matrix 表示相机的反向变换。
- Projection matrix 表示透视投影。
- Perspective divide 把 `x, y, z, w` 变成 `x/w, y/w, z/w`。
- Vulkan viewport 再把 NDC 映射到窗口像素。

**项目实现**

场景中每个 instance 的 transform 来自 `scene.json`：

```json
"translate": [0.0, 0.76, -9.0],
"eulerAngles": [0.0, 0.0, 0.0],
"scale": [1.35, 1.0, 1.05]
```

课程 `Scene` 框架把这些 transform 转换成 world matrix，也就是：

```cpp
SC.TI[0].I[instanceId].Wm
```

项目中每帧计算：

```cpp
ubo.mMat = SC.TI[0].I[instanceId].Wm;
ubo.mvpMat = ViewPrj * ubo.mMat;
```

vertex shader 中：

```glsl
gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
```

`gl_Position` 是 GPU 后续 rasterization 所需要的 clip-space 位置。

**困难点与解决办法**

困难点 1：矩阵乘法顺序不能乱。

解决方法：项目使用：

```text
Projection * View * Model * vertex
```

对应代码是：

```cpp
ViewPrj = Prj * View;
ubo.mvpMat = ViewPrj * ubo.mMat;
```

困难点 2：Vulkan 和 OpenGL 的 clip coordinate 在 Y 方向约定不同。

解决方法：本项目使用：

```cpp
Prj[1][1] *= -1;
```

把 GLM 默认投影矩阵调整到 Vulkan 使用的方向。

**考试回答模板**

物体顶点先通过 model matrix 从 local space 到 world space，再通过 view matrix 到 camera space，再通过 perspective projection 到 clip space。项目中 CPU 计算 `mvpMat = Projection * View * Model`，vertex shader 用它计算 `gl_Position`，之后 GPU 自动完成 perspective divide、viewport transform 和 rasterization。

### [x] `GlobalUniformBufferObject` 中的各个变量是如何被调用的？

**是什么**

`GlobalUniformBufferObject` 是项目中用于全局光照和相机数据的 uniform buffer。它的变量在 C++ 中被赋值，在 fragment shader 中被读取。

**项目实现**

C++ struct：

```cpp
struct GlobalUniformBufferObject {
    alignas(16) glm::vec4 eyePos;
    alignas(16) glm::vec4 ambientColor;
    alignas(16) glm::vec4 directionalLightDir;
    alignas(16) glm::vec4 directionalLightColor;
    alignas(16) glm::vec4 pointLightPositions[6];
    alignas(16) glm::vec4 pointLightColors[6];
    alignas(16) glm::vec4 materialParams;
    alignas(16) glm::ivec4 lightCounts;
};
```

shader uniform block：

```glsl
layout(binding = 0, set = 0) uniform GlobalUniformBufferObject {
    vec4 eyePos;
    vec4 ambientColor;
    vec4 directionalLightDir;
    vec4 directionalLightColor;
    vec4 pointLightPositions[6];
    vec4 pointLightColors[6];
    vec4 materialParams;
    ivec4 lightCounts;
} gubo;
```

变量用途：

- `eyePos`：相机世界位置，用于计算 view direction。
- `ambientColor`：环境光颜色。
- `directionalLightDir`：方向光方向。
- `directionalLightColor`：方向光颜色。
- `pointLightPositions[6]`：六个点光源的位置。
- `pointLightColors[6]`：六个点光源颜色和强度。
- `materialParams.x`：specular intensity。
- `materialParams.y`：shininess exponent。
- `lightCounts.x`：实际使用的点光源数量。

**调用流程**

CPU 端在 `updateUniformBuffer(...)` 中赋值：

```cpp
gubo.eyePos = glm::vec4(cameraPos, 1.0f);
gubo.ambientColor = glm::vec4(0.055f, 0.045f, 0.035f, 1.0f);
gubo.directionalLightDir = glm::vec4(glm::normalize(glm::vec3(-0.25f, -1.0f, -0.35f)), 0.0f);
gubo.directionalLightColor = glm::vec4(0.08f, 0.09f, 0.12f, 1.0f);
```

然后 map 到 descriptor：

```cpp
DSglobal.map(currentImage, &gubo, 0);
SC.TI[0].I[instanceId].DS[0][0]->map(currentImage, &gubo, 0);
```

GPU 端在 fragment shader 中读取：

```glsl
vec3 V = normalize(gubo.eyePos.xyz - fragPos);
vec3 color = albedo * gubo.ambientColor.rgb;
vec3 sunDir = normalize(-gubo.directionalLightDir.xyz);
int pointLightCount = min(gubo.lightCounts.x, 6);
```

**困难点与解决办法**

困难点 1：C++ 和 GLSL uniform block 顺序必须一致。

解决方法：两个 struct 的字段顺序完全对应，并使用 `vec4` / `ivec4` 和 `alignas(16)`。

困难点 2：点光源数组不能无限循环。

解决方法：shader 中使用：

```glsl
int pointLightCount = min(gubo.lightCounts.x, 6);
```

保证最多访问 6 个光源，避免越界。

**考试回答模板**

`GlobalUniformBufferObject` 是 CPU 每帧传给 shader 的全局数据。C++ 在 `updateUniformBuffer()` 中设置 camera position、ambient light、directional light、six point lights 和 material parameters，然后通过 descriptor map 给 GPU。fragment shader 用 `gubo.eyePos` 算观察方向，用 light positions 和 colors 算光照，用 `materialParams` 控制高光强度和 shininess。

## Descriptor Layouts

### [x] Descriptor Layouts 是什么？在 CG 中有什么作用？

**是什么**

Descriptor Layout 是 Vulkan 中用来描述 shader 资源接口的结构。它告诉 Vulkan：shader 会用哪些资源，这些资源在 set 和 binding 上分别是什么类型。

常见 descriptor 类型包括：

- uniform buffer
- storage buffer
- combined image sampler
- sampled image
- sampler

**原理**

shader 不能随便访问 CPU 的变量、图片或 buffer。Vulkan 要求程序显式声明 shader 资源布局，然后创建 descriptor set，把具体资源绑定进去。

Descriptor Layout 的作用类似函数签名：

- layout 说明 shader 需要什么类型的参数。
- descriptor set 是实际传入的参数。

**项目实现**

本项目有两个 descriptor layout：

```cpp
DescriptorSetLayout DSLlocal, DSLglobal;
```

`DSLglobal` 对应 shader set `0`：

```glsl
layout(binding = 0, set = 0) uniform GlobalUniformBufferObject
```

`DSLlocal` 对应 shader set `1`：

```glsl
layout(binding = 0, set = 1) uniform UniformBufferObject
layout(binding = 1, set = 1) uniform sampler2D albedoMap
```

**困难点与解决办法**

困难点：shader 中 `set` 和 `binding` 必须和 C++ descriptor layout 完全一致。

解决方法：项目明确把全局数据放在 set 0，把 per-object 数据和 texture 放在 set 1，并在 pipeline 中按顺序传入：

```cpp
{&DSLglobal, &DSLlocal}
```

**考试回答模板**

Descriptor Layout 是 Vulkan 中 shader 资源的接口描述。它定义 shader 使用哪些 uniform buffer 或 texture，以及它们位于哪个 set 和 binding。本项目用 set 0 存全局光照和相机数据，用 set 1 存每个物体的矩阵和纹理。

### [x] `DSLlocal` 和 `DSLglobal` 分别都是什么？用处是什么？

**是什么**

`DSLglobal` 是全局 descriptor set layout。

`DSLlocal` 是局部 descriptor set layout。

**项目实现**

`DSLlocal`：

```cpp
DSLlocal.init(this, {
    {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject), 1},
    {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0, 1}
});
```

它包含：

- binding 0：local uniform buffer，给 vertex shader 用。
- binding 1：texture sampler，给 fragment shader 用。

`DSLglobal`：

```cpp
DSLglobal.init(this, {
    {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS, sizeof(GlobalUniformBufferObject), 1}
});
```

它包含：

- binding 0：global uniform buffer，给 graphics shader stages 使用。

**原理**

为什么拆成两个？

因为它们变化频率不同：

- global data：camera 和 lights，一帧内对所有物体基本一样。
- local data：每个物体都有不同 model matrix 和 texture。

拆开后结构更清楚：

```text
set 0: frame/global data
set 1: object/local data
```

**困难点与解决办法**

困难点 1：如果把所有数据塞进一个 descriptor set，逻辑会混乱。

解决方法：分成 global/local 两层，符合图形学常见组织方式。

困难点 2：stage flag 要对应实际 shader 使用。

解决方法：

- local matrix 只在 vertex shader 用，所以使用 `VK_SHADER_STAGE_VERTEX_BIT`。
- texture 只在 fragment shader 采样，所以使用 `VK_SHADER_STAGE_FRAGMENT_BIT`。
- global lighting 有可能 vertex/fragment 都访问，本项目设置为 `VK_SHADER_STAGE_ALL_GRAPHICS`。

**考试回答模板**

`DSLglobal` 是 set 0，存 camera 和 lighting 这类全局数据；`DSLlocal` 是 set 1，存每个物体自己的 MVP/model matrix 和 texture sampler。这样 shader 可以同时拿到整帧共享信息和单个物体自己的信息。

### [x] `localInit()` 对应的 descriptor 部分在干什么？

**是什么**

`localInit()` 是项目初始化 Vulkan 资源和 scene 资源的核心函数之一。其中 descriptor 部分负责定义 shader 资源接口。

**项目实现**

descriptor 相关工作包括：

1. 初始化 `DSLlocal`。
2. 初始化 `DSLglobal`。
3. 初始化 vertex descriptor。
4. 初始化 render pass 和 pipeline。
5. 设置 descriptor pool size。
6. 将 vertex descriptor 和 technique 名称注册给 scene framework。
7. 加载 `scene.json`。

descriptor 最关键的代码是：

```cpp
DSLlocal.init(...)
DSLglobal.init(...)
```

之后 pipeline 使用它们：

```cpp
P.init(this, &VD, "shaders/tavern_lit.vert.spv",
                  "shaders/tavern_lit.frag.spv",
                  {&DSLglobal, &DSLlocal});
```

**原理**

Vulkan pipeline layout 必须提前知道 descriptor set layout。也就是说，创建 pipeline 之前，必须先定义 shader 会使用哪些 resources。

本项目顺序是合理的：

```text
Descriptor Layouts -> Vertex Descriptor -> Render Pass -> Pipeline -> Scene Init
```

**困难点与解决办法**

困难点：descriptor pool size 必须在 scene loading 前设置，否则 scene framework 创建 descriptor sets 时资源池可能不够。

解决方法：项目在 `SC.init(...)` 前设置：

```cpp
DPSZs.uniformBlocksInPool = 2;
DPSZs.texturesInPool = 1;
DPSZs.setsInPool = 2;
```

**考试回答模板**

`localInit()` 中 descriptor 部分是在告诉 Vulkan 和 scene framework：shader 需要哪些 uniform buffer 和 texture，以及这些资源绑定在哪些 set/binding。然后 pipeline 会使用这些 descriptor layouts，scene framework 再根据它们为每个 instance 创建实际 descriptor sets。

## Pipeline

### [x] Pipeline 是什么？在 CG 中有什么作用？

**是什么**

Graphics Pipeline 是 GPU 渲染流程的配置集合。它描述从 vertex input 到 fragment output 的整个渲染过程，包括 vertex format、shader、render pass、descriptor layouts、rasterization、depth test 等状态。

**原理**

在 Vulkan 中，pipeline 大部分状态是预先创建好的，不像一些旧 API 那样随时修改很多状态。这样做的好处是性能更稳定，但要求程序员显式配置。

一个简化的 graphics pipeline 流程是：

```text
Vertex Input
-> Vertex Shader
-> Primitive Assembly
-> Rasterization
-> Fragment Shader
-> Depth/Color Output
```

**项目实现**

本项目 pipeline 是：

```cpp
Pipeline P;
```

初始化：

```cpp
P.init(this, &VD, "shaders/tavern_lit.vert.spv",
                  "shaders/tavern_lit.frag.spv",
                  {&DSLglobal, &DSLlocal});
```

创建：

```cpp
P.create(&RP);
```

它包含：

- vertex descriptor `VD`
- vertex shader `tavern_lit.vert.spv`
- fragment shader `tavern_lit.frag.spv`
- descriptor set layouts `{&DSLglobal, &DSLlocal}`
- render pass `RP`

**困难点与解决办法**

困难点 1：pipeline 要求 shader input 和 vertex descriptor 对应。

解决方法：C++ 中 location 0/1/2 对应 shader 中 location 0/1/2。

困难点 2：pipeline layout 要求 descriptor set 顺序和 shader set 一致。

解决方法：pipeline 使用 `{&DSLglobal, &DSLlocal}`，对应 shader 中 `set = 0` 和 `set = 1`。

**考试回答模板**

Pipeline 是 Vulkan 中描述 GPU 如何绘制物体的一组固定配置。本项目的 pipeline 连接了 position-normal-UV 顶点格式、自定义 vertex/fragment shader、global/local descriptor layouts 和 main render pass。所有场景物体通过这个 pipeline 使用 Blinn-Phong lighting 渲染。

### [x] 什么是 `BlinnPhong` technique？和其他的比如 specular model、threshold in Toon shader 有什么区别？

**是什么**

在本项目中，`BlinnPhong` 是 scene framework 里的 technique 名称，用来把 `scene.json` 中的实例映射到某个 pipeline。

代码中：

```cpp
PRs[0].init("BlinnPhong", {
    {&P, {
        {},
        {
            {true, 0, {}}
        }
    }}
}, 1, &VD);
```

`scene.json` 中：

```json
"technique": "BlinnPhong"
```

这表示这组 scene instances 用 `BlinnPhong` 这个 technique，也就是使用当前的 pipeline `P` 和对应 shaders。

**原理**

Blinn-Phong 本身是一种经典局部光照模型。它通常包含：

- ambient term
- diffuse term
- specular term

Diffuse 通常基于 Lambert：

```glsl
max(dot(normal, lightDir), 0.0)
```

Specular 使用 halfway vector：

```glsl
halfwayDir = normalize(viewDir + lightDir)
pow(max(dot(normal, halfwayDir), 0.0), shininess)
```

Phong 和 Blinn-Phong 的区别：

- Phong specular 使用 reflection vector 和 view direction。
- Blinn-Phong 使用 halfway vector，通常更稳定、计算也更方便。

Toon shader 的 threshold 区别：

- Blinn-Phong 输出连续变化的亮度。
- Toon shader 会把亮度分成几个离散等级，比如亮、中、暗。
- Toon shader 常用 threshold，例如 dot product 大于 0.7 就亮，小于 0.3 就暗。

**项目实现**

fragment shader 中：

```glsl
float diffuseFactor = max(dot(normal, lightDir), 0.0);

vec3 halfwayDir = normalize(viewDir + lightDir);
float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), gubo.materialParams.y);

vec3 diffuse = albedo * diffuseFactor;
vec3 specular = vec3(gubo.materialParams.x * specularFactor);
```

这就是 Blinn-Phong 的核心。

**困难点与解决办法**

困难点：Blinn-Phong 容易太亮或太假。

解决方法：

- 使用较低 ambient。
- 使用暖色点光源。
- 使用 attenuation 控制点光源距离衰减。
- 使用 tone mapping 防止过曝。
- 使用 gamma correction 让颜色更自然。

**考试回答模板**

本项目里的 `BlinnPhong` 一方面是 scene framework 的 technique 名称，用来选择 pipeline；另一方面它也描述了 shader 使用的光照模型。它和 Toon shader 不同，Blinn-Phong 产生连续的 diffuse/specular 光照，而 Toon shader 通常用 threshold 把亮度分成离散色块。

### [x] `localInit()` 对应的 pipeline 部分在干什么？

**是什么**

`localInit()` 中 pipeline 部分负责定义渲染所需的 vertex input、render pass、shader 和 pipeline layout。

**项目实现**

主要步骤：

1. 初始化 render pass：

```cpp
RP.init(this);
RP.properties[0].clearValue = {0.025f, 0.022f, 0.020f, 1.0f};
```

2. 初始化 pipeline：

```cpp
P.init(this, &VD, "shaders/tavern_lit.vert.spv",
                  "shaders/tavern_lit.frag.spv",
                  {&DSLglobal, &DSLlocal});
```

3. 注册 technique：

```cpp
PRs[0].init("BlinnPhong", ...);
```

4. 加载 scene：

```cpp
SC.init(this, 1, VDRs, PRs, "assets/scenes/scene.json")
```

**原理**

scene framework 需要知道：

- 哪个 vertex descriptor 名称对应哪个真实 vertex format。
- 哪个 technique 名称对应哪个 pipeline。
- 每个 instance 应该使用哪个 technique。

所以 C++ 中的 `BlinnPhong` 名称必须和 `scene.json` 中的 `"technique": "BlinnPhong"` 对应。

**困难点与解决办法**

困难点：如果 technique 名称不一致，scene 无法找到 pipeline，模型不会正确渲染。

解决方法：统一使用 `BlinnPhong`，并在文档中记录。

**考试回答模板**

`localInit()` 里的 pipeline 部分是在准备渲染路线：先设置 render pass，再用 vertex descriptor、两个 shader 和 descriptor layouts 初始化 pipeline，最后把这个 pipeline 注册成 `BlinnPhong` technique，让 `scene.json` 中的实例可以使用它渲染。

## Scene Loading

### [x] 使用 `scene.json`

本项目使用 `source/assets/scenes/scene.json` 组织场景，而不是把所有物体硬编码在 C++ 中。

优点：

- 场景布局更清晰。
- 模型、纹理、实例分离。
- 调整位置、旋转、缩放时不需要改 C++ 渲染逻辑。
- 适合考试时解释 scene setup。

### [x] model 和 texture 是如何被加载的？

**是什么**

`scene.json` 中分三类：

- `models`：定义 mesh 文件。
- `textures`：定义 texture 文件。
- `instances`：定义场景中真正出现的物体。

**项目实现**

model 示例：

```json
{
  "id": "bench_table",
  "format": "MGCG",
  "VD": "VDposNormUV",
  "model": "assets/models/table.001_Mesh.7007.mgcg"
}
```

texture 示例：

```json
{
  "id": "dungeon_palette",
  "format": "Color",
  "texture": "assets/models/Textures.png"
}
```

instance 示例：

```json
{
  "id": "bar_counter_table",
  "model": "bench_table",
  "texture": ["dungeon_palette"],
  "translate": [0.0, 0.76, -9.0],
  "eulerAngles": [0.0, 0.0, 0.0],
  "scale": [1.35, 1.0, 1.05]
}
```

C++ 中加载：

```cpp
SC.init(this, 1, VDRs, PRs, "assets/scenes/scene.json")
```

**原理**

`Scene` framework 会读取 JSON：

1. 根据 `models` 加载 mesh。
2. 根据 `textures` 加载 image。
3. 根据 `instances` 创建每个物体实例。
4. 根据 `translate/eulerAngles/scale` 生成 world matrix。
5. 根据 `technique` 选择 pipeline。
6. 根据 texture id 给 descriptor set 绑定 texture sampler。

**困难点与解决办法**

困难点 1：文件路径必须相对运行目录正确。

解决方法：CMake 的 `copy_assets` target 会把 `source/assets` 拷贝到 executable 旁边的 `assets` 文件夹。

困难点 2：模型的 vertex descriptor 名称必须匹配。

解决方法：C++ 注册：

```cpp
VDRs[0].init("VDposNormUV", &VD);
```

JSON 使用：

```json
"VD": "VDposNormUV"
```

困难点 3：technique 名称必须匹配。

解决方法：C++ 注册 `BlinnPhong`，JSON 中 instances 也使用 `BlinnPhong`。

**考试回答模板**

模型和纹理不是在 C++ 中逐个硬编码加载的，而是由 `scene.json` 描述。`models` 区域定义 mesh 文件，`textures` 区域定义图片，`instances` 区域把某个 model 和 texture 放到场景中的具体位置。C++ 用 `SC.init(...)` 读取这个 JSON，并由 scene framework 创建模型、纹理、descriptor sets 和 instance transforms。

### [x] 明确 translation, Euler rotation, and scale 的三行分别对应什么？

**是什么**

这三行定义每个 instance 的空间变换：

- `translate`：平移，决定物体放在哪里。
- `eulerAngles`：欧拉角旋转，决定物体朝向。
- `scale`：缩放，决定物体大小。

**原理**

它们组合成 model matrix。常见顺序是：

```text
Model = Translation * Rotation * Scale
```

这样一个模型顶点先被缩放，再旋转，最后移动到世界位置。

**项目实现**

例如吧台：

```json
"translate": [0.0, 0.76, -9.0],
"eulerAngles": [0.0, 0.0, 0.0],
"scale": [1.35, 1.0, 1.05]
```

含义是：

- 放在世界坐标 `(0.0, 0.76, -9.0)`。
- 不旋转。
- X 方向放大 1.35，Y 方向不变，Z 方向放大 1.05。

例如右墙：

```json
"translate": [4.95, 0.0, -3.375],
"eulerAngles": [0.0, -90.0, 0.0],
"scale": [3.15, 2.05, 1.2]
```

含义是：

- 移到房间右侧。
- 绕 Y 轴旋转 -90 度。
- 缩放成适合墙面的尺寸。

**困难点与解决办法**

困难点：欧拉角顺序和坐标轴容易搞混。

解决方法：场景搭建时逐个调整，先确定房间尺寸和大件家具，再微调墙体、门、NPC 和灯光位置。

困难点：模型原始尺寸不同，scale 数值没有统一规律。

解决方法：在 scene.json 中对每个物体单独 scale，使最终视觉上统一。

**考试回答模板**

`translate` 决定物体的世界位置，`eulerAngles` 决定绕 X/Y/Z 轴的旋转，`scale` 决定三个轴上的缩放。Scene framework 会把这三部分组合成每个 instance 的 model matrix，项目中再用这个 matrix 计算 MVP 和 lighting 所需的 world position。

## Command Buffer And Draw Calls

### [x] Command Buffer And Draw Calls 是什么？在 CG 中有什么作用？

**是什么**

Command Buffer 是 Vulkan 中记录 GPU 命令的对象。Draw Call 是命令中的一种，用来告诉 GPU 绘制某个 mesh 或一批几何体。

**原理**

Vulkan 不推荐 CPU 每帧直接“立即执行”绘制命令，而是先把命令记录到 command buffer，再提交给 GPU queue 执行。

一个典型 command buffer 会包含：

- begin render pass
- bind pipeline
- bind vertex/index buffer
- bind descriptor sets
- draw indexed
- end render pass

**项目实现**

本项目 command buffer 入口：

```cpp
submitCommandBuffer("main", 0, populateCommandBufferAccess, this);
```

回调函数：

```cpp
static void populateCommandBufferAccess(VkCommandBuffer commandBuffer, int currentImage, void *Params) {
    DungeonTavernNPCApp *T = (DungeonTavernNPCApp *)Params;
    T->populateCommandBuffer(commandBuffer, currentImage);
}
```

真正记录命令：

```cpp
void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
    RP.begin(commandBuffer, currentImage);
    SC.populateCommandBuffer(commandBuffer, 0, currentImage);
    RP.end(commandBuffer);
}
```

**困难点与解决办法**

困难点：本项目使用课程 `Scene` framework，所以 draw call 细节不在 main.cpp 中完全展开。

解决方法：考试时解释项目负责的部分：

- 定义 pipeline。
- 定义 descriptor layouts。
- 加载 scene。
- 更新 uniforms。
- 调用 `SC.populateCommandBuffer(...)` 让 framework 根据 scene instances 记录实际 draw calls。

**考试回答模板**

Command Buffer 是 Vulkan 记录 GPU 绘制命令的容器，Draw Call 是其中真正发起绘制的命令。本项目在 `populateCommandBuffer()` 中开启 render pass，调用 `SC.populateCommandBuffer(...)`，然后结束 render pass。具体 bind pipeline、bind buffers、bind descriptors 和 draw 的细节由课程 Scene framework 根据我们配置的 pipeline、scene 和 uniforms 完成。

### [x] `populateCommandBuffer()` 对应的部分在干什么？

**是什么**

`populateCommandBuffer()` 是本项目每次重建主 command buffer 时记录绘制命令的函数。

**项目实现**

```cpp
void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
    RP.begin(commandBuffer, currentImage);
    SC.populateCommandBuffer(commandBuffer, 0, currentImage);
    RP.end(commandBuffer);
}
```

三步：

1. `RP.begin(...)`：开始主 render pass。
2. `SC.populateCommandBuffer(...)`：让 scene framework 为所有 scene instances 记录绘制命令。
3. `RP.end(...)`：结束 render pass。

**原理**

在 Vulkan 中，绘制必须发生在 render pass 内。render pass 描述颜色输出、depth buffer、清屏等信息。Command buffer 中的 draw commands 必须在 begin/end 之间。

**困难点与解决办法**

困难点：如果忘记 begin/end render pass，draw call 没有合法输出目标。

解决方法：函数结构非常简洁，固定保持：

```text
begin render pass -> populate scene draw calls -> end render pass
```

**考试回答模板**

`populateCommandBuffer()` 是把本帧绘制命令录入 command buffer 的地方。本项目先开启 render pass，然后调用 scene framework 绘制所有实例，最后结束 render pass。它是 CPU 告诉 GPU 这一帧要画什么的关键入口。

## Camera

### [x] Yaw/Pitch 视角变化是怎么计算的？又是怎么实现的？

**是什么**

Yaw 和 Pitch 是第一人称相机常用的两个旋转角：

- Yaw：绕竖直 Y 轴左右转头。
- Pitch：绕横向轴上下看。

本项目没有使用 roll，所以不会倾斜画面。

**原理**

通过 yaw 和 pitch 可以计算相机 forward direction：

```cpp
glm::vec3 viewForward = glm::normalize(glm::vec3(
    glm::sin(yaw) * glm::cos(pitch),
    glm::sin(pitch),
    -glm::cos(yaw) * glm::cos(pitch)
));
```

解释：

- X 分量由 `sin(yaw)` 控制左右方向。
- Z 分量由 `-cos(yaw)` 控制前后方向。
- Y 分量由 `sin(pitch)` 控制上下方向。
- `cos(pitch)` 让 pitch 越接近垂直时，水平分量越小。

**项目实现**

从输入读取旋转：

```cpp
getSixAxis(deltaT, m, r, fire);
```

更新角度：

```cpp
yaw += rotateSpeed * r.y * deltaT;
pitch += rotateSpeed * pitchInput * deltaT;
```

其中 `deltaT` 让旋转速度和帧率无关。

**困难点与解决办法**

困难点 1：上下方向和课程 `getSixAxis` 的默认映射相反。

解决方法：项目手动修正方向键：

```cpp
if(glfwGetKey(window, GLFW_KEY_UP) && !glfwGetKey(window, GLFW_KEY_DOWN)) {
    pitchInput = 1.0f;
}
if(glfwGetKey(window, GLFW_KEY_DOWN) && !glfwGetKey(window, GLFW_KEY_UP)) {
    pitchInput = -1.0f;
}
```

困难点 2：如果不乘 `deltaT`，高 FPS 机器旋转更快，低 FPS 机器旋转更慢。

解决方法：所有 movement 和 rotation 都乘以 `deltaT`。

**考试回答模板**

Yaw 控制左右看，Pitch 控制上下看。项目每帧从输入读取旋转量，用 `yaw += rotateSpeed * r.y * deltaT` 和 `pitch += rotateSpeed * pitchInput * deltaT` 更新角度，再用三角函数把 yaw/pitch 转成 `viewForward`，最后用这个方向构造 view matrix。

### [x] Pitch 是如何 clamp 来避免倒立视角的？

**是什么**

Pitch clamp 是限制上下看角度，防止相机向上或向下旋转超过 90 度导致画面翻转。

**原理**

如果 pitch 接近或超过 90 度，相机 forward direction 会越过正上方或正下方，造成控制方向反转或视觉倒立。第一人称相机通常只允许看向接近上方/下方，但不允许翻过去。

**项目实现**

```cpp
pitch = glm::clamp(pitch, glm::radians(-75.0f), glm::radians(75.0f));
```

这表示：

- 最多向下看 75 度。
- 最多向上看 75 度。
- 不允许达到 90 度或超过 90 度。

**困难点与解决办法**

困难点：如果限制太小，玩家看不到天花板或地面；如果限制太大，接近 90 度时操作可能不稳定。

解决方法：选择 `75` 度，既能看地面和天花板，又不容易产生翻转。

**考试回答模板**

项目用 `glm::clamp` 把 pitch 限制在 -75 到 75 度之间。这样玩家可以上下看，但不能翻过垂直方向，所以不会出现倒立视角或控制方向反转。

### [x] 相机运动是如何实现的？又是如何实现仅限房间内移动的？

**是什么**

相机运动就是改变 `cameraPos`。本项目使用第一人称风格，前后左右基于当前 yaw 方向移动，上下用额外的 vertical movement。

**原理**

移动方向分为：

- `moveForward`：水平前方。
- `right`：水平右方。

项目中：

```cpp
glm::vec3 moveForward(
    glm::sin(yaw),
    0.0f,
    -glm::cos(yaw)
);

glm::vec3 right(
    glm::cos(yaw),
    0.0f,
    glm::sin(yaw)
);
```

注意 `moveForward.y = 0`，所以向前走不会因为抬头而飞起来。

**项目实现**

移动代码：

```cpp
cameraPos -= moveForward * m.z * moveSpeed * deltaT;
cameraPos += right * m.x * moveSpeed * deltaT;
cameraPos.y += m.y * verticalSpeed * deltaT;
```

房间限制：

```cpp
if(cameraRoomLimitFlag == 1) {
    cameraPos = glm::clamp(cameraPos, cameraRoomMin, cameraRoomMax);
}
```

边界：

```cpp
const glm::vec3 cameraRoomMin = glm::vec3(-4.55f, 0.35f, -11.25f);
const glm::vec3 cameraRoomMax = glm::vec3(4.55f, 4.25f, 4.75f);
```

**困难点与解决办法**

困难点 1：如果直接使用 `viewForward` 移动，抬头按 W 会飞上去。

解决方法：移动使用 `moveForward`，它只包含 XZ 平面方向；视线使用 `viewForward`，它包含 pitch。

困难点 2：玩家可能穿墙或走出房间。

解决方法：用 AABB 风格的 min/max clamp 限制 camera position。

困难点 3：开发时有时需要自由相机调试。

解决方法：保留 `cameraRoomLimitFlag`，设置为 `0` 时可以关闭房间限制。

**考试回答模板**

相机位置存在 `cameraPos` 中。每帧根据输入和 yaw 计算水平 forward/right 方向，然后更新 position，并乘以 `deltaT` 保证帧率无关。为了不穿墙，项目用 `glm::clamp(cameraPos, cameraRoomMin, cameraRoomMax)` 把相机限制在房间 AABB 范围内。

### [x] `view matrix` 是如何构造的？

**是什么**

View matrix 是把 world space 转换到 camera/view space 的矩阵。它表示从相机角度看世界。

**原理**

概念上，相机在世界中移动和旋转，等价于把整个世界做相反的移动和旋转。`glm::lookAt` 可以根据 camera position、target 和 up vector 生成 view matrix。

**项目实现**

```cpp
View = glm::lookAt(
    cameraPos,
    cameraPos + viewForward,
    glm::vec3(0.0f, 1.0f, 0.0f)
);
```

参数含义：

- `cameraPos`：相机位置。
- `cameraPos + viewForward`：相机看向的目标点。
- `(0, 1, 0)`：世界上方向。

然后：

```cpp
ViewPrj = Prj * View;
```

用于后续每个 instance 的 MVP。

**困难点与解决办法**

困难点：`lookAt` 第二个参数不是方向，而是目标点。

解决方法：使用 `cameraPos + viewForward`，把方向转换成目标点。

困难点：如果 up vector 错误，画面可能倾斜。

解决方法：本项目没有 roll，固定使用世界 Y 轴作为 up vector。

**考试回答模板**

View matrix 用来把世界坐标转换到相机坐标。本项目先由 yaw/pitch 算出 `viewForward`，再调用 `glm::lookAt(cameraPos, cameraPos + viewForward, up)`。第二个参数是相机看向的点，所以要用当前位置加 forward direction。

## Projection

### [x] Project 用的是什么透视？Parallel / perspective？在哪里体现的？

**是什么**

本项目使用 perspective projection，也就是透视投影，不是 parallel/orthographic projection。

**原理**

透视投影模拟人眼或相机：

- 近处物体看起来大。
- 远处物体看起来小。
- 平行线可能在远处汇聚。

平行投影则不随距离缩放，常用于 CAD、工程图或等距视角。

**项目实现**

在 `GameLogic()` 中：

```cpp
const float FOVy = glm::radians(45.0f);
const float nearPlane = 0.1f;
const float farPlane = 100.f;

glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
```

`glm::perspective(...)` 明确表示透视投影。

参数：

- `FOVy = 45°`：垂直视野角。
- `Ar`：窗口宽高比。
- `nearPlane = 0.1`：近裁剪面。
- `farPlane = 100`：远裁剪面。

**困难点与解决办法**

困难点：FOV 太大容易变形，太小视野太窄。

解决方法：选择 45 度，是常见且稳定的第一人称展示 FOV。

困难点：near/far 范围不合理会导致 depth precision 问题。

解决方法：场景是小房间，`0.1` 到 `100` 足够覆盖，同时不会过于极端。

**考试回答模板**

项目使用 perspective projection，通过 `glm::perspective(FOVy, Ar, nearPlane, farPlane)` 创建。它让近处物体更大、远处物体更小，符合第一人称 tavern 场景的视觉效果。

### [x] `Prj[1][1] *= -1` 是什么意思？

**是什么**

这是为了适配 Vulkan 坐标系统，对 GLM 生成的投影矩阵做 Y 方向翻转。

**原理**

GLM 的 `glm::perspective` 默认更接近 OpenGL 的 clip coordinate 约定，而 Vulkan 的 normalized device coordinates 在 Y 方向和 OpenGL 有差异。如果不翻转，最终画面可能上下颠倒。

投影矩阵中的 `[1][1]` 影响 Y 坐标缩放。乘以 `-1` 等价于翻转 clip-space Y 方向。

**项目实现**

```cpp
glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
Prj[1][1] *= -1;
```

**困难点与解决办法**

困难点：如果忘记这一行，可能出现画面倒置或和输入方向不一致。

解决方法：保留 Vulkan skeleton 常用修正方式，在 projection 创建后立即翻转 Y。

**考试回答模板**

`Prj[1][1] *= -1` 是 Vulkan 投影矩阵的 Y 轴修正。GLM 的 perspective 默认坐标约定和 Vulkan 不完全一致，所以项目把投影矩阵的 Y 分量翻转，避免画面上下颠倒。

### [x] `ViewPrj = Prj * View;` 又做了什么？

**是什么**

`ViewPrj` 是 View matrix 和 Projection matrix 的组合矩阵。

**原理**

每个物体最终需要：

```text
Projection * View * Model
```

其中 `Projection * View` 对所有物体一样，因为同一帧里相机和投影相同。可以先合并成 `ViewPrj`，再对每个物体乘自己的 model matrix。

**项目实现**

```cpp
ViewPrj = Prj * View;
```

之后每个 instance：

```cpp
ubo.mvpMat = ViewPrj * ubo.mMat;
```

所以：

```text
ubo.mvpMat = Projection * View * Model
```

**困难点与解决办法**

困难点：矩阵乘法不满足交换律，顺序错了会导致物体位置完全错误。

解决方法：固定使用 `Projection * View * Model` 顺序，并在代码中分两步清楚表示。

**考试回答模板**

`ViewPrj = Prj * View` 是把投影矩阵和相机矩阵先合并起来。之后对每个物体只需要再乘它自己的 model matrix，得到 `Projection * View * Model`，也就是 vertex shader 用来计算 `gl_Position` 的 MVP。

## Lighting

### [x] `updateUniformBuffer()` 这部分在干什么？

**是什么**

`updateUniformBuffer()` 是每帧更新运行状态的核心函数。它不仅更新 uniform buffer，也调用 game logic、处理退出、更新 FPS 和文字 command buffer。

**项目实现**

主要工作：

1. ESC 退出：

```cpp
if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, GL_TRUE);
}
```

2. 调用 `GameLogic()` 更新 camera 和 interaction：

```cpp
float deltaT = GameLogic();
```

3. 填充 global lighting uniform：

```cpp
GlobalUniformBufferObject gubo{};
gubo.eyePos = glm::vec4(cameraPos, 1.0f);
gubo.ambientColor = glm::vec4(0.055f, 0.045f, 0.035f, 1.0f);
...
```

4. 设置六个点光源：

```cpp
gubo.pointLightPositions[0] = glm::vec4(0.0f, 3.35f, -11.15f, 1.0f);
...
gubo.pointLightPositions[5] = glm::vec4(0.0f, 3.85f, -2.70f, 1.0f);
```

5. 把 global uniform map 到 GPU：

```cpp
DSglobal.map(currentImage, &gubo, 0);
```

6. 遍历 scene instances，更新每个物体的 local uniform：

```cpp
for(instanceId = 0; instanceId < SC.TI[0].InstanceCount; instanceId++) {
    ubo.mMat = SC.TI[0].I[instanceId].Wm;
    ubo.mvpMat = ViewPrj * ubo.mMat;
    SC.TI[0].I[instanceId].DS[0][0]->map(currentImage, &gubo, 0);
    SC.TI[0].I[instanceId].DS[0][1]->map(currentImage, &ubo, 0);
}
```

7. 更新 FPS 和 interaction text：

```cpp
updateInteractionText();
txt.updateCommandBuffer();
```

**原理**

每帧 camera 位置可能变，NPC 状态可能变，lights 和 matrices 也要送给 GPU。shader 只有拿到最新 uniform 才能渲染正确画面。

**困难点与解决办法**

困难点 1：相机移动后，如果不更新 MVP，画面不会按新相机变化。

解决方法：每帧先调用 `GameLogic()` 更新 `ViewPrj`，再遍历所有 instances 更新 `ubo.mvpMat`。

困难点 2：shader 光照需要 camera position，如果只更新 matrix 不更新 `eyePos`，specular 会错误。

解决方法：每帧设置：

```cpp
gubo.eyePos = glm::vec4(cameraPos, 1.0f);
```

困难点 3：文字如果只和 FPS 一起 1 秒更新，会有延迟。

解决方法：interaction text 每帧检查变化，并调用 `txt.updateCommandBuffer()`。

**考试回答模板**

`updateUniformBuffer()` 是每帧同步 CPU 状态到 GPU 的地方。它先处理输入和 camera/interactions，然后填充 global lighting uniform，再遍历每个 scene instance 计算 model matrix 和 MVP matrix，并 map 到 descriptor sets。最后更新 FPS 和交互文字。没有这一步，shader 就拿不到最新 camera、lighting 和 object transform。

## NPC Interaction

### [x] 解释一下 `E` is edge-triggered with `interactionKeyWasPressed`.

**是什么**

Edge-triggered input 指的是只在按键从“未按下”变成“按下”的那一帧触发动作，而不是按住期间每帧都触发。

**原理**

程序每秒可能运行 60 帧或更多。如果直接写：

```cpp
if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    interact();
}
```

那么玩家按住 E 半秒，可能触发 30 次交互。这对注册、结账、打开菜单这种状态切换会出错。

所以需要保存上一帧状态：

- 当前帧 E 是否按下：`ePressed`
- 上一帧 E 是否按下：`interactionKeyWasPressed`

只有当前按下且上一帧没按，才是 just pressed：

```cpp
const bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
const bool eJustPressed = ePressed && !interactionKeyWasPressed;
...
interactionKeyWasPressed = ePressed;
```

**项目实现**

在 `GameLogic()` 中：

```cpp
const bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
const bool eJustPressed = ePressed && !interactionKeyWasPressed;
```

交互逻辑只响应 `eJustPressed`：

```cpp
if(eJustPressed) {
    ...
}
```

函数末尾更新上一帧状态：

```cpp
interactionKeyWasPressed = ePressed;
```

**困难点与解决办法**

困难点：如果不做 edge trigger，注册后同一次按住可能马上又 checkout，状态来回切换。

解决方法：所有 E 触发状态切换都使用 `eJustPressed`，不是 `ePressed`。

**考试回答模板**

`interactionKeyWasPressed` 保存上一帧 E 键状态。本帧如果 E 是 pressed，而上一帧不是 pressed，就说明这是一次新的按键边沿，变量 `eJustPressed` 才为 true。这样按住 E 不会每帧重复触发注册、点单或 checkout。

### [x] 搞清楚点单交互的状态机

**是什么**

状态机是用一组 boolean 和字符串描述 NPC 交互流程。本项目没有复杂 dialogue system，而是用简单状态变量实现 greeter 和 bartender 的逻辑。

**相关状态变量**

```cpp
bool registered = false;
bool orderCompleted = false;
bool interactionKeyWasPressed = false;
bool drinkChoiceKeyWasPressed = false;
bool bartenderMenuOpen = false;
std::string message = "";
std::string greeterFeedback = "";
std::string selectedDrink = "";
```

含义：

- `registered`：玩家是否已经在 greeter 登记。
- `bartenderMenuOpen`：bartender 菜单是否打开。
- `orderCompleted`：是否已经点完一杯饮料。
- `selectedDrink`：当前选择的饮料名称。
- `message`：当前屏幕显示文字。
- `greeterFeedback`：greeter 的反馈文字。

**距离判断**

```cpp
const bool nearGreeter = horizontalDistance(cameraPos, greeterPos) < greeterInteractionRadius;
const bool nearBartender = horizontalDistance(cameraPos, bartenderPos) < bartenderInteractionRadius;
```

使用 XZ 平面距离：

```cpp
float horizontalDistance(glm::vec3 a, glm::vec3 b) {
    return glm::distance(glm::vec2(a.x, a.z), glm::vec2(b.x, b.z));
}
```

**Greeter 状态**

如果靠近 greeter：

1. 未注册时，显示：

```text
Press E to register
```

2. 按 E 后：

```cpp
registered = true;
greeterFeedback = "Registration completed. You may proceed to order.\nPress E to check out";
```

3. 已注册再按 E：

```cpp
registered = false;
orderCompleted = false;
bartenderMenuOpen = false;
selectedDrink.clear();
```

这表示 checkout，并重置 bartender 状态。

**Bartender 状态**

如果靠近 bartender：

1. 未注册：

```text
Hello dear customer, please register with the greeter before ordering.
```

2. 已注册但菜单未打开、未点单：

```text
Press E to order
```

3. 按 E 后：

```cpp
bartenderMenuOpen = true;
message = drinkChoiceMenuText();
```

显示：

```text
What would you like to drink?
1. Ale
2. Red wine
3. Water
```

4. 按 `1/2/3`：

```cpp
selectedDrink = drinkNameFromChoice(drinkChoice);
orderCompleted = true;
bartenderMenuOpen = false;
message = "You ordered " + selectedDrink + ". Enjoy!";
```

5. 已点单后按 E 可以重新点：

```cpp
orderCompleted = false;
selectedDrink.clear();
bartenderMenuOpen = true;
```

6. 离开 bartender 范围：

```cpp
if(!nearBartender) {
    orderCompleted = false;
    bartenderMenuOpen = false;
    selectedDrink.clear();
}
```

**数字键也做了 edge trigger**

```cpp
const bool drinkChoicePressed = key1Pressed || key2Pressed || key3Pressed;
const bool drinkChoiceJustPressed = drinkChoicePressed && !drinkChoiceKeyWasPressed;
...
drinkChoiceKeyWasPressed = drinkChoicePressed;
```

**困难点与解决办法**

困难点 1：按住 E 会反复切换。

解决方法：使用 `eJustPressed`。

困难点 2：按住数字键会重复点单。

解决方法：使用 `drinkChoiceJustPressed`。

困难点 3：离开 NPC 后文字残留或状态不清楚。

解决方法：每帧 `message.clear()`，根据当前位置重新设置；离开 bartender 时清空 order/menu 状态。

困难点 4：相机高度变化可能影响距离判断。

解决方法：使用 horizontal XZ distance，而不是 3D distance。

**考试回答模板**

点单系统是一个简单状态机。玩家必须先在 greeter 注册，`registered` 变成 true 后，靠近 bartender 才能按 E 打开菜单。菜单打开时按 1/2/3 选择饮料，设置 `selectedDrink` 并把 `orderCompleted` 设为 true。按键都用 edge trigger，避免按住一键触发多次。离开 bartender 范围后，菜单和订单状态会被清空，保证演示流程稳定。

## Text Output

### [x] 文字是怎么显示的？又是怎么储存和消失的？display 逻辑是什么？

**是什么**

本项目使用课程提供的 `TextMaker` 显示屏幕文字。文字主要用于：

- FPS 显示。
- NPC interaction prompt。
- Bartender menu。
- Greeter feedback。

**项目实现**

TextMaker 对象：

```cpp
TextMaker txt;
```

初始化：

```cpp
txt.init(this, windowWidth, windowHeight);
```

窗口大小变化时：

```cpp
txt.resizeScreen(w, h);
```

FPS 初始文字：

```cpp
txt.print(1.0f, 1.0f, "FPS:", 1, ...);
```

interaction text 由两个字符串控制：

```cpp
std::string message = "";
std::string displayedInteractionMessage = "";
```

`message` 表示当前逻辑想显示什么。

`displayedInteractionMessage` 表示屏幕上已经显示的内容。

**显示逻辑**

```cpp
void updateInteractionText() {
    const int interactionTextId = 3;

    if(message == displayedInteractionMessage) {
        return;
    }

    if(message.empty()) {
        txt.removeText(interactionTextId);
    } else {
        txt.print(-0.95f, -0.95f, message, interactionTextId, "CO", false, true, true,
                  TAL_LEFT, TRH_LEFT, TRV_TOP,
                  {1.0f,1.0f,1.0f,1.0f},
                  {0.0f,0.0f,0.0f,1.0f});
    }

    displayedInteractionMessage = message;
}
```

解释：

- 使用固定 id `3` 管理 interaction text。
- 如果新 message 和旧 message 一样，不重复 print。
- 如果 message 为空，调用 `removeText(3)`。
- 如果 message 非空，调用 `txt.print(...)` 更新文字。

**文字如何消失**

在 `GameLogic()` 中每帧先：

```cpp
message.clear();
```

如果不靠近 NPC，就不会重新设置 message。之后 `updateInteractionText()` 发现 message 为空，就调用：

```cpp
txt.removeText(interactionTextId);
```

**困难点与解决办法**

困难点 1：早期 interaction text 如果放在 FPS 的 1 秒更新块里，会刷新慢。

解决方法：把 interaction text 更新独立出来，每帧调用 `updateInteractionText()`。

困难点 2：离开 NPC 后文字可能残留。

解决方法：每帧 `message.clear()`，并在 message 为空时 `removeText(3)`。

困难点 3：每帧重复 print 同样文字可能浪费或闪烁。

解决方法：用 `displayedInteractionMessage` 做比较，只有变化时才更新。

**考试回答模板**

文字由 `TextMaker` 显示。逻辑层每帧计算当前应该显示的 `message`，`updateInteractionText()` 比较它和已经显示的 `displayedInteractionMessage`。如果内容没变就不更新；如果新 message 为空就用 `removeText(3)` 删除；否则用固定 id `3` 重新 print。这样文字能即时出现和消失，也不会每帧重复刷新。

## Shader

### [x] Vertex Shader 是什么？作用是什么？由谁调用？

**是什么**

Vertex Shader 是 GPU graphics pipeline 中处理每个顶点的 shader stage。它对每个输入顶点运行一次。

它的主要任务是：

- 接收 vertex buffer 中的顶点属性。
- 计算顶点最终的 clip-space position。
- 把需要插值的数据传给 fragment shader。

**由谁调用**

它不是 C++ 手动逐个调用的，而是 GPU 在 draw call 执行时自动调用。C++ 只是在 pipeline 中指定使用哪个 vertex shader：

```cpp
P.init(this, &VD, "shaders/tavern_lit.vert.spv",
                  "shaders/tavern_lit.frag.spv",
                  {&DSLglobal, &DSLlocal});
```

当 command buffer 中 draw call 执行时，GPU 会对每个 vertex 运行 `tavern_lit.vert`。

**项目实现**

`source/shaders/tavern_lit.vert`：

```glsl
layout(binding = 0, set = 1) uniform UniformBufferObject {
    mat4 mvpMat;
    mat4 mMat;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragUV;

void main() {
    gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
    fragPos = (ubo.mMat * vec4(inPosition, 1.0)).xyz;
    fragNormal = normalize(mat3(transpose(inverse(ubo.mMat))) * inNormal);
    fragUV = inUV;
}
```

逐行解释：

- `ubo.mvpMat`：local uniform，负责最终投影。
- `ubo.mMat`：model matrix，负责 world-space position 和 normal。
- `inPosition`：来自 vertex buffer 的 position。
- `inNormal`：来自 vertex buffer 的 normal。
- `inUV`：来自 vertex buffer 的 texture coordinate。
- `gl_Position`：GPU 必须拿到的 clip-space position。
- `fragPos`：传给 fragment shader 的 world position。
- `fragNormal`：传给 fragment shader 的 world normal。
- `fragUV`：传给 fragment shader 的 UV。

**原理**

vertex shader 输出的 `fragPos`、`fragNormal`、`fragUV` 会在 rasterization 阶段被插值。也就是说，一个 triangle 的三个顶点都有这些值，GPU 会自动为 triangle 内部每个 fragment 计算中间值。

例如一个三角形顶点 UV 分别是 A/B/C，fragment shader 中拿到的 `fragUV` 是插值后的结果，用它采样纹理就能得到连续的贴图效果。

**困难点与解决办法**

困难点 1：必须写 `gl_Position`。

解决方法：用 `mvpMat * vec4(inPosition, 1.0)` 计算。

困难点 2：normal 在非均匀缩放下会错误。

解决方法：用 inverse-transpose normal matrix。

困难点 3：shader input location 必须和 C++ vertex descriptor 一致。

解决方法：C++ 和 GLSL 都使用 location 0/1/2 对应 position/normal/UV。

**考试回答模板**

Vertex shader 是 GPU 对每个顶点执行的程序。本项目 vertex shader 接收 position、normal、UV，用 MVP 矩阵计算 `gl_Position`，用 model matrix 计算 world position 和 world normal，并把 normal、position、UV 传给 fragment shader。它由 Vulkan pipeline 在 draw call 执行时自动调用。

### [x] Fragment Shader 是什么？作用是什么？由谁调用？

**是什么**

Fragment Shader 是 GPU graphics pipeline 中处理每个 fragment 的 shader stage。fragment 可以理解为“候选像素”，它经过 depth test 等步骤后可能成为屏幕上的最终像素。

Fragment Shader 的主要任务是计算颜色。

**由谁调用**

它同样不是 C++ 手动调用的。C++ 在 pipeline 中指定 fragment shader：

```cpp
"shaders/tavern_lit.frag.spv"
```

当 GPU rasterize triangles 后，会为 triangle 覆盖到的 fragments 自动执行 fragment shader。

**项目实现**

`source/shaders/tavern_lit.frag` 输入：

```glsl
layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;
```

texture sampler：

```glsl
layout(binding = 1, set = 1) uniform sampler2D albedoMap;
```

global uniform：

```glsl
layout(binding = 0, set = 0) uniform GlobalUniformBufferObject { ... } gubo;
```

输出：

```glsl
layout(location = 0) out vec4 outColor;
```

核心流程：

1. 归一化 normal：

```glsl
vec3 N = normalize(fragNormal);
```

2. 计算 view direction：

```glsl
vec3 V = normalize(gubo.eyePos.xyz - fragPos);
```

3. 采样 texture：

```glsl
vec3 albedo = pow(texture(albedoMap, fragUV).rgb, vec3(2.2));
```

4. 加 ambient：

```glsl
vec3 color = albedo * gubo.ambientColor.rgb;
```

5. 加 directional light：

```glsl
vec3 sunDir = normalize(-gubo.directionalLightDir.xyz);
color += applyLight(albedo, N, V, sunDir, gubo.directionalLightColor.rgb, 1.0);
```

6. 加 point lights：

```glsl
for(int i = 0; i < pointLightCount; i++) {
    color += applyPointLight(albedo, N, V, i);
}
```

7. tone mapping 和 gamma correction：

```glsl
color = color / (color + vec3(1.0));
color = pow(color, vec3(1.0 / 2.2));
```

8. 输出颜色：

```glsl
outColor = vec4(color, 1.0);
```

**原理**

Fragment shader 基于插值后的 world position、normal 和 UV，计算每个 fragment 的最终颜色。

光照核心在 `applyLight(...)`：

```glsl
float diffuseFactor = max(dot(normal, lightDir), 0.0);
vec3 halfwayDir = normalize(viewDir + lightDir);
float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), gubo.materialParams.y);
```

Diffuse 表示表面朝向光源的程度。

Specular 表示高光，取决于 view direction 和 light direction 的 halfway vector。

Point light attenuation：

```glsl
float attenuation = 1.0 / (1.0 + 0.45 * distanceToLight + 0.22 * distanceToLight * distanceToLight);
```

距离越远，光照越弱。

**困难点与解决办法**

困难点 1：颜色空间问题。直接在线性计算和 sRGB 显示之间混用，颜色可能不自然。

解决方法：采样后做近似 gamma decode：

```glsl
pow(texture(...).rgb, vec3(2.2))
```

输出前做 gamma encode：

```glsl
pow(color, vec3(1.0 / 2.2))
```

困难点 2：多个点光源可能过亮。

解决方法：加入 attenuation 和 tone mapping：

```glsl
color = color / (color + vec3(1.0));
```

困难点 3：没有 ambient 时室内背光面会全黑。

解决方法：加入低强度 warm ambient：

```cpp
gubo.ambientColor = glm::vec4(0.055f, 0.045f, 0.035f, 1.0f);
```

**考试回答模板**

Fragment shader 是 GPU 对每个 fragment 执行的程序，主要负责计算最终颜色。本项目 fragment shader 接收 vertex shader 传来的 world position、normal 和 UV，先采样 texture 得到 albedo，再用 global uniform 中的 camera、ambient light、directional light 和 point lights 计算 Blinn-Phong 光照，最后做 tone mapping 和 gamma correction 输出 `outColor`。

### [x] 这部分完全 0 基础：Vertex Shader 和 Fragment Shader 分别是什么，作用是什么，由谁调用？

**最简单理解**

可以把 GPU 渲染一个模型想成两大问题：

1. 这个三角形应该画在屏幕哪里？
2. 这个像素应该是什么颜色？

Vertex Shader 主要回答第一个问题。

Fragment Shader 主要回答第二个问题。

**完整流程**

```text
CPU records draw call
    -> GPU reads vertex buffer
    -> Vertex Shader runs once per vertex
    -> GPU assembles triangles
    -> GPU rasterizes triangles into fragments
    -> Fragment Shader runs once per fragment
    -> depth/color output becomes final image
```

**Vertex Shader 做什么**

输入：

- 顶点位置
- 顶点法线
- 顶点 UV
- uniform matrices

输出：

- `gl_Position`
- 要传给 fragment shader 的数据

本项目中它做：

```text
local vertex position -> clip-space position
local normal -> world-space normal
local UV -> fragment UV
```

**Fragment Shader 做什么**

输入：

- 插值后的 world position
- 插值后的 normal
- 插值后的 UV
- texture
- lighting uniforms

输出：

- `outColor`

本项目中它做：

```text
texture color + ambient + diffuse + specular + attenuation -> final pixel color
```

**由谁调用**

不是我们在 C++ 中写：

```cpp
vertexShader();
fragmentShader();
```

而是：

1. C++ 创建 pipeline，指定 shader 文件。
2. C++ 记录 draw call。
3. Vulkan/GPU 执行 draw call。
4. GPU 自动调用 vertex shader 和 fragment shader。

**项目中 C++ 和 shader 的连接**

C++ pipeline：

```cpp
P.init(this, &VD, "shaders/tavern_lit.vert.spv",
                  "shaders/tavern_lit.frag.spv",
                  {&DSLglobal, &DSLlocal});
```

这告诉 Vulkan：

- 顶点阶段使用 `tavern_lit.vert.spv`。
- 像素/fragment 阶段使用 `tavern_lit.frag.spv`。
- shader 资源来自 global/local descriptor sets。

**困难点与解决办法**

困难点 1：初学时容易以为 shader 是普通 C++ 函数。

解决方法：理解 shader 是 GPU 程序，由 pipeline 和 draw call 间接触发。

困难点 2：vertex shader 和 fragment shader 的变量要通过 location 对接。

解决方法：本项目统一使用：

```glsl
layout(location = 0) out vec3 fragPos;
layout(location = 0) in vec3 fragPos;
```

location 一致，GPU 才知道如何把 vertex 输出传给 fragment 输入。

困难点 3：shader 中的 uniform set/binding 要和 C++ descriptor layout 对接。

解决方法：本项目明确：

- set 0 binding 0：global lighting uniform。
- set 1 binding 0：local matrix uniform。
- set 1 binding 1：texture sampler。

**考试回答模板**

Vertex shader 和 fragment shader 都是 GPU 程序，不是 C++ 直接调用的函数。C++ 在 Vulkan pipeline 中指定它们，然后 draw call 执行时 GPU 自动调用。Vertex shader 对每个顶点运行，负责把模型顶点变换到屏幕相关的 clip space，并传出 normal/UV 等数据。Fragment shader 对每个 fragment 运行，负责根据 texture、normal、light 和 camera 计算最终颜色。

## Cross-Topic Summary

如果老师要求从整体上解释一次渲染流程，可以按这个顺序讲：

1. `scene.json` 定义模型、纹理和实例 transform。
2. `SC.init(...)` 加载 scene。
3. `Vertex` 和 `VD.init(...)` 定义 GPU 如何读取每个顶点。
4. `DSLglobal` 和 `DSLlocal` 定义 shader 需要哪些 uniform 和 texture。
5. `P.init(...)` 把 vertex format、shader、descriptor layouts 和 render pass 组合成 pipeline。
6. 每帧 `GameLogic()` 更新 camera 和 interaction。
7. `updateUniformBuffer()` 更新 global lighting uniform 和每个 instance 的 MVP/model matrix。
8. `populateCommandBuffer()` 开始 render pass，并让 scene framework 记录 draw calls。
9. GPU 执行 draw call，先运行 vertex shader，再 rasterize，再运行 fragment shader。
10. fragment shader 输出最终颜色，屏幕显示 dungeon tavern scene。

可以背的最终短版：

> The project uses a scene JSON file to define models, textures, and instance transforms. In C++, we define a position-normal-UV vertex format, global and local descriptor layouts, and one Blinn-Phong graphics pipeline. Every frame, the program updates the camera, interaction state, lighting uniform buffer, and per-object MVP matrices. The command buffer starts the render pass and asks the scene framework to draw all instances. On the GPU, the vertex shader transforms vertices and passes world position, normal, and UV; the fragment shader samples textures and applies ambient, diffuse, specular, point lights, attenuation, tone mapping, and gamma correction.
