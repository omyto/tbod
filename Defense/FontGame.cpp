#include "stdafx.h"
#include "FontGame.h"
#include <cstring>
#include "ResourcesManager.h"
#include "GameDefineList.h"
#include "Globals.h"

struct quadVertex
{
	Vector3 A;	//	A-------B
	Vector3 B;	//	|       |
	Vector3 C;	//	|       |
	Vector3 D;	//	D-------C
};

FontGame* FontGame::m_instance = NULL;
FontGame::FontGame()
{
	uVNBaiSau_BTextcoorId = 0;
	uVNBaiSau_BTextureId = 0;

	vinqueTextcoorId = 0;
	vinqueTextureId = 0;

	courierNewTextcoorId = 0;
	courierNewTextureId = 0;

	fontShader = NULL;
}


FontGame::~FontGame()
{
}

FontGame* FontGame::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new FontGame();
	}
	return m_instance;
}

void FontGame::Init()
{
	uVNBaiSau_BTextcoorId = loadFNT("../Resources/Fonts/UVNBaiSau_B.fnt");
	uVNBaiSau_BTextureId = ResourcesManager::GetInstance()->GetTexture2DByID(FONT_BAI_SAU_TEXTURE_ID)->GetTextureID();

	vinqueTextcoorId = loadFNT("../Resources/Fonts/Vinque.fnt");
	vinqueTextureId = ResourcesManager::GetInstance()->GetTexture2DByID(FONT_VINQUE_TEXTURE_ID)->GetTextureID();

	courierNewTextcoorId = loadFNT("../Resources/Fonts/CourierNew.fnt");;
	courierNewTextureId = ResourcesManager::GetInstance()->GetTexture2DByID(FONT_COURIERNEW_TEXTURE_ID)->GetTextureID();

	fontShader = ResourcesManager::GetInstance()->GetShaderByID(SHADER_2D_ID);
}

void FontGame::FreeInstance()
{
	if (m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void FontGame::DrawString(const char * str, int x, int y, float size, int font)
{
	FontSizeW = size*0.015625f;
	FontSizeH = size*0.037f;
	switch (font)
	{
	case 1:
		textcoorId = courierNewTextcoorId;
		textureId = courierNewTextureId;
		break;
	case 2:
		textcoorId = uVNBaiSau_BTextcoorId;
		textureId = uVNBaiSau_BTextureId;
		break;
	case 0:
		textcoorId = vinqueTextcoorId;
		textureId = vinqueTextureId;
		break;
	}
	glUseProgram(fontShader->program);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//GLfloat vertexData[12];
	//{
	//	-0.015625f	,  0.037f	, 0 ,
	//	 0.015625f	,  0.037f	, 0 ,
	//	 0.015625f	, -0.037f	, 0 ,
	//	-0.015625f	, -0.037f	, 0
	//};
	float SCREEN_W = Globals::screenWidth;
	float SCREEN_H = Globals::screenHeight;

	vertexData[0] = x*2.0f / SCREEN_W - 1.0f; vertexData[1] = -(y*2.0f / SCREEN_H - 1.0f); vertexData[2] = 0;
	vertexData[3] = vertexData[0] + FontSizeW; vertexData[4] = -(y*2.0f / SCREEN_H - 1.0f); vertexData[5] = 0;
	vertexData[6] = vertexData[0] + FontSizeW; vertexData[7] = vertexData[1] - FontSizeH; vertexData[8] = 0;
	vertexData[9] = x*2.0f / SCREEN_W - 1.0f; vertexData[10] = vertexData[1] - FontSizeH; vertexData[11] = 0;

	int indices[] = {
		0, 3, 2,
		0, 2, 1
	};
	for (unsigned int i = 0; i < strlen(str); i++)
	{
		// Bind vertex
		if (fontShader->positionAttribute != -1)
		{
			glEnableVertexAttribArray(fontShader->positionAttribute);
			glVertexAttribPointer(fontShader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), vertexData);
		}

		// Bind textcoor
		glBindBuffer(GL_ARRAY_BUFFER, textcoorId);
		if (fontShader->uvLocation != -1)
		{
			glEnableVertexAttribArray(fontShader->uvLocation);
			glVertexAttribPointer(fontShader->uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)(sizeof(TextCoorData)*(str[i] - 32)));
		}

		//Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		if (fontShader->uListTexturesLoc[0] != -1)
		{
			glUniform1i(fontShader->uListTexturesLoc[0], 0);
		}

		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, indices);

		vertexData[0] = vertexData[0] + FontSizeW;
		vertexData[3] = vertexData[0] + FontSizeW;
		vertexData[6] = vertexData[0] + FontSizeW;
		vertexData[9] = vertexData[0];

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	glDisable(GL_BLEND);
}
void FontGame::DrawLevel(int level, int x, int y, float size, int font)
{
	char str[10] = { 'L', 'e', 'v', 'e', 'l', ' ' };
	//                 0     1     2     3     4     5     6
	int i = 6;
	do
	{
		str[i] = level % 10 + 48;
		level /= 10;
		i++;
	} while (level);
	str[i] = '\0';
	char temp;
	for (int j = 0; j < (i - 6) / 2; j++)
	{
		temp = str[j + 6];
		str[j + 6] = str[i - j - 1];
		str[i - j - 1] = temp;
	}
	DrawString(str, x, y, size, font);
}

void FontGame::DrawNumber(int number, int x, int y, float size, int font)
{
	char str[15];
	int i = 0;
	do
	{
		str[i] = number % 10 + 48;
		number /= 10;
		i++;
	} while (number);
	str[i] = '\0';
	char temp;
	for (int j = 0; j < i / 2; j++)
	{
		temp = str[j];
		str[j] = str[i - j - 1];
		str[i - j - 1] = temp;
	}
	DrawString(str, x, y, size, font);

}

void FontGame::CleanUp()
{
	glDeleteBuffers(1, &uVNBaiSau_BTextcoorId);
	glDeleteBuffers(1, &courierNewTextcoorId);
	glDeleteBuffers(1, &vinqueTextcoorId);
}