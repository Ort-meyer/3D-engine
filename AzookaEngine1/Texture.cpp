#include "Texture.h"
#include <cassert>
#include <iostream>

Texture::Texture(string& p_fileName)
{
	int t_width;
	int t_height;
	int t_componentCount;
	unsigned char* t_imageData = SOIL_load_image(p_fileName.c_str(), &t_width, &t_height, 0, SOIL_LOAD_RGBA);
	
	if (t_imageData == NULL)
	{
		cerr << "texture loading failed for texture: " << p_fileName << endl;
	}

	glGenTextures(1, &m_textureHandle);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_imageData);

	//glTexImage2D()

	SOIL_free_image_data(t_imageData);
}



Texture::~Texture()
{
	glDeleteTextures(1, &m_textureHandle);
}


void Texture::Bind(unsigned int p_unit)
{
	assert(p_unit >= 0 && p_unit <= 31); //only 31 textures in openGL
	glActiveTexture(GL_TEXTURE0 + p_unit);	//good way to use more textures
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);
}