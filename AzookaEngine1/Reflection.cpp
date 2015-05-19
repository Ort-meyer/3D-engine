#include "Reflection.h"


Reflection::Reflection()
{

}


Reflection::~Reflection()
{
}

void Reflection::Initialize(mat4 p_perspectiveMatrix)
{
	glGenFramebuffers(1, &m_framBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framBuffer);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture, 0);
	GLenum t_drawBuffers[1];
	t_drawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, t_drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	m_reflectionPerspectiveMatrix = p_perspectiveMatrix;
}

void Reflection::InitializeShadow()
{
	glGenFramebuffers(1, &m_framBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framBuffer);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 768, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	//GLuint depthrenderbuffer;
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	//glGenRenderbuffers(1, &depthrenderbuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture, 0);
	//GLenum t_drawBuffers[1];
	//t_drawBuffers[0] = GL_DEPTH_ATTACHMENT;
	//glDrawBuffers(1, t_drawBuffers);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;


}


void Reflection::Update(mat4 p_worldMatrix)
{
	m_reflectionCameraMatrix = m_reflectionPerspectiveMatrix*p_worldMatrix;
}


void Reflection::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_framBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Reflection::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

mat4 Reflection::GetReflectionCameraMatrix()
{
	return m_reflectionCameraMatrix;
}
GLuint Reflection::GetTexture()
{
	return m_texture;
}