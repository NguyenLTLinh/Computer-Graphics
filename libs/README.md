# libs

This directory is intended for manual Visual Studio configuration when you are not using vcpkg or CMake package discovery.

Recommended layout:

```text
libs/
  GLFW/
    include/GLFW/glfw3.h
    lib/glfw3.lib
  GLEW/
    include/GL/glew.h
    lib/glew32s.lib
  GLM/
    glm/...
  stb_image/
    stb_image.h
```

If you use vcpkg, you do not need to copy binary dependencies into this directory. Install the dependencies with:

```powershell
vcpkg install glfw3 glew glm stb
```

Then configure CMake with the vcpkg `CMAKE_TOOLCHAIN_FILE`.
