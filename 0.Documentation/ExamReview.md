# Dungeon Tavern NPC Exam Review

## One-Minute Project Summary

This project is a real-time 3D dungeon tavern scene built with C++ and Vulkan using the provided Computer Graphics course skeleton.

The user moves in first person inside a closed tavern, approaches two NPCs, and interacts with them through distance-based prompts and keyboard input. The scene uses textured `.MGCG` and OBJ models, custom shaders, a position-normal-UV vertex format, uniform buffers, descriptor layouts, a graphics pipeline, command buffer draw calls, camera transforms, and Blinn-Phong lighting.

## Demo Path

1. Launch the application.
2. Show the dungeon tavern environment.
3. Move with the first-person camera.
4. Look at the textured walls, floor, furniture, candles, and chandelier.
5. Approach the door greeter.
6. Press `E` to register.
7. Move to the bartender behind the counter.
8. Press `E` to open the drink menu.
9. Press `1`, `2`, or `3` to order a drink.
10. Move away and show that the interaction text disappears.
11. Return to the greeter and press `E` to check out.
12. Explain the rendering and interaction code.

## Main Files To Explain

| Topic | File | What To Explain |
| --- | --- | --- |
| Main application | `source/src/main.cpp` | Vulkan object setup, camera, lighting, interaction logic |
| Scene layout | `source/assets/scenes/scene.json` | Models, textures, instances, transforms |
| Vertex shader | `source/shaders/tavern_lit.vert` | MVP transform, world position, normal transform, UV pass-through |
| Fragment shader | `source/shaders/tavern_lit.frag` | Texture sampling, ambient, diffuse, specular, point lights |
| CMake setup | `CMakeLists.txt` | Project target, dependencies, shader compilation, asset copy |
| Development logs | `0.Documentation/DevelopmentLog` | Implementation history and verification notes |

## Code Map

### Application Class

Location: `source/src/main.cpp`

Key element:

```cpp
class DungeonTavernNPCApp : public BaseProject
```

Explain:

- The application derives from the course skeleton `BaseProject`.
- Project-specific Vulkan setup and runtime logic are implemented inside this class.
- Framework files are not modified.

### Vertex Format

Location: `source/src/main.cpp`

Key element:

```cpp
struct Vertex {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 UV;
};
```

Explain:

- `pos` is used to position each vertex.
- `norm` is used for lighting.
- `UV` is used for texture lookup.
- This format is registered in `VD.init(...)` and matched with shader input locations `0`, `1`, and `2`.

### Uniform Buffers

Location: `source/src/main.cpp`

Local uniform:

```cpp
struct UniformBufferObject {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
};
```

Explain:

- `mvpMat` transforms vertices to clip space.
- `mMat` transforms positions and normals to world space.

Global uniform:

```cpp
struct GlobalUniformBufferObject
```

Explain:

- Stores camera position, ambient color, directional light, point lights, material values, and light count.
- Updated every frame before drawing.
- Used mainly by the fragment shader for lighting.

### Descriptor Layouts

Location: `localInit()` in `source/src/main.cpp`

Explain:

- `DSLglobal` is descriptor set `0`.
- It contains one uniform buffer for global lighting and camera data.
- `DSLlocal` is descriptor set `1`.
- It contains one local uniform buffer and one texture sampler.
- This separation keeps per-frame data and per-object data easy to understand.

### Pipeline

Location: `localInit()` in `source/src/main.cpp`

Key element:

```cpp
P.init(this, &VD,
       "shaders/tavern_lit.vert.spv",
       "shaders/tavern_lit.frag.spv",
       {&DSLglobal, &DSLlocal});
```

Explain:

- The pipeline connects the vertex format, shaders, render pass, and descriptor layouts.
- All current scene instances use the `BlinnPhong` technique.
- The pipeline uses custom project shaders, not only framework shaders.

### Scene Loading

Location: `source/assets/scenes/scene.json`

Explain:

- `models` define mesh files.
- `textures` define texture files.
- `instances` define placed objects.
- Each instance has model id, texture id, translation, Euler rotation, and scale.
- This makes the tavern layout editable without hardcoding every object transform in C++.

### Command Buffer And Draw Calls

Location: `populateCommandBuffer(...)` in `source/src/main.cpp`

Key element:

```cpp
RP.begin(commandBuffer, currentImage);
SC.populateCommandBuffer(commandBuffer, 0, currentImage);
RP.end(commandBuffer);
```

Explain:

- The render pass is started.
- The scene framework records bindings and draw calls for all instances.
- The render pass is ended.
- The project provides the pipeline, descriptor data, scene data, and uniform updates used by this call.

### Camera

Location: `GameLogic()` in `source/src/main.cpp`

Explain:

- The camera stores position, yaw, and pitch.
- Movement changes the camera position.
- Rotation changes yaw and pitch.
- Pitch is clamped to avoid flipping.
- Room bounds clamp the camera inside the tavern.
- The view matrix is built with:

```cpp
View = glm::lookAt(cameraPos, cameraPos + viewForward, glm::vec3(0.0f, 1.0f, 0.0f));
```

### Projection

Location: `GameLogic()` in `source/src/main.cpp`

Explain:

- The project uses perspective projection.
- `Prj[1][1] *= -1` adapts GLM projection to Vulkan clip coordinates.
- The final matrix is:

```cpp
ViewPrj = Prj * View;
```

### Lighting

Location: `updateUniformBuffer(...)` in `source/src/main.cpp`

Explain:

- The CPU fills `GlobalUniformBufferObject`.
- Six point lights are placed near candles and the chandelier.
- Warm light colors create the tavern atmosphere.
- A small ambient term keeps dark surfaces visible.

Location: `source/shaders/tavern_lit.frag`

Explain:

- The shader samples the albedo texture.
- Ambient light is added first.
- Directional light and point lights use diffuse and specular terms.
- Blinn-Phong uses a halfway vector between view direction and light direction.
- Point lights use distance attenuation.
- Final color uses tone mapping and gamma correction.

### NPC Interaction

Location: `GameLogic()` in `source/src/main.cpp`

Explain:

- NPC positions are fixed world positions.
- The camera is compared against NPC positions using horizontal XZ distance.
- This avoids camera height affecting interaction.
- `E` is edge-triggered with `interactionKeyWasPressed`.
- Number keys are edge-triggered with `drinkChoiceKeyWasPressed`.
- The interaction is a small state machine:
  - not registered
  - registered
  - bartender menu open
  - order completed
  - checkout resets state

### Text Output

Location: `updateInteractionText()` in `source/src/main.cpp`

Explain:

- Interaction text has a stable text id.
- Text updates only when the message changes.
- Empty messages remove the text.
- This prevents stale prompts from staying on screen.

## Shader Explanation

### Vertex Shader

File: `source/shaders/tavern_lit.vert`

Inputs:

- `inPosition`
- `inNormal`
- `inUV`

Outputs:

- `fragPos`
- `fragNormal`
- `fragUV`

Important points:

- `gl_Position` uses the MVP matrix.
- `fragPos` is world-space position for lighting.
- `fragNormal` uses inverse-transpose model matrix so normals remain correct with scaling.
- `fragUV` is passed to the fragment shader for texture sampling.

### Fragment Shader

File: `source/shaders/tavern_lit.frag`

Important points:

- Texture color is sampled from `albedoMap`.
- Texture color is converted from gamma space to linear space.
- Ambient light approximates indirect light.
- Diffuse light uses `max(dot(normal, lightDir), 0.0)`.
- Specular light uses the Blinn-Phong halfway vector.
- Point lights fade with distance.
- Tone mapping prevents overly bright values.
- Gamma correction converts the final color back for display.

## Likely Exam Questions

### Why use position, normal, and UV?

Position is required to place geometry. Normal is required for lighting. UV is required to sample textures. Together they are the minimal practical vertex format for textured lit models.

### Why split global and local descriptor sets?

Global data is shared by the whole frame, such as camera position and lights. Local data changes per object, such as model matrices and textures. Splitting them makes the rendering data model easier to understand.

### Why use inverse-transpose for normals?

When a model has non-uniform scaling, directly multiplying normals by the model matrix can make them incorrect. The inverse-transpose matrix preserves the correct normal direction for lighting.

### Why use horizontal distance for NPC interaction?

The interaction should depend on the player being near the NPC on the floor plane. Camera height should not prevent interaction when the player looks up, crouches, or changes vertical position.

### Why edge-trigger keys?

Without edge detection, holding `E` or a number key would trigger the action every frame. Edge detection makes one physical press produce one logical action.

### Why not use PBR?

The project target is a course-level Vulkan scene with explainable shaders. Blinn-Phong is simpler, stable, and enough to demonstrate texture mapping, normals, uniforms, point lights, and shader-controlled lighting.

### What is custom in the shader?

Both project shaders are custom:

- the vertex shader transforms position and normals and passes UVs
- the fragment shader implements albedo sampling, ambient light, directional light, point lights, diffuse/specular shading, attenuation, tone mapping, and gamma correction

## Final Checklist Before Exam

- Build and run from CLion.
- Confirm the correct window title appears.
- Confirm scene assets load.
- Confirm camera movement works.
- Confirm camera stays inside the room.
- Confirm greeter registration works.
- Confirm bartender menu works.
- Confirm interaction text disappears when leaving NPC range.
- Be ready to open:
  - `source/src/main.cpp`
  - `source/shaders/tavern_lit.vert`
  - `source/shaders/tavern_lit.frag`
  - `source/assets/scenes/scene.json`
  - `0.Documentation/DevelopmentLog/2026-06-29-037-document-shader-pipeline.md`

## Short Speaking Script

This project uses the provided Vulkan skeleton but keeps project logic in `source/src`, shaders, assets, and documentation. The scene is defined in `scene.json`, where each instance has a model, texture, translation, rotation, and scale. The C++ code defines a position-normal-UV vertex format, two descriptor layouts, and one Blinn-Phong pipeline. Each frame, it updates camera matrices, lighting uniforms, and per-object MVP matrices. The command buffer starts the render pass and lets the scene framework draw all instances with the configured pipeline. The custom shaders sample textures and apply ambient, diffuse, specular, and point light attenuation. The interaction system is intentionally simple: it checks horizontal distance from the camera to fixed NPC positions and uses edge-triggered keys to update a small dialogue state machine.
