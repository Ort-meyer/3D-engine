#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 texCoord0;
out vec3 normal0;

uniform mat4 cameraMatrix;
uniform mat4 worldMatrix[10];

void main()
{
	gl_Position = cameraMatrix* worldMatrix[gl_InstanceID] * vec4(vertexPosition,1);
	texCoord0 = texCoord;
	normal0 = (normalize(worldMatrix[gl_InstanceID] * vec4(normal, 0))).xyz;
}