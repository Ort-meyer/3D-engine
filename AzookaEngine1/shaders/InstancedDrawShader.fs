#version 330

in vec2 texCoord0;
in vec3 normal0;

uniform sampler2D texture;
uniform vec3 lightDirection;

out vec4 fragColor;

void main()
{
	fragColor = texture2D(texture,texCoord0) * clamp(dot(lightDirection, normal0), 0, 1); 
	fragColor.a = 1;
}