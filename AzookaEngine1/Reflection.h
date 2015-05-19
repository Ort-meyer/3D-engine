#pragma once
#include <GLEW\glew.h>
#include <glm\glm.hpp>


using namespace glm;

class Reflection
{
public:
	Reflection();
	~Reflection();

	void Initialize(mat4 p_perspectiveMatrix);
	void InitializeShadow();
	mat4 GetReflectionCameraMatrix();
	GLuint GetTexture();

	void BindFrameBuffer();
	void UnbindFrameBuffer();
	void Update(mat4 p_worldMatrix);
private:
	GLuint m_framBuffer;
	GLuint m_texture;
	
	mat4 m_reflectionCameraMatrix;
	mat4 m_reflectionPerspectiveMatrix;
	
};

