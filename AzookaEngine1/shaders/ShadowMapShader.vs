#version 330

layout (location = 0) in vec3 vertexPosition;


uniform mat4 cameraMatrix;
uniform mat4 worldMatrix[10];


void main()
{
	gl_Position = cameraMatrix * worldMatrix[gl_InstanceID] * vec4(vertexPosition,1);
}