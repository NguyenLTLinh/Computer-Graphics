# Computer-Graphics: The Sunken Capital

Khung dự án C++17/OpenGL Core Profile 3.3 cho cảnh "The Sunken Capital" - thành phố cổ bị chìm dưới đại dương. Project tập trung vào kiến trúc OOP, indexed mesh rendering, camera tự do, Phong lighting có attenuation, texture mipmaps và animation dưới nước.

## Cấu Trúc Thư Mục

```text
TheSunkenCapital/
  src/                 Chứa các file .cpp
  include/             Chứa các file .h
  libs/                Chứa hoặc mô tả cách đặt GLFW, GLEW, GLM, stb_image
  assets/shaders/      Chứa file .vert và .frag shader
  assets/textures/     Chứa texture đá cổ, rêu phong, san hô
  CMakeLists.txt
  VisualStudio_Setup.md
```

## Yêu Cầu

- CMake 3.20+
- Trình biên dịch C++17
- OpenGL 3.3+
- Dependencies:
  - GLFW
  - GLEW
  - GLM
  - stb_image

Ví dụ với vcpkg:

```powershell
vcpkg install glfw3 glew glm stb
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
.\build\Release\TheSunkenCapital.exe
```

Nếu cấu hình Visual Studio thủ công, xem `VisualStudio_Setup.md`.

## Điều Khiển

- `W`, `A`, `S`, `D`: di chuyển tự do trong cảnh
- `Space`: nổi lên
- `Left Ctrl`: lặn xuống
- Chuột: xoay camera bằng yaw/pitch
- `Esc`: thoát

## Kiến Trúc

- `Mesh`: quản lý VAO/VBO/IBO theo RAII, dùng `glDrawElements` để tránh lặp dữ liệu đỉnh.
- `Shader`: nạp, biên dịch, link GLSL 330 và set uniform.
- `Camera`: view matrix bằng `glm::lookAt`, điều hướng WASD/chuột, tốc độ độc lập phần cứng nhờ delta time.
- `Texture`: load ảnh bằng `stb_image`, tạo mipmaps, sampling trilinear và anisotropic filtering nếu GPU hỗ trợ.
- `SceneFactory`: tạo hình học mẫu cho khối đá, cột cổ, rong biển và bọt khí.

## Shader

- `assets/shaders/ruin.vert` và `assets/shaders/ruin.frag`: model/view/projection, normal matrix, Phong reflection, point light attenuation, fog màu xanh thẳm.
- `assets/shaders/seaweed.vert` và `assets/shaders/seaweed.frag`: chuyển động rong biển theo arcs, follow-through và overlap bằng sóng lệch pha theo chiều cao vertex.
- `assets/shaders/bubble.vert` và `assets/shaders/bubble.frag`: bọt khí dạng billboard quad đơn giản, alpha rim và dao động nổi.

## Gợi Ý Mở Rộng

- Thêm normal map/tangent space để bề mặt đá có chiều sâu hơn.
- Dùng instancing cho nhiều cột, rong biển hoặc bọt khí nếu scene lớn.
- Thêm framebuffer post-processing: caustics, bloom nhẹ, volumetric shafts và color grading xanh lục.
