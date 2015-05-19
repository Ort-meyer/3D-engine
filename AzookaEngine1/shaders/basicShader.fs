#version 330

varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D diffuse;
uniform vec3 lightDirection;

out vec4 fragColor;

void main()
{
	//vec3 lightDirection = vec3(1,-1,1);
	fragColor = texture2D(diffuse,texCoord0) *clamp(dot(-lightDirection, normal0), 0, 1); 
	fragColor.a =1;
}