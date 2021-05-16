#pragma once
#include "StdAfx.h"
#include <stddef.h>
#include "ResourcesManager.h"
enum Texture2DId
{
	MAIN_SCREEN_TEXTURE_2D = 0,
	MAIN_MENU_CONTINUE = 1,
	MAIN_MENU_NEW = 2,
	MAIN_MENU_HELP = 3,
	MAIN_MENU_AUTHOR = 4,
	IN_GAME_MENU_RESUME = 5,
	IN_GAME_MENU_RESTART = 6,
	IN_GAME_MENU_MAIN_MENU = 7,
	IN_GAME_MENU_GROUND = 8,
	LEVEL_COMPLETE_GROUND = 9,
	LEVEL_COMPLETE_RESTART = 10,
	LEVEL_COMPLETE_MAIN_MENU = 11,
	LEVEL_COMPLETE_NEXT_LEVEL = 12,
	GAME_OVER_GROUND = 13,
	GAME_OVER_RESTART = 14,
	GAME_OVER_MAIN_MENU = 15,
	HELP_OBJECT_2D = 16,
	BULLETS_GROUND = 17,
	ENEMIES_GROUND = 18,
	HP_GROUND = 19,
	LEVEL_GROUND = 20
};
struct Object2DType
{
	GLuint vboId;
	GLuint textureId;
	int x, y, w, h;
};
class Object2DManager
{
private:
	static Object2DManager* m_instance;
	Shaders* m_shader;
	unsigned int m_numOfObject2D;
	GLuint m_iboID;
	GLuint m_numOfIndices;
	Object2DType* m_listObject2D;
public:
	Object2DManager();
	~Object2DManager();
	static Object2DManager* GetInstance();
	static void FreeInstance();
	void Init(char* pFileTarget);
	void Draw(int objectID);
	void CleanUp();
	GLuint CreateRectangle(unsigned int i_x, unsigned int i_y, unsigned int i_w, unsigned int i_h);
	Vector2 ScreenPosToPos(unsigned int Sx, unsigned int Sy);
	//Vector4 GetPosSizeObject(int objID);
	bool IsClickInObject(int objID);
	void DrawInGameMenu();
	int GetObjectInGameMenuIdClicked();
};

