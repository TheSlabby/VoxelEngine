#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 viewPos;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightDir = normalize(vec3(-1.0, -3.0, -0.5));

    float ambientStrength = 0.6;
    float specularStrength = 2.0;
    float diffuseStrength = 0.7;

    vec3 norm = normalize(Normal);

    // diffuse stuff
    float diff = max(dot(-lightDir, norm), 0.0f) * diffuseStrength;
    vec3 diffuse = diff * lightColor;

    //spectral stuff
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    //ambient color
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

    FragColor = texture(texture1, TexCoords) * vec4(ambient + diffuse + specular, 1.0);
}