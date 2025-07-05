// Import Others
#include "includes.h"
#include "window.cpp"
#include "shader.cpp"

GLuint loadTexture(GLchar const * path);

// lighting
// glm::vec3 lightPosition(0.6f, 0.6f, 0.6f);

GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

int main(){
    // Calling The Window Section
    GLFWwindow *window = createMyWindow("(---Chapan Harder Window---)");

    // Calling The Shader Section
    Shader lightingShader("basic_lighting.vs", "basic_lighting.fs");
    Shader lightCubeShader("light_cube.vs", "light_cube.fs");

    // Make The Triangle Section
    GLfloat vertices[] = {
        // Positions                // Normals              // Texture Coordinates
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,      0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,      1.0f, 0.0f,
         0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,      1.0f, 1.0f,
         0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,      1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,      0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,      0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,        0.0f, -1.0, 0.0f,       1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,         0.0f, -1.0, 0.0f,       1.0f, 0.0f,
         0.5f, -0.5f, 0.5f,         0.0f, -1.0, 0.0f,       0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, -1.0, 0.0f,       1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,        0.0f, -1.0, 0.0f,       0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,         0.0f, -1.0, 0.0f,       0.0f, 0.0f,

         0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,       1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,          1.0f, 0.0f, 0.0f,       1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,          1.0f, 0.0f, 0.0f,       1.0f, 1.0f,
         0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,       0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,       0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,          -1.0f, 0.0f, 0.0f,      1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,          -1.0f, 0.0f, 0.0f,      1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,         -1.0f, 0.0f, 0.0f,      0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,      0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,       0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,       1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,       1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,       1.0f, 1.0f,
         0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,       0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,       0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,       0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -1.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-3.3f,  1.0f, -1.5f)
    };

    GLuint VBO, meshVAO;
    glGenVertexArrays(1, &meshVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(meshVAO);

    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Normal Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // -----------------------------------------------------------------------------------------------------------------
    // load and generate the texture
    GLuint diffuseMap = loadTexture("container2.jpg");
    GLuint specularMap = loadTexture("container2_specular.jpg");
    // -----------------------------------------------------------------------------------------------------------------

    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)){
        GLfloat currenFrame = glfwGetTime();
        deltaTime = currenFrame - lastFrame;
        lastFrame = currenFrame;
        glfwPollEvents();
        // Action Buttons
        camera_and_window_action(window, deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat timeValue = glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);

        // Light Location
        // --------------------------
        // Define your variables
        GLfloat radius = 1.5f; // Distance from the center
        GLfloat speed = 1.0f; // Speed of rotation

        // Calculate the new position for the light cube
        GLfloat x = radius * cos(speed * timeValue); // X position based on cosine
        GLfloat z = radius * sin(speed * timeValue); // Z position based on sine
        glm::vec3 lightPosition(x, 0.8f, z); // Update light position
        // --------------------------

        // Create The Projection Matrix
        // --------------------------
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
        // Create The View Transformations
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPose, cameraPose + cameraFront, cameraUp);
        // --------------------------

        // Draw Mesh Shader
        // --------------------------
        lightingShader.use();
        lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 1);
        lightingShader.setVec3("viewPosition", cameraPose);

        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        lightingShader.setFloat("material.shininess", 32.0f);
        // view/projection transformations
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("model", model);
        // --------------------------


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Render The Cube
        // --------------------------
        glBindVertexArray(meshVAO);
        // Create Transformations For Cube
        // --------------------------
        for (GLuint i = 0; i < 10; i++) {
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            
            // Create Transformations
            model = glm::translate(model, glm::vec3(0.0, 0.0, sin(timeValue * 3.0f) / 2.0f));

            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // --------------------------
        // --------------------------


        // Also Draw The Lamp Object
        // --------------------------
        // model = glm::mat4(1.0f);
        // model = glm::translate(model, lightPosition);
        // model = glm::scale(model, glm::vec3(0.1f)); // Make The Smaler Cube
        // lightCubeShader.use();
        // lightCubeShader.setMat4("projection", projection);
        // lightCubeShader.setMat4("view", view);
        // lightCubeShader.setMat4("model", model);
        // --------------------------

        // Render The Cube
        // --------------------------
        // glBindVertexArray(lightCubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // --------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

GLuint loadTexture(GLchar const * path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    GLint width, height, nrComponents;
    GLboolean *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
