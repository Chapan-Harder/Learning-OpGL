#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

in vec3 FragPosition;
in vec3 Normal;
in vec2 TextureCoord;

uniform Material material;
uniform sampler2D ourTexture;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

void main()
{
    // Ambient
    vec3 ambient =  lightColor * material.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  lightColor * (diff * material.diffuse);

    // Specular
    vec3 viewDir = normalize(viewPosition - FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  lightColor * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = texture(ourTexture, TextureCoord) * vec4(result, 1.0f);
}
