#include "window.hpp"
#include "model.hpp"

GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

int main() {
  Create_Window create_window;
  GLFWwindow *window = create_window.create_window("(---Chapan Harder Window---)");

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);

  // Load shader
  // Shader ourShader("path_to_your_vertex_shader.vs", "path_to_your_fragment_shader.fs");
  Shader ourShader("sh.vs", "sh.fs");

  // Load model
  Model ourModel((char *)"models/Tower.obj");

  while (!glfwWindowShouldClose(window)) {
    GLfloat currenFrame = glfwGetTime();
    deltaTime = currenFrame - lastFrame;
    lastFrame = currenFrame;
    create_window.window_action(window, deltaTime);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // In your render loop:
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Center model
    model = glm::scale(model, glm::vec3(0.1f)); // Adjust scale if needed

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)create_window.WIDTH / (float)create_window.HEIGHT, 0.1f, 100.0f);

    // Set light position (should be somewhere visible)

    glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
    ourShader.use();
    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);
    ourShader.setVec3("lightPos", lightPos);
    ourShader.setVec3("viewPos", camera.Position);

    // Reset camera position (add this before main loop)
    camera.Position = glm::vec3(0.0f, 0.5f, 2.0f);

    // Draw model
    ourModel.Draw(ourShader);

    create_window.window_action(window, deltaTime);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();

  return 0;
}
