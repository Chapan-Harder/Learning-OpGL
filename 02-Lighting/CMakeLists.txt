cmake_minimum_required(VERSION 3.10)
project(Lighting VERSION 0.1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the policy to suppress the warning
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

# Find the required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

# Add the executable
add_executable(Lighting src/main.cpp)

# Link the libraries
target_link_libraries(Lighting
    glfw
    OpenGL::GL
    GLEW::GLEW
)

# Copy shader files and others file to the build directory
file(COPY shaders/ DESTINATION ${CMAKE_BINARY_DIR})
file(COPY src/Resources/Images/ DESTINATION ${CMAKE_BINARY_DIR})

#Includes the folder
include_directories(${PROJECT_SOURCE_DIR}/include/)

