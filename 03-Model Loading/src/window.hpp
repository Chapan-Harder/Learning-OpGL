#ifndef WINDOW_H
#define WINDOW_H
// #include <GLFW/glfw3.h>
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
  static const GLuint WIDTH = 936;
  static const GLuint HEIGHT = 624;

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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

void Create_Window::mouse_movement(GLFWwindow* window, double xpos, double ypos) {
  static float lastX = WIDTH / 2.0f;
  static float lastY = HEIGHT / 2.0f;

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
    return;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  // Only process movement if cursor is disabled (right mouse held)
  if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
    camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void Create_Window::scroll_callback(GLFWwindow *window, GLdouble xoffset, GLdouble yoffset) {
  camera.ProcessMouseScroll(static_cast<GLfloat>(yoffset));
}

void Create_Window::window_action(GLFWwindow *window, GLfloat &deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  
  // Camera movement controls
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    camera.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    camera.ProcessKeyboard(DOWN, deltaTime);

  // Mouse toggle control
  static bool rightMousePressed = false;
  static bool cursorDisabled = false;
  
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !rightMousePressed) {
    cursorDisabled = !cursorDisabled; // Toggle state
    if (cursorDisabled) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      firstMouse = true;
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    rightMousePressed = true;
  } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
    rightMousePressed = false;
  }
}
#endif
