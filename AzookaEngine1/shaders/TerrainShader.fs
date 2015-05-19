#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec4 vertexPosition0;

out vec4 fragColor;

uniform sampler2DShadow shadowMap;
uniform sampler2D textureTerrain;
uniform mat4 lightMatrix;
uniform vec3 lightDirection;

void main()
{
	vec4 positionLightPers = lightMatrix * vertexPosition0;
	positionLightPers.xy /= positionLightPers.w;
	vec2 smTexCoord = vec2(0.5 * positionLightPers.x, 0.5 * positionLightPers.y) + 0.5;
	float depth = positionLightPers.z / positionLightPers.w;
	//float depth = positionLightPers.z;
	depth = 0.5*depth + 0.5;
	vec4 finalColor;
	//float test = texture2D(shadowMap,smTexCoord).z;
	
	float xOffset = 1.0/1024;
    float yOffset = 1.0/768;
	float Factor = 0.0;
	
	for (int y = -1 ; y <= 1 ; y++)  //Alliasing from the 9 pixels around you + the acctual pixel
	{
		for (int x = -1 ; x <= 1 ; x++) 
		{
			vec2 Offsets = vec2(x * xOffset, y * yOffset);
			vec3 UVC = vec3(smTexCoord + Offsets, depth);
			Factor += texture(shadowMap, UVC);
		}
    }
	Factor =  0.5 + Factor/9;
	//if(depth > texture(shadowMap,vec3(smTexCoord,depth))) // Without aliasing
	//{
		//finalColor = vec4(0,0,0,1);
	//}
	//else
	//{
		finalColor =  texture2D(textureTerrain,texCoord0)* clamp(dot(lightDirection, normal0), 0, 1) * Factor;
	//}
	fragColor = finalColor;
	fragColor.a = 1;
}