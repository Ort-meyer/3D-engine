#include "ShaderProgram.h"
#include <fstream>
#include <iostream>


ShaderProgram::ShaderProgram(string& p_programName)
{
	m_programHandle = glCreateProgram();
	m_shaderHandles.push_back(CreateShader(LoadShader(p_programName + ".vs"), GL_VERTEX_SHADER));
	m_shaderHandles.push_back(CreateShader(LoadShader(p_programName + ".fs"), GL_FRAGMENT_SHADER));

	for (int i = 0; i < m_shaderHandles.size(); i++)
		glAttachShader(m_programHandle, m_shaderHandles.at(i));

	glBindAttribLocation(m_programHandle, 0, "vertexPosition");
	glBindAttribLocation(m_programHandle, 1, "texCoord");
	glBindAttribLocation(m_programHandle, 2, "normal");

	glLinkProgram(m_programHandle);
	CheckShaderError(m_programHandle,GL_LINK_STATUS, true, string("error: Program linking failed: "));

	glValidateProgram(m_programHandle);
	CheckShaderError(m_programHandle, GL_VALIDATE_STATUS, true, string("error: Program is invalid: "));
	
	m_uniforms.resize(NUM_UNIFORMS);
	m_uniforms.at(U_CAMERA_MATRIX) = glGetUniformLocation(m_programHandle, "worldMatrix");

}


ShaderProgram::~ShaderProgram()
{
	for (int i = 0; i < m_shaderHandles.size(); i++)
	{
		glDetachShader(m_programHandle, m_shaderHandles.at(i));
		glDeleteShader(m_shaderHandles.at(i));
	}
	glDeleteProgram(m_programHandle);
}


void ShaderProgram::Bind()
{
	glUseProgram(m_programHandle);
}

void ShaderProgram::Update(Transform p_transformation, mat4 t_cameraMatrix)
{
	mat4 t_finalMatrix = t_cameraMatrix*p_transformation.GetWorldMatrix();
	glUniformMatrix4fv(m_uniforms.at(U_CAMERA_MATRIX), 1, GL_FALSE, &t_finalMatrix[0][0]);
}



string ShaderProgram::LoadShader(string& p_fileName)
{
	ifstream t_file;
	t_file.open(p_fileName.c_str());
	string r_output;
	string t_line;

	if (t_file.is_open())
	{
		while (t_file.good())
		{
			getline(t_file, t_line);
			r_output.append(t_line + "\n");
		}
	}

	else
	{
		cout << "unable to load shader: " << p_fileName << endl;
	}
	return r_output;
}

void ShaderProgram::CheckShaderError(GLuint p_shaderHandle, GLuint p_flag, bool p_isProgram, string& errorMessage)
{
	GLint t_success = 0;
	GLchar t_error[1024] = { 0 };
	if (p_isProgram)
		glGetProgramiv(p_shaderHandle, p_flag, &t_success);
	else
		glGetShaderiv(p_shaderHandle, p_flag, &t_success);

	if (t_success == GL_FALSE)
	{
		if (p_isProgram)
			glGetProgramInfoLog(p_shaderHandle, sizeof(t_error), NULL, t_error);
		else
			glGetShaderInfoLog(p_shaderHandle, sizeof(t_error), NULL, t_error);
		cout << errorMessage << ": " << t_error << "'" << endl;
	}
}

GLuint ShaderProgram::CreateShader(string& p_shaderText, GLenum p_shaderType)
{
	GLuint r_shaderHandler = glCreateShader(p_shaderType);
	if (r_shaderHandler == 0)
		cerr << "error: shader creation failed!:" << endl;

	
	const GLchar* t_shaderSourceString = p_shaderText.c_str();
	GLint t_shaderSourceLength = p_shaderText.length();

	glShaderSource(r_shaderHandler, 1, &t_shaderSourceString, &t_shaderSourceLength);
	glCompileShader(r_shaderHandler);

	CheckShaderError(r_shaderHandler, GL_COMPILE_STATUS, false, string("error: shader compilation failed: "));

	return r_shaderHandler;
}

void ShaderProgram::AddUniform(uniform p_type, string p_uniformName)
{
	m_uniforms.at(p_type) = glGetUniformLocation(m_programHandle, p_uniformName.c_str());
}

//overloaded uniform creation methods
//Vectors of stuff
void ShaderProgram::UseUniform(uniform p_type, std::vector<glm::vec2>vector2)
{
	glUniform3fv(m_uniforms.at(p_type), vector2.size(), (float*)&vector2);
}
void ShaderProgram::UseUniform(uniform p_type, std::vector<glm::mat4> matrices)
{
	glUniformMatrix4fv(m_uniforms.at(p_type), matrices.size(), GL_FALSE, (float*)&matrices[0]);
}
void ShaderProgram::UseUniform(uniform p_type, std::vector<float> values)
{
	glUniform1fv(m_uniforms.at(p_type), values.size(), (float*)&values);
}

void ShaderProgram::UseUniform(uniform p_type, std::vector<int> values)
{
	glUniform1iv(m_uniforms.at(p_type), values.size(), (const GLint*)&values[0]);
}

void ShaderProgram::UseUniform(uniform p_type, std::vector<glm::vec3> vector3)
{
	glUniform3fv(m_uniforms.at(p_type), vector3.size(), (float*)&vector3[0]);
}


//Singular stuff
void ShaderProgram::UseUniform(uniform p_type, glm::vec3 vector3)
{
	glUniform3f(m_uniforms.at(p_type), vector3.x, vector3.y, vector3.z);
}
void ShaderProgram::UseUniform(uniform p_type, glm::mat4 mat4)
{
	glUniformMatrix4fv(m_uniforms.at(p_type), 1, GL_FALSE, &mat4[0][0]);
}

//works for textures
void ShaderProgram::UseUniform(uniform p_type, int uniformInt)
{
	glUniform1i(m_uniforms.at(p_type), uniformInt);
}

void ShaderProgram::UseUniform(uniform p_type, float uniformFloat)			
{
	glUniform1f(m_uniforms.at(p_type), uniformFloat);
}
