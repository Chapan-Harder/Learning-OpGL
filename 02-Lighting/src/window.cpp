#pragma once
#include "includes.h"

void mouse_movement(GLFWwindow *window, GLdouble xpos, GLdouble ypos);

// Screen Size Or Screen Resolution
const GLint WIDTH = 720, HEIGHT = 405;
// const GLint WIDTH = 1920, HEIGHT = 1080;

// Camera
glm::vec3 cameraPose = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

// Mouse Info
GLboolean firstMouse = true;
GLfloat lastX = 400.0, lastY = 300.0;
GLfloat yaw = -90.0f, pitch = 0.0f, fov = 45.0f;

void framebuffer_size_callback(GLFWwindow *window, GLint WIDTH, GLint HEIGHT){
    glViewport(0, 0, WIDTH, HEIGHT);
}

GLFWwindow * createMyWindow(const char *title){
    GLint screenWidth, screenHeight;

    glfwInit(); // Initialize GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // FOR MAC_OS

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);

    if (window == NULL){
        std::cout << "\tERROR::Failed to create window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if (window == NULL){
        return NULL;
    }
    glfwSetCursorPosCallback(window, mouse_movement);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()){
        std::cout << "\tFailed to initionalize GLEW" << std::endl;
        return NULL;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void camera_and_window_action(GLFWwindow *window, GLfloat &deltaTime){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    GLfloat cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed = 5.0f * deltaTime;
    else
        cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPose += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPose -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPose -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPose += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPose += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPose -= cameraSpeed * cameraUp;
}

void mouse_movement(GLFWwindow *window, GLdouble xpos, GLdouble ypos){
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat mouse_sensitivity = 0.05f;
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}