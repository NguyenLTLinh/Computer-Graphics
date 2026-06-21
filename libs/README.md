# libs

Thu muc nay danh cho cach cau hinh Visual Studio thu cong khi khong dung vcpkg/CMake.

Khuyen nghi bo tri:

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

Neu dung vcpkg, khong can copy binary vao day. Hay cai dependencies:

```powershell
vcpkg install glfw3 glew glm stb
```

Sau do cau hinh CMake voi `CMAKE_TOOLCHAIN_FILE` cua vcpkg.
