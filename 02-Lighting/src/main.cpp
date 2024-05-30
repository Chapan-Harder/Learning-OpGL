// Import Others
#include "includes.h"
#include "window.cpp"
#include "shader.cpp"

GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

int main(){
    // Calling The Window Section
    GLFWwindow *window = createMyWindow("(---Chapan Harder Window---)");

    // Calling The Shader Section
    GLuint shaderProgram = createShaderProgram();

    // Make The Triangle Section
    GLfloat vertices[] = {
        // Positions                // Texture Coordinates
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,        1.0f, 0.0f,
         0.5f, 0.5f, -0.5f,         1.0f, 1.0f,
         0.5f, 0.5f, -0.5f,         1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,         0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,         1.0f, 0.0f,
         0.5f, -0.5f, 0.5f,         0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,        1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,        0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,         0.0f, 0.0f,

         0.5f, -0.5f, -0.5f,        0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,         1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,          1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,          1.0f, 1.0f,
         0.5f, 0.5f, -0.5f,         0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,        0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,         1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,          1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,          1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,         0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f,         0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,          1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,          1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,          1.0f, 1.0f,
         0.5f, 0.5f, -0.5f,         0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,         0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,         0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,         1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,          1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,          1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,          0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,         0.0f, 0.0f,
    };

    // GLfloat vertices[] = {
    //     // Positions                // Texture Coordinates
    //     -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
    //      0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
    //      0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
    //     -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
    // };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // Texture Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // -----------------------------------------------------------------------------------------------------------------
    // load and generate the texture
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint width, height, nrChannels;
    GLubyte *data = stbi_load("Blue ground test.jpg", &width, &height, &nrChannels, 0);

    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // -----------------------------------------------------------------------------------------------------------------

    // Create The Projection Matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)){
        GLfloat currenFrame = glfwGetTime();
        deltaTime = currenFrame - lastFrame;
        lastFrame = currenFrame;
        glfwPollEvents();
        // Action Buttons
        camera_and_window_action(window, deltaTime);

        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create Transformations
        GLfloat timeValue = glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, timeValue * 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));

        // Create The View Transformations
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPose, cameraPose + cameraFront, cameraUp);

        // get matrix's uniform location and set matrix
        // Transform The Model
        GLint modelLog = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLog, 1, GL_FALSE, glm::value_ptr(model));
        // Transform The View
        GLint viewLog = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLog, 1, GL_FALSE, glm::value_ptr(view));
        // Transform The Projection
        GLint projectionLog = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLog, 1, GL_FALSE, glm::value_ptr(projection));

        glUseProgram(shaderProgram);
        // Bind the vertex array object And Texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}