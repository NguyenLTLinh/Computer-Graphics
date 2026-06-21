# Manual Visual Studio Setup

If you are not using CMake, create a C++ Console App project in Visual Studio and add the following paths and linker settings.

## Include Directories

```text
$(ProjectDir)include
$(ProjectDir)libs\GLFW\include
$(ProjectDir)libs\GLEW\include
$(ProjectDir)libs\GLM
$(ProjectDir)libs\stb_image
```

## Library Directories

```text
$(ProjectDir)libs\GLFW\lib
$(ProjectDir)libs\GLEW\lib
```

## Additional Dependencies

```text
opengl32.lib
glfw3.lib
glew32s.lib
```

## Preprocessor Definitions

```text
GLEW_STATIC
_CRT_SECURE_NO_WARNINGS
```

## Runtime Assets

Set the working directory to `$(ProjectDir)` so the application can load:

```text
assets/shaders/*.vert
assets/shaders/*.frag
assets/textures/*.png
```
