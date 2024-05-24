cmake_minimum_required(VERSION 3.11) # The Minimum Version Of Cmake
project(GettingStarted VERSION 0.1.1) # The Project Name And
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED) # Find Package In You PC
find_package(GLEW REQUIRED) # Find Package In You PC

add_executable(GettingStarted src/main.cpp) # The File For Run and Debug

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/textures/M.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/textures/P.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# The Targets For Link The Libraries
target_link_libraries(GettingStarted
    glfw
    OpenGL::GL
    GLEW::GLEW
    )
