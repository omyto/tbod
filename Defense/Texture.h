#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

class Texture
{
private:
	GLuint m_hTexture;
	char* ExtractFace(char* bufferTGA, GLsizei width, GLsizei height, GLsizei bpp, int offsetX, int offsetY);
public:
	Texture(void);
	~Texture(void);
	void Init2DTextures(char* pFile);
	void Init2DTextures(char* pFile, char* pTiling);
	void Init3DTextures(char* pFile);
	void InitCubeTexture(char* pFile);
	GLuint GetTextureID();
	//void CleanUp();
};
