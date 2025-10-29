#include "window.hpp"
#include "style.c"
#include "grid.hpp"
#include "model.hpp"
#include "ui.cpp"

GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

int main() {
  Create_Window create_window;
  GLFWwindow * window = create_window.create_window("04-Advanced OpenGL");

  // Shader section
  Shader chapanBox("sh.vs", "sh.fs");
  Shader gridShader("grid.vs", "grid.fs");

  // Load the models
  Model loadChapanBox((char *)"meshes/ChapanBox.gltf");
  Grid grid;

  // Nuklear UI init with glfw3
  // --------------------------------------------------------------------------
  struct nk_context *ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
  set_style(ctx, THEME_CATPPUCCIN_MACCHIATO);
  struct nk_font_atlas *defult_ui_font;
  {
    nk_glfw3_font_stash_begin(&defult_ui_font);
    struct nk_font *audiowide = nk_font_atlas_add_from_file(defult_ui_font, "fonts/Audiowide-Regular.ttf", 15, 0);
    nk_glfw3_font_stash_end();
    nk_style_set_font(ctx, &audiowide->handle);
  }
  struct nk_colorf bg_color = {0.05f, 0.05f, 0.1f, 1.0f};
  struct nk_colorf lt_color = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat lt_power = 1.0f;
  // --------------------------------------------------------------------------

  // Initialize camera with proper starting position
  camera.Position = glm::vec3(0.0f, 0.3f, 1.0f);  // Back a bit from the model

  while (!glfwWindowShouldClose(window)) {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Start new frame
    nk_glfw3_new_frame();

    // Window action
    create_window.window_action(window, deltaTime);

    // Clear buffers
    glClearColor(bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Ui sections
    ui_setting(ctx, &bg_color, &lt_color, &lt_power);

    // 3D rendering
    // ------------------------------------------------------------------------
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)create_window.WIDTH / (float)create_window.HEIGHT, 0.1f, 100.0f);
    glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::vec3 lightColor = glm::vec3(lt_color.r, lt_color.g, lt_color.b);
    GLfloat lightPower = lt_power;
    glm::vec3 viewPos = glm::vec3(camera.Position);
    glm::mat4 gridModel = glm::mat4(1.0f);
    glm::mat4 chapanBoxModel = glm::mat4(1.0f);

    // Grid section
    gridShader.use();
    gridShader.setMat4("view", view);
    gridShader.setMat4("projection", projection);
    gridModel = glm::scale(gridModel, glm::vec3(0.1f));
    gridShader.setMat4("model", gridModel);
    gridShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // Gray grid lines
    grid.Draw(gridShader);

    // Box section
    chapanBox.use();
    chapanBox.setMat4("view", view);
    chapanBox.setMat4("projection", projection);
    chapanBox.setVec3("lightPos", lightPos);
    chapanBox.setVec3("lightColor", lightColor);
    chapanBox.setFloat("lightPower", lightPower);
    chapanBox.setVec3("viewPos", viewPos);
    chapanBoxModel = glm::scale(chapanBoxModel, glm::vec3(0.1f));
    chapanBoxModel = glm::translate(chapanBoxModel, glm::vec3(0.0f, 1.0f, 0.0f));
    chapanBox.setMat4("model", chapanBoxModel);
    loadChapanBox.Draw(chapanBox);
    // ------------------------------------------------------------------------

    // Swap buffers
    nk_glfw3_render(NK_ANTI_ALIASING_ON);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  nk_glfw3_shutdown();
  glfwTerminate();

  return 0;
}
