#pragma once
#include "stb_image.h"
#include <GLEW/glew.h>
#include <SOIL/SOIL.h>

#include <string>

using namespace std;
class Texture
{
public:
	Texture(string& p_fileName);
	~Texture();

	void Bind(unsigned int p_unit);


private:
	GLuint m_textureHandle;
};

