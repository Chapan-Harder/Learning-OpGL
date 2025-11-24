#include "found_libs.hpp"
#include "shader.cpp"
#include "window.hpp"
#include "style.c"
#include "grid.hpp"
#include "model.hpp"
#include "ui.cpp"
#include <glm/ext/matrix_transform.hpp>
#include <map>

unsigned int loadTexture(char const * path);

GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

int main() {
  Create_Window create_window;
  GLFWwindow * window = create_window.create_window("04-Advanced OpenGL");

  // Shader section
  Shader ground("sh.vs", "sh.fs");
  Shader chapanBox("sh.vs", "sh.fs");
  Shader gridShader("grid.vs", "grid.fs");
  Shader outLine("shaderSingleColor.vs", "shaderSingleColor.fs");
  Shader transparentShader("trans_image.vs", "trans_image.fs");

  // Load the models
  Model loadGround((char *)"meshes/Ground.gltf");
  Model loadChapanBox((char *)"meshes/ChapanBox.gltf");
  Model loadZPepper((char *)"meshes/Plane.gltf");
  Grid grid;

  // Simple Plane
  float transparentVertices[] = {
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
  };

  // transparent VAO
  unsigned int transparentVAO, transparentVBO;
  glGenVertexArrays(1, &transparentVAO);
  glGenBuffers(1, &transparentVBO);
  glBindVertexArray(transparentVAO);
  glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glBindVertexArray(0);

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
  struct nk_colorf out_line_color = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat out_line_scale = 0.0f;
  GLfloat lt_power = 0.0f;
  bool show_grid = true, show_ground = false, out_line_onoff = false;
  // --------------------------------------------------------------------------

  // Initialize camera with proper starting position
  camera.Position = glm::vec3(0.0f, 0.3f, 1.0f);  // Back a bit from the model

  // load textures
  // -------------
  unsigned int transparentTexture = loadTexture((char *)"pictures/Green-window.png");
  // -------------
 
  std::vector<glm::vec3> z_pepper {
    glm::vec3(-1.5f, 0.5f, 2.48f),
    glm::vec3( 1.5f, 0.5f, 2.51f),
    glm::vec3( 0.0f, 0.5f, 2.7f),
    glm::vec3(-0.3f, 0.5f, 2.3f),
    glm::vec3 (0.5f, 0.5f, 2.6f)
  };

  transparentShader.setInt("texture1", 0);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Ui sections
    ui_setting(ctx, &bg_color, &lt_color, &lt_power, &show_grid, &show_ground, &out_line_color, &out_line_scale, &out_line_onoff);

    // 3D rendering
    // ------------------------------------------------------------------------
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)create_window.WIDTH / (float)create_window.HEIGHT, 0.1f, 100.0f);
    glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::vec3 lightColor = glm::vec3(lt_color.r, lt_color.g, lt_color.b);
    GLfloat lightPower = lt_power;
    glm::vec3 viewPos = glm::vec3(camera.Position);
    glm::mat4 chapanBoxModel = glm::mat4(1.0f);

    // sort the transparent windows before rendering
    // ---------------------------------------------
    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < z_pepper.size(); i++) {
      float distance = glm::length(camera.Position - z_pepper[i]);
      sorted[distance] = z_pepper[i];
    }

    // Grid section
    if (show_grid == true) {
      glm::mat4 gridModel = glm::mat4(1.0f);

      gridShader.use();
      gridShader.setMat4("view", view);
      gridShader.setMat4("projection", projection);
      gridModel = glm::scale(gridModel, glm::vec3(0.1f));
      gridShader.setMat4("model", gridModel);
      gridShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // Gray grid lines
      grid.Draw(gridShader);
    }

    if (show_ground) {
      // Gound section
      glm::mat4 groundModel = glm::mat4(1.0f);

      ground.use();
      ground.setMat4("view", view);
      ground.setMat4("projection", projection);
      ground.setVec3("lightPos", lightPos);
      ground.setVec3("lightColor", lightColor);
      ground.setFloat("lightPower", lightPower);
      ground.setVec3("viewPos", viewPos);
      groundModel = glm::scale(groundModel, glm::vec3(0.3f));
      groundModel = glm::translate(groundModel, glm::vec3(0.0f, -0.01f, 0.0f));
      ground.setMat4("model", groundModel);
      loadGround.Draw(ground);
    }

    if (out_line_onoff) {
      // Box section
      // FIRST PASS: Draw normal box and write to stencil buffer
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0xFF);

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

      // SECOND PASS: Draw outline
      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glStencilMask(0x00);
      glDisable(GL_DEPTH_TEST);
      // OutLine color
      outLine.use();
      outLine.setMat4("view", view);
      outLine.setMat4("projection", projection);

      // Create scaled model for outline
      glm::mat4 outlineModel = chapanBoxModel;
      outlineModel = glm::scale(outlineModel, glm::vec3(1.0f + out_line_scale)); // Slightly larger
      outLine.setMat4("model", outlineModel);
      outLine.setVec3("color", glm::vec3(out_line_color.r, out_line_color.g, out_line_color.b));
      loadChapanBox.Draw(outLine);
    
      // Reset stencil and depth test
      glStencilMask(0xFF);
      glStencilFunc(GL_ALWAYS, 0, 0xFF);
      glEnable(GL_DEPTH_TEST);
    }
    else {
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
    }

    // Draw window or peppers
    // -------------------
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, transparentTexture);
    transparentShader.use();

    // for (unsigned int i = 0; i < z_pepper.size(); i++) {
    for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
      transparentShader.setMat4("view", view);
      transparentShader.setMat4("projection", projection);
  
      // Reset model matrix for each plane
      glm::mat4 zpepperModel = glm::mat4(1.0f);
      zpepperModel = glm::scale(zpepperModel, glm::vec3(0.2f));
      zpepperModel = glm::translate(zpepperModel, it->second);
  
      transparentShader.setMat4("model", zpepperModel);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    // -------------------
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

unsigned int loadTexture(char const * path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
