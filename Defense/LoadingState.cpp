#include "stdafx.h"
#include "LoadingState.h"
#include "GameDefineList.h"
#include "InGameState.h"
#include "EffectManager.h"
#include "Object2DManager.h"
#include "MenuState.h"
#include "GamePlayManager.h"
#include "FontGame.h"

LoadingState::LoadingState()
{
	m_shader = NULL;
	m_texture = NULL;
	m_vboID = 0;
	m_iboID = 0;
	m_numOfIndices = 6;

	isLoadCompleted = false;
	loadingTimes = 5.0f;
}


LoadingState::~LoadingState()
{
}

void LoadingState::Init()
{
	printf("Init Loading state...\n");
	Globals::currentState = STATE_LOADING;

	//Vertices Buffer
	Vertex vertices[4];
	vertices[0].pos.x = -0.75f;	vertices[0].pos.y = 0.75f;	vertices[0].pos.z = 0.0f;	vertices[0].uv.x = 0.0f;	vertices[0].uv.y = 1.0f;
	vertices[1].pos.x = -0.75f;	vertices[1].pos.y = -0.75f;	vertices[1].pos.z = 0.0f;	vertices[1].uv.x = 0.0f;	vertices[1].uv.y = 0.0f;
	vertices[2].pos.x = 0.75f;	vertices[2].pos.y = -0.75f;	vertices[2].pos.z = 0.0f;	vertices[2].uv.x = 1.0f;	vertices[2].uv.y = 0.0f;
	vertices[3].pos.x = 0.75f;	vertices[3].pos.y = 0.75f;	vertices[3].pos.z = 0.0f;	vertices[3].uv.x = 1.0f;	vertices[3].uv.y = 1.0f;
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Indices Data
	GLuint indicesData[] = {
		0, 1, 2,
		0, 2, 3
	};
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Texture
	m_texture = new Texture();
	m_texture->Init2DTextures("../Resources/Textures/loading.tga");

	//Shader
	m_shader = new Shaders();
	m_shader->Init("../Resources/Shaders/Shader2D.vs", "../Resources/Shaders/Shader2D.fs");
}

void LoadingState::Update(float deltaTime)
{
	if (loadingTimes > 0.0f)
	{
		loadingTimes -= 0.5f;
	}
	else
	{
		if (!isLoadCompleted)
		{
			//Init Resources Manager
			printf("Init RM\n");
			ResourcesManager::GetInstance()->Init("../Resources/_RM.txt");

			//Init Effect Manager
			printf("Init FM\n");
			EffectManager::GetInstance()->Load("../Resources/EM.txt");

			//Init 2D Object
			printf("Init 2D Objects\n");
			Object2DManager::GetInstance()->Init("../Resources/Object2D.txt");

			//Init Game play manager
			printf("Init Game Play Manager\n");
			GamePlayManager::GetInstance()->Init("../Resources/Save.txt");

			//Font Game
			printf("Init Font\n");
			FontGame::GetInstance()->Init();

			isLoadCompleted = true;
			printf("Complete init RM, FM & 2D Objects\n");
		}
	}
	//
	if (isLoadCompleted)
	{
		//if (Globals::isKeyPressed)
		{
			StateManager::GetInstance()->SwitchState(new MenuState);
		}
	}
}

void LoadingState::Draw()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
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
	glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureID());
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
}

void LoadingState::Destroy()
{
	if (m_shader != NULL)
	{
		delete m_shader;
	}
	if (m_texture != NULL)
	{
		delete m_texture;
	}
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_iboID);
}