cmake_minimum_required(VERSION 3.30.3)
project(Advanced_OpenGL.cm VERSION 0.2.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# C standard settings (kept for any C dependencies)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build settings
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -pedantic)

# Set the policy to suppress the warning
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

# Find the required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

# Include directories
include_directories(${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${assimp_INCLUDE_DIRS})

# Add the executable
add_executable(Advanced_OpenGL.cm src/main.cpp)

# Link the libraries
target_link_libraries(Advanced_OpenGL.cm
    glfw
    OpenGL::GL
    GLEW::GLEW
    assimp::assimp
    ${EXTRA_LIBS}
)

# Copy shader files and others file to the build directory
file(COPY shaders/ DESTINATION ${CMAKE_BINARY_DIR})
file(COPY src/resources/ DESTINATION ${CMAKE_BINARY_DIR})

#Includes the folder
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/src/resources/)
