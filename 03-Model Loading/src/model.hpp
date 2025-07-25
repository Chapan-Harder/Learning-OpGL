#ifndef MODEL_H
#define MODEL_H

#include "mesh.hpp"
#include "shader.cpp"

static GLuint TextureFromFile(const char *path, const std::string &directory) {
    std::string filename = std::string(path);
    
    // Only prepend directory if it's not empty and the path isn't absolute
    if (!directory.empty() && path[0] != '/') {
        filename = directory + '/' + filename;
    }

    std::cout << "Loading texture from: " << filename << std::endl;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    // stbi_set_flip_vertically_on_load(true); // Important for OpenGL
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    
    if (data) {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << filename << " - " << stbi_failure_reason() << std::endl;
        stbi_image_free(data);
        return 0;
    }

    return textureID;
}

class Model {
public:
  Model(char *path) {
    loadModel(path);
  }
  void Draw(Shader &shader) {
    for(unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].Draw(shader);
  }

private:
  // Model data
  std::vector<Texture> textures_loaded;
  std::vector<Mesh> meshes;
  std::string directory;

  void loadModel(std::string path) {
    // Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
  }
  void processNode(aiNode *node, const aiScene *scene) {
    // Process all the node’s meshes (if any)
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }
    // Then do the same for each of its children
    for (GLuint i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }
  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      // Process vertex positions, normals and texture coordinates
      glm::vec3 vector;
      // Positions
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;
      // Normals
      if (mesh->HasNormals()) {
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
      }
      // Textures
      if (mesh->mTextureCoords[0]) {
        glm::vec2 vec;
        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.TextureCoordinate = vec;
      }
      else {
        vertex.TextureCoordinate = glm::vec2(0.0f, 0.0f);
      }
      vertices.push_back(vertex);
    }
    // Process indices
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (GLuint j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    // Process material
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      std::vector<Texture> diffuseMap = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());
      std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
  }
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        
        if(!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
  }
};

#endif
