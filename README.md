# Computer-Graphics: The Sunken Capital

A C++17/OpenGL Core Profile 3.3 project framework for **The Sunken Capital**, an ancient city submerged beneath the ocean. The project focuses on object-oriented architecture, indexed mesh rendering, free-flight camera navigation, Phong lighting with distance attenuation, mipmapped texture mapping, and underwater animation.

## Project Structure

```text
TheSunkenCapital/
  src/                 C++ source files
  include/             Header files
  libs/                Library placement notes for GLFW, GLEW, GLM, and stb_image
  assets/shaders/      GLSL .vert and .frag shader files
  assets/textures/     Ancient stone, moss, and coral-style texture assets
  CMakeLists.txt
  VisualStudio_Setup.md
```

## Requirements

- CMake 3.20+
- A C++17 compiler
- OpenGL 3.3+
- Dependencies:
  - GLFW
  - GLEW
  - GLM
  - stb_image

Example using vcpkg:

```powershell
vcpkg install glfw3 glew glm stb
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
.\build\Release\TheSunkenCapital.exe
```

For a manual Visual Studio setup, see `VisualStudio_Setup.md`.

## Controls

- `W`, `A`, `S`, `D`: move through the scene
- `Space`: move upward
- `Left Ctrl`: move downward
- Mouse: rotate the camera using yaw and pitch
- `Esc`: exit

## Architecture

- `Mesh`: manages VAO/VBO/IBO resources with RAII and renders with `glDrawElements` to avoid duplicated vertex data.
- `Shader`: loads, compiles, links GLSL 330 programs, and uploads uniforms.
- `Camera`: builds the view matrix with `glm::lookAt`, supports WASD/mouse navigation, and uses delta time for hardware-independent movement.
- `Texture`: loads images with `stb_image`, generates mipmaps, uses trilinear sampling, and enables anisotropic filtering when supported.
- `SceneFactory`: creates sample geometry for stone blocks, ancient columns, seaweed blades, and bubbles.

## Shaders

- `assets/shaders/ruin.vert` and `assets/shaders/ruin.frag`: model/view/projection transforms, normal matrix handling, Phong reflection, point-light attenuation, and deep-water fog.
- `assets/shaders/seaweed.vert` and `assets/shaders/seaweed.frag`: seaweed animation using arcs, follow-through, and overlap through height-based phase-shifted sine waves.
- `assets/shaders/bubble.vert` and `assets/shaders/bubble.frag`: simple bubble billboard quads with rim alpha and soft floating motion.

## Extension Ideas

- Add normal mapping and tangent-space lighting for deeper stone surface detail.
- Use instanced rendering for large groups of columns, seaweed, or bubbles.
- Add framebuffer post-processing such as caustics, subtle bloom, volumetric light shafts, and underwater color grading.
