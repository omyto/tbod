#include "stdafx.h"
#include "GameOverState.h"
#include "Object2DManager.h"
#include "GameDefineList.h"
#include "InGameState.h"
#include "MenuState.h"

GameOverState::GameOverState() : m_numOfObject(0)
{
	m_listObjectId = NULL;
}


GameOverState::~GameOverState()
{
}

void GameOverState::Init()
{
	printf("Init Game Over state...\n");
	Globals::currentState = STATE_GAME_OVER;

	m_numOfObject = 3;
	m_listObjectId = new int[m_numOfObject];
	//--
	m_listObjectId[0] = GAME_OVER_GROUND;
	m_listObjectId[1] = GAME_OVER_RESTART;
	m_listObjectId[2] = GAME_OVER_MAIN_MENU;
	//--
}

void GameOverState::Update(float deltaTime)
{
	if (Globals::mouseType == ES_LMOUSE_DOWN_FUNC)//Khi co su kien click chuot
	{
		Globals::mouseType = 0;
		int objClickedId = -1;
		for (int i = 1; i < m_numOfObject; i++)//Don't test first object, it's background
		{
			bool isClick = Object2DManager::GetInstance()->IsClickInObject(m_listObjectId[i]);
			if (isClick)
			{
				objClickedId = m_listObjectId[i];
				break;
			}
		}
		if (objClickedId != -1)
		{
			switch (objClickedId)
			{
			case GAME_OVER_RESTART:
				StateManager::GetInstance()->SwitchState(new InGameState);
				break;
			case GAME_OVER_MAIN_MENU:
				StateManager::GetInstance()->SwitchState(new MenuState);
				break;
			}
		}
	}
}

void GameOverState::Draw()
{
	for (int i = 0; i < m_numOfObject; i++)
	{
		Object2DManager::GetInstance()->Draw(m_listObjectId[i]);
	}
}
void GameOverState::Destroy()
{
	if (m_listObjectId != NULL)
	{
		delete[] m_listObjectId;
		m_listObjectId = NULL;
	}
}