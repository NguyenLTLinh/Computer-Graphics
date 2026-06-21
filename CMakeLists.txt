cmake_minimum_required(VERSION 3.20)
project(TheSunkenCapital LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(TheSunkenCapital
    src/main.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/SceneFactory.cpp
)

target_include_directories(TheSunkenCapital PRIVATE include)
target_include_directories(TheSunkenCapital PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(TheSunkenCapital PRIVATE glfw GLEW::GLEW glm::glm)

if (MSVC)
    target_compile_options(TheSunkenCapital PRIVATE /W4 /permissive-)
else()
    target_compile_options(TheSunkenCapital PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(TARGET TheSunkenCapital POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:TheSunkenCapital>/assets
)
