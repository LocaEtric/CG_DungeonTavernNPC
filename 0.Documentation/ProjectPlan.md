# Dungeon Tavern NPC Project Plan

## Project Goal

Build a small but complete real-time 3D tavern scene in Vulkan. The user navigates in first person, approaches an NPC, and presses a key to trigger a simple interaction.

The project should demonstrate the core Computer Graphics course requirements: mesh loading, textures, camera transforms, shaders, lighting, Vulkan pipeline setup, uniform updates, command buffer draw calls, and interactive application logic.

## Final Feature Target

1. A 3D tavern or dungeon tavern environment.
2. First-person navigation with keyboard movement and mouse or keyboard rotation.
3. At least one NPC, such as an innkeeper behind the counter.
4. Distance-based interaction detection between the player camera and the NPC.
5. On-screen prompt when near the NPC: `Press E to interact`.
6. Simple dialogue or state change after pressing `E`, for example: `Welcome to my tavern!`.
7. Textured models and basic lighting.
8. Custom vertex and fragment shaders that the team can explain.
9. Clean code and documentation suitable for exam review.

## Development Order

### 1. Make The Skeleton Run

- Create the missing `source/assets/scenes/scene.json`.
- Load a minimal scene first, such as `Plane.gltf` with `toChange.jpg`.
- Confirm the Vulkan window opens and renders a visible textured object.

### 2. Rename The Project

- Rename the CMake project from the skeleton placeholder to `DungeonTavernNPC`.
- Rename the main application class and window title.
- Keep the change small and easy to review.

### 3. Improve The Vertex Format

- Replace the temporary `pos + UV` vertex format with a format that also includes normals.
- Update the vertex shader to pass transformed normals.
- Update the fragment shader to use normals for lighting instead of deriving normals from screen-space derivatives.

### 4. Import Tavern Assets

- Select a small set of assets from the provided course asset folder:
  - tavern or dungeon room
  - bar counter
  - table
  - chair
  - barrel
  - NPC or character model
- Copy only the assets actually needed for the project.
- Record asset choices in a development log entry.

### 5. Compose The Scene

- Use `scene.json` instances to place models.
- Use translation, rotation, and scale matrices for tavern layout.
- Build a simple readable scene: room/floor, counter, tables, chairs, barrels, NPC.

### 6. Implement First-Person Camera

- Store camera position, yaw, and pitch in application state.
- Use `W`, `A`, `S`, `D` for movement.
- Use mouse movement or keyboard keys for view rotation.
- Compute the view matrix with a look-in-direction or equivalent `lookAt` formulation.

### 7. Add NPC Interaction

- Store the NPC world position.
- Each frame, compute the distance between the camera and the NPC.
- Show interaction text only when the player is close enough.
- Use key debounce for `E` so one press triggers one interaction change.

### 8. Finalize Lighting And Shaders

- Implement texture sampling, ambient lighting, diffuse lighting, and a simple specular term.
- Keep the shader understandable and easy to explain.
- Avoid unnecessary PBR complexity unless the base project is already stable.

### 9. Test And Prepare Presentation

- Verify the application builds from a clean build directory.
- Verify models, textures, camera movement, lighting, and NPC interaction.
- Prepare a short demonstration path:
  1. launch the app
  2. navigate inside the tavern
  3. show textured objects and lighting
  4. approach the NPC
  5. show the prompt
  6. press `E`
  7. show the dialogue
  8. explain the relevant code sections

## Deliberately Out Of Scope

- Quest system
- Shop system
- Combat
- Inventory
- Complex AI
- Multiplayer
- Complex animation
- Full physics engine
- Full PBR renderer

These features are not needed for the selected topic and would make the project harder to finish and explain.

