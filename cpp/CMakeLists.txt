cmake_minimum_required(VERSION 3.5)
project(OpenGLMemoryTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLEW
find_package(GLEW REQUIRED)

# Find GLFW
find_package(glfw3 3.3 REQUIRED)

# Include directories
include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

# Add executable
add_executable(OpenGLMemoryTest main.cpp)

# Link libraries
target_link_libraries(OpenGLMemoryTest
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glfw
)
