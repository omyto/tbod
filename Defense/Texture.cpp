#include "StdAfx.h"
#include "Texture.h"

Texture::Texture(void) : m_hTexture(0)
{
}

Texture::~Texture(void)
{
	glDeleteTextures(1, &m_hTexture);
}

void Texture::Init2DTextures(char* pFile)
{
	printf("Init 2D texture 1 - pFile: %s\n",pFile);
	glGenTextures(1, &m_hTexture);
	glBindTexture(GL_TEXTURE_2D, m_hTexture);

	int width, height, bpp;
	char *imgData = LoadTGA(pFile, &width, &height, &bpp);
	glTexImage2D(GL_TEXTURE_2D, 0, (bpp == 32)? GL_RGBA : GL_RGB, width, height, 0, (bpp == 32)? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imgData);
	delete [] imgData;

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::Init2DTextures(char* pFile, char* pTiling)
{
	printf("Init 2D texture 2 - pFile: %s\n", pFile);
	glGenTextures(1, &m_hTexture);
	glBindTexture(GL_TEXTURE_2D, m_hTexture);

	int width, height, bpp;
	char *imgData = LoadTGA(pFile, &width, &height, &bpp);
	glTexImage2D(GL_TEXTURE_2D, 0, (bpp == 32)? GL_RGBA : GL_RGB, width, height, 0, (bpp == 32)? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imgData);
	delete [] imgData;

	if( strcmp(pTiling, "CLAMP") == 0 )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	if( strcmp(pTiling, "REPEAT") == 0 )
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Init3DTextures(char* pFile)
{
}

void Texture::InitCubeTexture(char* pFile)
{
	printf("Init cube texture - pFile: %s\n", pFile);
	glGenTextures(1, &m_hTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_hTexture);

	GLint width, height, bpp;
	char* bufferTGA = LoadTGA(pFile, &width, &height, &bpp);

	int offsetDataW[] = { 2, 0, 1, 1, 1, 3};
	int offsetDataH[] = { 1, 1, 0, 2, 1, 1};

	GLenum internalFormat = (bpp == 32)? GL_RGBA : GL_RGB;
	GLsizei cubeWidth = width / 4;
	GLsizei cubeHeight = height /3;

	for(int i = 0 ; i < 6 ; i++)
	{
		char* cubePixels = ExtractFace(bufferTGA, width, height, bpp, offsetDataW[i], offsetDataH[i]);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			internalFormat,
			cubeWidth,
			cubeHeight,
			0,
			internalFormat,
			GL_UNSIGNED_BYTE,
			cubePixels
		);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		delete [] cubePixels;
	}
	delete [] bufferTGA;
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

char* Texture::ExtractFace(char* bufferTGA, GLsizei width, GLsizei height, GLsizei bpp, int offsetX, int offsetY)
{
	GLsizei cubeWidth = width / 4;
	GLsizei cubeHeight = height /3;
	GLsizei numColors = bpp / 8;
	GLsizei cubeLineSize = cubeWidth * numColors;

	char* cubePixels = new char[cubeWidth * cubeHeight * numColors];
	for(int iHeight = 0; iHeight < cubeHeight; iHeight++)
	{
		char* dstAddress = cubePixels + cubeLineSize * iHeight;
		char* srcAddress =  bufferTGA + ( width*(cubeHeight*offsetY + iHeight) + cubeWidth*offsetX ) * numColors;
		memcpy( dstAddress, srcAddress, cubeLineSize);
	}
	return cubePixels;
}

GLuint Texture::GetTextureID()
{
	return m_hTexture;
}
//void Texture::CleanUp()
//{
//	if(m_hTexture) glDeleteTextures(1, &m_hTexture);
//}
