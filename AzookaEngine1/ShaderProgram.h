#pragma once

#include <string>
#include <vector>
#include <GLEW/glew.h>
#include "Transform.h"
#include "Camera.h"

enum uniform
{
	U_CAMERA_MATRIX,
	U_WORLD_MATRIX,
	U_TEXTURE_SAMPLER,
	U_LIGHT_DIRECTION,
	U_LIGHT_MATRIX,
	U_SHADOW_MAP,
	NUM_UNIFORMS
};

using namespace std;
class ShaderProgram
{
public:


	ShaderProgram(string& p_programName);
	~ShaderProgram();

	void Bind();
	void Update(Transform p_transformation, mat4 t_cameraMatrix);

	void AddUniform(uniform p_type, string p_uniformName);

	//overloaded uniform using location
	//Vectors of stuff
	void UseUniform(uniform p_type, std::vector<glm::vec2>vector2);
	void UseUniform(uniform p_type, std::vector<glm::mat4> matrices);
	void UseUniform(uniform p_type, std::vector<float> values);
	void UseUniform(uniform p_type, std::vector<int> values);
	void UseUniform(uniform p_type, std::vector<glm::vec3> vector3);

	//Singular stuff
	void UseUniform(uniform p_type, glm::vec3 vector3);
	void UseUniform(uniform p_type, glm::mat4 mat4);
	void UseUniform(uniform p_type, int uniformInt);
	void UseUniform(uniform p_type, float uniformFloat);

private:
	///member variables
	GLuint m_programHandle;
	vector<GLuint> m_shaderHandles;
	vector<GLuint> m_uniforms;




	//methods
	string LoadShader(string& p_fileName);
	void CheckShaderError(GLuint p_shaderHandle, GLuint p_flag, bool p_isProgram, string& errorMessage);
	GLuint CreateShader(string& p_shaderText, GLenum p_shaderType);


};

