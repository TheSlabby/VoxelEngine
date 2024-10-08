#version 460 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightDir;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // vec3 lightDir = normalize(lightDir);

    float ambientStrength = 0.6;
    float specularStrength = 2.0; //currently unused
    float diffuseStrength = 0.8;

    vec3 norm = normalize(Normal);

    // diffuse stuff
    float diff = max(dot(-lightDir, norm), 0.0f) * diffuseStrength;
    vec3 diffuse = diff * lightColor;


    //ambient color
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

    //shadows
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // setup bias
    float bias = 0.001;
    // check whether current frag pos is in shadow
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float shadow = 0.0;

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 15.0;
    if(projCoords.z > 1.0)
    shadow = 0.0;

    FragColor = texture(texture1, TexCoords) * vec4(ambient + diffuse + (0.7 - shadow), 1.0);

}