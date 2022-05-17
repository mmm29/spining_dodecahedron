cmake_minimum_required(VERSION 3.19)
project(spinning_dodecahedron)

set(CMAKE_CXX_STANDARD 17)

# SFML
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/lib/SFML")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")

find_package(SFML COMPONENTS window REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# ImGui
include_directories(lib/imgui)

file(GLOB IMGUI_SOURCES lib/imgui/*.cpp)

# Source files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Executable
add_executable(spinning_dodecahedron WIN32 ${SOURCE_FILES} ${IMGUI_SOURCES})

# Libraries
target_link_libraries(spinning_dodecahedron PUBLIC sfml-window sfml-graphics opengl32)