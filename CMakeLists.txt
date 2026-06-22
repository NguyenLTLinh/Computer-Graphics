cmake_minimum_required(VERSION 3.16)
project(TheSunkenCapital VERSION 1.0.0 LANGUAGES CXX)

# ============================================================
# Cài đặt C++17 Standard
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# Tìm các thư viện bắt buộc
# ============================================================
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

# GLM là header-only, chỉ cần include path
find_package(glm REQUIRED)

# ============================================================
# Thu thập tất cả source files
# ============================================================
set(SOURCES
    src/main.cpp
    src/Shader.cpp
    src/Mesh.cpp
    src/Camera.cpp
    src/Texture.cpp
    src/Renderer.cpp
    src/SceneBuilder.cpp
)

set(HEADERS
    include/Shader.h
    include/Mesh.h
    include/Camera.h
    include/Texture.h
    include/Renderer.h
    include/SceneBuilder.h
    include/stb_image.h
    include/Common.h
)

# ============================================================
# Tạo executable
# ============================================================
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# ============================================================
# Include directories
# ============================================================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

# ============================================================
# Liên kết thư viện
# ============================================================
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)

# ============================================================
# Copy assets vào thư mục build để runtime có thể đọc
# ============================================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets to build directory..."
)

# ============================================================
# Compiler flags tối ưu hóa
# ============================================================
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /O2)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
endif()

# Thông báo cấu hình
message(STATUS "=== The Sunken Capital Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "OpenGL: ${OPENGL_VERSION_STRING}")
