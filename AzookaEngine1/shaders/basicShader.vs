#version 330

attribute vec3 vertexPosition;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec3 normal0;

uniform mat4 worldMatrix;
uniform mat4 cameraMatrix;


void main()
{
	gl_Position = cameraMatrix * worldMatrix* vec4(vertexPosition,1);
	texCoord0 = texCoord;
	normal0 = (inverse(transpose(worldMatrix)) * vec4(normal, 0)).xyz;
}