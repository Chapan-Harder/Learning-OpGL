#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos = vec3(2.0, 2.0, 2.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform float lightPower = 0.0;
uniform vec3 viewPos;

void main()
{
        // Ambient
        float ambientStrength = 0.5;
        vec3 ambient = ambientStrength * (lightColor * lightPower);
    
        // Diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
    
        // Combine with texture
        vec4 texColor = texture(texture_diffuse1, TexCoords);
        if (texColor.a < 0.1)
            discard;

        vec3 result = (ambient + diffuse) * texColor.rgb;
        FragColor = vec4(result, 1.0);
}
