#include "StdAfx.h"
#include "FBO.h"

FBO::FBO(void)
{
}

FBO::~FBO(void)
{
}
void FBO::Init()
{
	GLsizei screenW = Globals::screenWidth;
	GLsizei screenH = Globals::screenHeight;

	// Create frame buffer object
	glGenFramebuffers(1, &m_hFrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFrameBufferId);

	// Generate color texture
	glGenTextures(1, &m_hColorTexId);
	glBindTexture(GL_TEXTURE_2D, m_hColorTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Init empty color texture with size of screen
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenW, screenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	// Attach color texture to FBO color attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hColorTexId, 0);
	// Bind system default texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Generate depth texture
	glGenTextures(1, &m_hDepthTexId);
	glBindTexture(GL_TEXTURE_2D, m_hDepthTexId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Init empty depth texture with size of screen
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenW, screenH, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	// Attach depth texture to FBO depth attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_hDepthTexId, 0);
	// Bind system default texture
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if ( status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Don't have frame buffer");
	}

	// Bind  system default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
GLuint FBO::GetFrameBufferID()
{
	return m_hFrameBufferId;
}
GLuint FBO::GetColorTextureID()
{
	return m_hColorTexId;
}
GLuint FBO::GetDepthTextureID()
{
	return m_hDepthTexId;
}
void FBO::BindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_hFrameBufferId);
}
void FBO::UnbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FBO::CleanUp()
{
	glDeleteFramebuffers(1, &m_hFrameBufferId);
	glDeleteTextures(1, &m_hColorTexId);
	glDeleteTextures(1, &m_hDepthTexId);
}