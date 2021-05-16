#include "stdafx.h"
#include "Object2DManager.h"
#include "Globals.h"
#include "GameDefineList.h"
Object2DManager* Object2DManager::m_instance = NULL;
Object2DManager::Object2DManager() : m_numOfObject2D(0), m_iboID(0), m_numOfIndices(0)
{
	m_shader = NULL;
	m_listObject2D = NULL;
}

Object2DManager::~Object2DManager()
{
}
Object2DManager* Object2DManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new Object2DManager();
	}
	return m_instance;
}

void Object2DManager::Init(char* pFileTarget)
{
	FILE* pFile = NULL;
	pFile = fopen(pFileTarget, "r");

	fscanf(pFile, "#Objects2D: %u\n", &m_numOfObject2D);
	m_listObject2D = new Object2DType[m_numOfObject2D];

	unsigned int objectID;
	unsigned int pos_x, pos_y, width, height;
	unsigned int textureId;

	for (unsigned int i = 0; i < m_numOfObject2D; i++)
	{
		fscanf(pFile, "\n");
		fscanf(pFile, "ID %u\n", &objectID);
		fscanf(pFile, "POS %u, %u\n", &pos_x, &pos_y);
		fscanf(pFile, "SIZE %u, %u\n", &width, &height);
		fscanf(pFile, "TEXTURE %u\n", &textureId);

		//--
		pos_x = (int)pos_x * Globals::screenWidth / Globals::SCREEN_W_DEFAULT;
		pos_y = (int)pos_y * Globals::screenHeight / Globals::SCREEN_H_DEFAULT;
		width = (int)width *  Globals::screenWidth / Globals::SCREEN_W_DEFAULT;
		height = (int)height * Globals::screenHeight / Globals::SCREEN_H_DEFAULT;
		//--

		m_listObject2D[objectID].vboId = CreateRectangle(pos_x, pos_y, width, height);
		m_listObject2D[objectID].textureId = ResourcesManager::GetInstance()->GetTexture2DByID(textureId)->GetTextureID();
		m_listObject2D[objectID].x = pos_x; m_listObject2D[objectID].y = pos_y;
		m_listObject2D[objectID].w = width; m_listObject2D[objectID].h = height;
	}

	//Indices Data
	m_numOfIndices = 6;
	GLuint indicesData[] = {
		0, 1, 2,
		0, 2, 3
	};
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//
	m_shader = ResourcesManager::GetInstance()->GetShaderByID(SHADER_2D_ID);
}

GLuint Object2DManager::CreateRectangle(unsigned int i_x, unsigned int i_y, unsigned int i_w, unsigned int i_h)
{
	Vector2 staPos = ScreenPosToPos(i_x, i_y);
	Vector2 endPos = ScreenPosToPos(i_x + i_w, i_y + i_h);

	//Vertices Buffer
	Vertex vertices[4];
	vertices[0].pos.x = staPos.x;	vertices[0].pos.y = staPos.y;	vertices[0].pos.z = 0.0f;	vertices[0].uv.x = 0.0f;	vertices[0].uv.y = 1.0f;
	vertices[1].pos.x = staPos.x;	vertices[1].pos.y = endPos.y;	vertices[1].pos.z = 0.0f;	vertices[1].uv.x = 0.0f;	vertices[1].uv.y = 0.0f;
	vertices[2].pos.x = endPos.x;	vertices[2].pos.y = endPos.y;	vertices[2].pos.z = 0.0f;	vertices[2].uv.x = 1.0f;	vertices[2].uv.y = 0.0f;
	vertices[3].pos.x = endPos.x;	vertices[3].pos.y = staPos.y;	vertices[3].pos.z = 0.0f;	vertices[3].uv.x = 1.0f;	vertices[3].uv.y = 1.0f;

	GLuint m_vboID;
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return m_vboID;
}

Vector2 Object2DManager::ScreenPosToPos(unsigned int Sx, unsigned int Sy)
{
	float SCREEN_W = Globals::screenWidth;
	float SCREEN_H = Globals::screenHeight;
	float Px =  1.0f * Sx / (SCREEN_W / 2.0f) - 1.0f;
	float Py = -1.0f * Sy / (SCREEN_H / 2.0f) + 1.0f;
	return Vector2(Px, Py);
}

void Object2DManager::Draw(int objectId)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, m_listObject2D[objectId].vboId);
	glUseProgram(m_shader->program);

	//Position
	if (m_shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(m_shader->positionAttribute);
		glVertexAttribPointer(m_shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	}
	//UV
	if (m_shader->uvLocation != -1)
	{
		glEnableVertexAttribArray(m_shader->uvLocation);
		glVertexAttribPointer(m_shader->uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}
	//Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_listObject2D[objectId].textureId);
	if (m_shader->uListTexturesLoc[0] != -1)
	{
		glUniform1i(m_shader->uListTexturesLoc[0], 0);
	}
	//Draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glDrawElements(GL_TRIANGLES, m_numOfIndices, GL_UNSIGNED_INT, 0);

	//Bind buffer to default
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}

void Object2DManager::CleanUp()
{
	if (m_listObject2D != NULL)
	{
		for (unsigned int i = 0; i < m_numOfObject2D; i++)
		{
			glDeleteBuffers(1, &m_listObject2D[i].vboId);
		}
		delete [] m_listObject2D;
		m_listObject2D = NULL;
	}
	glDeleteBuffers(1, &m_iboID);
}

void Object2DManager::FreeInstance()
{
	if (m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

//Vector4 Object2DManager::GetPosSizeObject(int objID)
//{
//	Object2DType obj = m_listObject2D[objID];
//	return Vector4(obj.x, obj.y, obj.w, obj.h);
//}

bool Object2DManager::IsClickInObject(int objID)
{
	int x1 = m_listObject2D[objID].x;
	int x2 = m_listObject2D[objID].x + m_listObject2D[objID].w;
	int y1 = m_listObject2D[objID].y;
	int y2 = m_listObject2D[objID].y + m_listObject2D[objID].h;

	if (Globals::mousePointX > x1 && Globals::mousePointX < x2 &&
		Globals::mousePointY > y1 && Globals::mousePointY < y2)
	{
		return true;
	}
	return false;
}

void Object2DManager::DrawInGameMenu()
{
	Draw(IN_GAME_MENU_GROUND);
	Draw(IN_GAME_MENU_RESUME);
	Draw(IN_GAME_MENU_RESTART);
	Draw(IN_GAME_MENU_MAIN_MENU);
}

int Object2DManager::GetObjectInGameMenuIdClicked()
{
	//if (IsClickInObject(IN_GAME_MENU_GROUND)) return IN_GAME_MENU_GROUND;
	if (IsClickInObject(IN_GAME_MENU_RESUME)) return IN_GAME_MENU_RESUME;
	if (IsClickInObject(IN_GAME_MENU_RESTART)) return IN_GAME_MENU_RESTART;
	if (IsClickInObject(IN_GAME_MENU_MAIN_MENU)) return IN_GAME_MENU_MAIN_MENU;
	return -1;
}