#version 330

out float fragDepth;

void main()
{
	//fragDepth = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,0);
	fragDepth = gl_FragCoord.z/100;
}