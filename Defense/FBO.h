#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Globals.h"
class FBO
{
private:
	GLuint m_hFrameBufferId;
	GLuint m_hColorTexId;
	GLuint m_hDepthTexId;
public:
	FBO(void);
	~FBO(void);
	void Init();
	void CleanUp();
	GLuint GetFrameBufferID();
	GLuint GetColorTextureID();
	GLuint GetDepthTextureID();
	void BindFBO();
	void UnbindFBO();
};
