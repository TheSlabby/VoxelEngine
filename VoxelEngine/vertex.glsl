#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() {
	FragPos = aPos;
	Normal = aNormal;
	TexCoords = aTex;
	gl_Position = projection * view * vec4(aPos, 1.0);
	//gl_Position = lightSpaceMatrix * vec4(aPos, 1.0);
}