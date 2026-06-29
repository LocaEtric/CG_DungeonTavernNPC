# 2026-06-29 037 Document Shader And Pipeline

## Date

2026-06-29

## Goal

Document the final shader and graphics pipeline design so the rendering path can be explained clearly during the exam.

This step does not change source code. It records how the existing implementation satisfies the course requirements for vertex formats, descriptor layouts, uniform buffers, graphics pipelines, command buffer population, draw calls, and shader lighting.

## Changed Files

- `0.Documentation/DevelopmentLog/2026-06-29-037-document-shader-pipeline.md`
- `0.Documentation/ProjectPlan.md`
- `0.Documentation/ExamReview.md`

## Relevant Source Files

- `source/src/main.cpp`
- `source/shaders/tavern_lit.vert`
- `source/shaders/tavern_lit.frag`
- `source/assets/scenes/scene.json`

## Rendering Data Flow

The project uses the provided Vulkan course skeleton and implements the project-specific rendering setup in `source/src/main.cpp`.

The high-level rendering flow is:

1. `scene.json` defines models, textures, and scene instances.
2. `Scene SC` loads the assets and instance transforms.
3. `VertexDescriptor VD` describes the vertex memory layout.
4. `DescriptorSetLayout DSLglobal` describes scene-wide lighting and camera data.
5. `DescriptorSetLayout DSLlocal` describes per-object matrices and texture sampler data.
6. `Pipeline P` connects the vertex descriptor, shaders, descriptor layouts, and render pass.
7. Each frame updates global and local uniform buffers.
8. The command buffer asks `SC.populateCommandBuffer(...)` to bind resources and issue draw calls for scene instances.

## Vertex Format

The final vertex format is:

```cpp
struct Vertex {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 UV;
};
```

The corresponding `VertexDescriptor` maps:

- location `0`: position, `VK_FORMAT_R32G32B32_SFLOAT`
- location `1`: normal, `VK_FORMAT_R32G32B32_SFLOAT`
- location `2`: UV, `VK_FORMAT_R32G32_SFLOAT`

This is enough for textured Blinn-Phong lighting:

- position is used for the final clip-space vertex position and world-space light vectors
- normal is used for diffuse and specular lighting
- UV is used to sample the albedo texture

## Uniform Buffers

### Local Uniform Buffer

`UniformBufferObject` stores per-instance transform data:

```cpp
struct UniformBufferObject {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
};
```

- `mvpMat` transforms object-space vertices to clip space.
- `mMat` transforms object-space positions to world space.
- The vertex shader also uses `mMat` to transform normals with the inverse-transpose normal matrix.

### Global Uniform Buffer

`GlobalUniformBufferObject` stores per-frame scene data:

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

It contains:

- camera position for specular view direction
- ambient light color
- weak directional light
- six warm point lights placed near candles and the chandelier
- material parameters, currently specular intensity and shininess
- active point light count

The global uniform buffer is updated every frame before drawing.

## Descriptor Layouts

The project uses two descriptor set layouts.

### Set 0: Global Descriptor Set

`DSLglobal` contains one uniform buffer:

- binding `0`
- type `VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER`
- visible to `VK_SHADER_STAGE_ALL_GRAPHICS`
- data type: `GlobalUniformBufferObject`

This set is shared by the whole scene because lighting and camera position are global frame data.

### Set 1: Local Descriptor Set

`DSLlocal` contains:

- binding `0`: uniform buffer for `UniformBufferObject`
- binding `1`: combined image sampler for the albedo texture

This set is per scene instance because each object has its own model matrix and texture.

## Graphics Pipeline

The graphics pipeline is created with:

```cpp
P.init(this, &VD,
       "shaders/tavern_lit.vert.spv",
       "shaders/tavern_lit.frag.spv",
       {&DSLglobal, &DSLlocal});
```

The pipeline uses:

- vertex descriptor: `VDposNormUV`
- vertex shader: `tavern_lit.vert`
- fragment shader: `tavern_lit.frag`
- descriptor set 0: global lighting/camera data
- descriptor set 1: local object transform and texture data
- render pass: the main on-screen render pass from the course skeleton

The scene technique is named `BlinnPhong` in `scene.json` and maps all current scene instances to this pipeline.

## Render Pass And Command Buffer

The project uses a single main render pass:

```cpp
RP.begin(commandBuffer, currentImage);
SC.populateCommandBuffer(commandBuffer, 0, currentImage);
RP.end(commandBuffer);
```

`SC.populateCommandBuffer(...)` is provided by the course scene framework. It uses the scene data, descriptor sets, model buffers, textures, and active pipeline to record the draw calls for all instances.

The project-specific part is that `main.cpp` defines the pipeline, descriptor layouts, scene technique references, and per-frame uniform data that the scene framework uses when recording the command buffer.

## Vertex Shader

`tavern_lit.vert` receives:

- position
- normal
- UV

It outputs:

- world-space fragment position
- transformed normal
- UV

Important operations:

```glsl
gl_Position = ubo.mvpMat * vec4(inPosition, 1.0);
fragPos = (ubo.mMat * vec4(inPosition, 1.0)).xyz;
fragNormal = normalize(mat3(transpose(inverse(ubo.mMat))) * inNormal);
fragUV = inUV;
```

The inverse-transpose normal transform is important because normal vectors should remain correct under non-uniform scaling.

## Fragment Shader

`tavern_lit.frag` samples the albedo texture and computes lighting.

The lighting model includes:

- ambient light
- weak directional light
- up to six point lights
- Lambert diffuse term
- Blinn-Phong specular term
- distance attenuation for point lights
- tone mapping
- gamma correction

The shader first samples albedo:

```glsl
vec3 albedo = pow(texture(albedoMap, fragUV).rgb, vec3(2.2));
```

Then it starts with ambient light:

```glsl
vec3 color = albedo * gubo.ambientColor.rgb;
```

Then it adds directional and point lights. The shared helper function computes diffuse and specular light:

```glsl
float diffuseFactor = max(dot(normal, lightDir), 0.0);
vec3 halfwayDir = normalize(viewDir + lightDir);
float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), gubo.materialParams.y);
```

The final color is tone mapped and converted back to display gamma:

```glsl
color = color / (color + vec3(1.0));
color = pow(color, vec3(1.0 / 2.2));
```

## Lighting Design

The lighting is designed for a closed dungeon tavern:

- low ambient light keeps the room dark
- warm point lights represent candles and the chandelier
- a weak cool directional light gives slight shape to surfaces
- point light attenuation makes light fall off naturally with distance

This is intentionally simpler than PBR. It is easier to explain and is sufficient for the course project.

## Tested Or Verified

The team manually verified the final runtime features:

- the project builds and runs in CLion
- the scene is visible
- textures render correctly
- point lights affect the tavern scene
- camera movement works
- NPC interaction text works
- bartender and greeter flows work

This documentation-only step did not require another code build.

## Notes For Exam Review

- The rendering path is intentionally a single-pipeline design.
- All visible scene objects use the same vertex format and `BlinnPhong` technique.
- The project uses two descriptor sets to separate global frame data from local per-object data.
- The shader is custom and understandable by the team.
- The lighting model is simple enough to explain line by line:
  - texture albedo
  - ambient term
  - diffuse term
  - Blinn-Phong specular term
  - point light attenuation
  - tone mapping and gamma correction
