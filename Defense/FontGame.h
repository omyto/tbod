#pragma once
#include "LoadFNT.h"
#include "Shaders.h"
class FontGame
{
private:
	static FontGame* m_instance;

	GLuint textcoorId;
	GLuint textureId;
	GLuint uVNBaiSau_BTextcoorId;
	GLuint uVNBaiSau_BTextureId;
	GLuint courierNewTextcoorId;
	GLuint courierNewTextureId;
	GLuint vinqueTextcoorId;
	GLuint vinqueTextureId;

	GLfloat vertexData[12];
	GLfloat FontSizeW;
	GLfloat FontSizeH;

	Shaders* fontShader;
public:
	FontGame(void);
	~FontGame(void);

	static FontGame* GetInstance();
	static void FreeInstance();

	void Init();
	void CleanUp();

	void DrawString(const char *, int x, int y, float = 2.5f, int = 0);
	void DrawLevel(int, int = 5, int = 5, float = 1.0f, int = 0);
	void DrawNumber(int, int, int, float, int);
};

