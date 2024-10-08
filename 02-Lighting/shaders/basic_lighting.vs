#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // The TextureCoord
    FragPosition = vec3(model * vec4(position, 1.0f));
    TextureCoord = textureCoord;
    Normal = mat3(transpose(inverse(model))) * normal;

    gl_Position = projection * view * model * vec4(position, 1.0f);
}
