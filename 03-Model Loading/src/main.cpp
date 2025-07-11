#include "camera.hpp"
#include "window.hpp"
#include "model.hpp"
#include "ui.hpp"

GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

int main() {
  Create_Window create_window;
  GLFWwindow *window = create_window.create_window("(---Chapan Harder Window---)");

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);

  // Load shader
  Shader ourShader("sh.vs", "sh.fs");

  // Load model
  Model ourModel((char *)"models/Tower.obj");

  // create context
  // -----------------------------------------
  struct nk_context *ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
  struct nk_colorf bg_color = {0.07f, 0.07f, 0.07f, 1.0f};
  struct nk_font_atlas *defult_ui_font;
  nk_glfw3_font_stash_begin(&defult_ui_font);
  nk_glfw3_font_stash_end();

  bool bg_color_option = nk_false;
  bool wireframe_mode = nk_false;
  // -----------------------------------------

  // Initialize camera with proper starting position
  camera.Position = glm::vec3(0.0f, 0.3f, 1.0f);  // Back a bit from the model

  while (!glfwWindowShouldClose(window)) {
    // Timing
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Start new frame
    nk_glfw3_new_frame();
    
    // Input handling
    create_window.window_action(window, deltaTime);

    // Set polygon mode before drawing
    if (wireframe_mode) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Clear buffers
    glClearColor(bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 3D Rendering
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f));

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)create_window.WIDTH / (float)create_window.HEIGHT, 0.1f, 100.0f);

    ourShader.use();
    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);
    ourShader.setVec3("lightPos", glm::vec3(2.0f, 2.0f, 2.0f));
    ourShader.setVec3("viewPos", camera.Position);
    ourModel.Draw(ourShader);

    // UI Rendering
    ui_app(ctx, &bg_color, &bg_color_option, &wireframe_mode);
    glDisable(GL_DEPTH_TEST);
    nk_glfw3_render(NK_ANTI_ALIASING_ON);
    glEnable(GL_DEPTH_TEST);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();

  return 0;
}
