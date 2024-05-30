cmake_minimum_required(VERSION 3.11)
Project(Lighting VERSION 0.1.0)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(Lighting src/main.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/Resources/Images/Blue\ ground\ test.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Lighting
    glfw
    OpenGL::GL
    GLEW::GLEW
    )