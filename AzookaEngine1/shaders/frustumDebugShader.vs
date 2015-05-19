#version 330
layout (location = 0) in vec4 position;

uniform mat4 cameraMatrix;
uniform mat4 rotationMatrix;

void main()
{
	gl_Position = cameraMatrix * rotationMatrix * position;
}
