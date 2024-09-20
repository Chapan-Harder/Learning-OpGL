#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec3 Normal;
in vec2 TextureCoord;

uniform sampler2D ourTexture;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse);
    FragColor = texture(ourTexture, TextureCoord) * vec4(result, 1.0f);
}
