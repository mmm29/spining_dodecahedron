cmake_minimum_required(VERSION 3.19)
project(spinning_dodecahedron)

set(CMAKE_CXX_STANDARD 17)

# SFML
find_package(SFML COMPONENTS window system graphics REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# ImGui
include_directories(lib/imgui)

file(GLOB IMGUI_SOURCES lib/imgui/*.cpp)

# ImGui SFML
set(BUILD_SHARED_LIBS TRUE)
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/lib/imgui")

add_subdirectory(lib/imgui-sfml)

# Source files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Executable
add_executable(spinning_dodecahedron WIN32 ${SOURCE_FILES} ${IMGUI_SOURCES})

# Libraries
target_link_libraries(spinning_dodecahedron PUBLIC
                        sfml-window
                        sfml-graphics
                        ImGui-SFML
                        ${OPENGL_LIBRARIES})