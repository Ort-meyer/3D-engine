#version 330
layout (location = 0) in vec4 position;

uniform mat4 cameraMatrix;

void main()
{
	gl_Position = cameraMatrix * position;
}
