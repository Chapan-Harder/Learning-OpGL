#ifndef WINDOW_H
#define WINDOW_H
#include <found_libs.hpp>
#include "camera.hpp"

Camera camera;

// Mouse Info And Camera
GLboolean firstMouse = true;
GLfloat lastX = 400.0;
GLfloat lastY = 300.0;

class Create_Window {
public:
  // Screen Size Or Screen Resolution
  const GLuint WIDTH = 720;
  const GLuint HEIGHT = 480;

  // -------------------------------------------------------------------------
  // Create The Window
  GLFWwindow *create_window(const char *title);
  static void framebuffer_size_callback(GLFWwindow *window, GLint WIDTH, GLint HEIGHT);
  static void mouse_movement(GLFWwindow *window, GLdouble xpos, GLdouble ypos);
  static void scroll_callback(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset);
  static void window_action(GLFWwindow *window, GLfloat &deltaTime);
  // -------------------------------------------------------------------------
};

GLFWwindow *Create_Window::create_window(const char *title) {
  GLuint screenWidth, screenHeight;

  // Initialize GLFW
  glfwInit();

  // Configure GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // For Mac

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);

  if (window == nullptr) {
    std::cout << "\tERROR::Failed to create window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  if (window == nullptr) {
    return nullptr;
  }
  glfwSetCursorPosCallback(window, mouse_movement);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  if (GLEW_OK != glewInit()) {
    std::cout << "\tFailed to initionalize GLEW" << std::endl;
    return nullptr;
  }
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

void Create_Window::framebuffer_size_callback(GLFWwindow *window, GLint WIDTH, GLint HEIGHT) {
  glViewport(0, 0, WIDTH, HEIGHT);
}

void Create_Window::mouse_movement(GLFWwindow *window, GLdouble xpos, GLdouble ypos) {
  if (firstMouse){
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void Create_Window::scroll_callback(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset) {
  camera.ProcessMouseScroll(static_cast<GLfloat>(yoffset));
}

void Create_Window::window_action(GLFWwindow *window, GLfloat &deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
#endif
