#ifndef GRID_H
#define GRID_H

#include <found_libs.hpp>
#include "shader.cpp"

class Grid {
public:
  Grid() {
    setupGrid();
  }

  void Draw(Shader &shader) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, (gridSize * 8 + 8)); // 2 lines per grid cell in both directions
    glBindVertexArray(0);
  }

private:
  unsigned int VAO, VBO;
  int gridSize = 10; // 10x10 grid

  void setupGrid() {
    std::vector<float> vertices;

      // Create grid lines
      for (int i = -gridSize; i <= gridSize; i++) {
        // X-axis lines (parallel to Z)
        vertices.push_back(-gridSize);
        vertices.push_back(0.0f);
        vertices.push_back(i);
        vertices.push_back(gridSize);
        vertices.push_back(0.0f);
        vertices.push_back(i);

        // Z-axis lines (parallel to X)
        vertices.push_back(i);
        vertices.push_back(0.0f);
        vertices.push_back(-gridSize);
        vertices.push_back(i);
        vertices.push_back(0.0f);
        vertices.push_back(gridSize);
      }

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);

      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindVertexArray(0);
    }
};
#endif
