# Cau Hinh Visual Studio Thu Cong

Neu khong dung CMake, tao project Console App C++ trong Visual Studio va them cac duong dan sau.

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

Dat working directory ve `$(ProjectDir)` de chuong trinh doc duoc:

```text
assets/shaders/*.vert
assets/shaders/*.frag
assets/textures/*.png
```
