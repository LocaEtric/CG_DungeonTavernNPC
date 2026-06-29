# Dungeon Tavern NPC Project Plan

## Project Goal

Build a small but complete real-time 3D dungeon tavern scene in Vulkan. The user navigates in first person, approaches NPC characters, and presses keys to trigger simple interactions.

The project demonstrates the core Computer Graphics course requirements: mesh loading, textures, camera transforms, custom shaders, lighting, Vulkan pipeline setup, uniform updates, command buffer draw calls, and interactive application logic.

## Final Implemented Feature Set

- A closed dungeon tavern environment with floor, walls, ceiling, front door, bar counter, tables, chairs, barrels, cupboards, lights, and dungeon-themed decoration.
- Final prop pass with drink items on the bar, potion and globe on the right table, a cage and torture chair in the right storage area, a crow on the door greeter, a bat under the chandelier, a hanging skeleton, and two pumpkins.
- First-person navigation using keyboard movement and keyboard view rotation.
- Camera height control with `R` / `F`.
- Optional room boundary logic, currently enabled, so the presentation camera stays inside the tavern.
- Two NPC interaction points:
  - `npc_door_greeter`
  - `npc_bartender`
- Distance-based interaction detection using horizontal XZ distance from the camera to each NPC.
- On-screen interaction text using `TextMaker`.
- Edge-triggered `E` interaction input, so holding the key does not trigger repeated actions.
- Greeter registration and checkout flow.
- Bartender order flow with drink choices using number keys `1`, `2`, and `3`.
- Textured models loaded from `source/assets/scenes/scene.json`.
- Custom vertex and fragment shaders:
  - `source/shaders/tavern_lit.vert`
  - `source/shaders/tavern_lit.frag`
- Vertex format with position, normal, and UV.
- Global uniform buffer for camera position, ambient light, directional light, point lights, material parameters, and light count.
- Local uniform buffer for per-instance model and MVP matrices.
- Blinn-Phong style lighting with ambient, diffuse, specular, point light attenuation, tone mapping, and gamma correction.

## Current Project Structure

- `source/src/main.cpp`
  - Main application class.
  - Descriptor layouts.
  - Vertex format.
  - Render pass and graphics pipeline setup.
  - Scene loading.
  - Command buffer population.
  - Camera, lighting, and NPC interaction logic.
- `source/shaders/tavern_lit.vert`
  - Transforms vertices by MVP matrix.
  - Computes world-space fragment position.
  - Transforms normals with inverse-transpose model matrix.
  - Passes UV coordinates to the fragment shader.
- `source/shaders/tavern_lit.frag`
  - Samples the albedo texture.
  - Applies ambient light, directional light, and multiple point lights.
  - Uses a simple Blinn-Phong specular term.
- `source/assets/scenes/scene.json`
  - Defines model assets, texture assets, and all scene instances.
  - Stores tavern layout transforms in translation, Euler rotation, and scale form.
- `source/assets/models`
  - Contains selected course-provided `.MGCG` assets and a small number of project OBJ panels.
- `source/assets/textures`
  - Contains project textures used by the tavern shell and selected assets.
- `0.Documentation/DevelopmentLog`
  - Records each meaningful implementation step.

## Completed Milestones

### 1. Skeleton Bootstrap

- Created a working asset and scene loading path.
- Added missing scene file.
- Confirmed the project follows the provided Vulkan course skeleton.
- Kept framework files and `source/include` unchanged.

### 2. Project Rename

- Renamed the CMake project to `DungeonTavernNPC`.
- Renamed the main application class.
- Updated the window title.

### 3. Vertex Format And Shader Foundation

- Replaced the temporary position + UV vertex format with position + normal + UV.
- Updated the vertex shader to pass world-space position, normal, and UV.
- Updated the fragment shader to use normals for lighting.

### 4. Asset Selection And Scene Composition

- Selected tavern and dungeon assets from the course-provided `.MGCG` asset set.
- Pruned unused model assets.
- Built a readable tavern layout with bar area, guest area, entrance area, storage area, candles, and chandelier.
- Added two NPC placeholders as scene instances.

### 5. First-Person Camera

- Implemented camera position, yaw, and pitch state.
- Implemented movement and rotation.
- Added height movement.
- Added room limits for presentation stability.
- Built the view matrix using a look-in-direction formulation.

### 6. NPC Interaction

- Implemented horizontal distance checks against fixed NPC world positions.
- Added greeter registration and checkout.
- Added bartender drink menu.
- Added edge-triggered input for `E` and number keys.
- Decoupled interaction text updates from the 1 Hz FPS update.
- Cleared interaction text when the user moves away from NPCs.

### 7. Lighting And Shader Polish

- Added global lighting uniform data.
- Added warm point lights at wall candles and chandelier.
- Added ambient and weak directional lighting.
- Implemented Blinn-Phong diffuse and specular terms in the fragment shader.
- Added simple attenuation for point lights.
- Added tone mapping and gamma correction.

### 8. Feature Verification

- The main runtime features have been manually verified by the team:
  - scene rendering
  - model placement
  - texture mapping
  - first-person movement
  - camera room limits
  - NPC proximity prompts
  - greeter registration and checkout
  - bartender order menu
  - interaction text cleanup
  - shader lighting behavior

## Final Presentation Path

1. Launch the application.
2. Show the closed dungeon tavern environment.
3. Move around with the first-person camera.
4. Point out textured assets, walls, furniture, candles, and chandelier.
5. Approach the door greeter.
6. Press `E` to register.
7. Walk to the bartender.
8. Press `E` to open the drink menu.
9. Press `1`, `2`, or `3` to choose a drink.
10. Walk away to show that interaction text disappears.
11. Return to the greeter and press `E` to check out.
12. Explain the code sections for scene loading, vertex format, descriptors, pipeline, command buffer, camera transform, lighting, and interaction logic.

## Remaining Optional Polish

- Visually inspect the final prop heights and rotations in CLion, then adjust individual transforms only if a model still floats or clips.
- Prepare screenshots for the final report or slides.
- Keep the current interaction system simple; do not add quests, inventory, combat, or animation unless required.

## Deliberately Out Of Scope

- Quest system
- Shop or economy system
- Combat
- Inventory
- Complex AI
- Multiplayer
- Complex animation
- Full physics engine
- Full PBR renderer

These features are not needed for the selected topic and would make the project harder to finish, verify, and explain during the exam.
