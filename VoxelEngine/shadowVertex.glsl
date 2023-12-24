#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;


uniform sampler2D texture1;
uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;
uniform vec3 viewPos;

void main() {
	gl_Position = lightSpaceMatrix * vec4(aPos, 1.0);
}