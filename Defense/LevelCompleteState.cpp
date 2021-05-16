#include "stdafx.h"
#include "LevelCompleteState.h"
#include "GameDefineList.h"
#include "Object2DManager.h"
#include "GamePlayManager.h"
#include "MenuState.h"
#include "InGameState.h"

LevelCompleteState::LevelCompleteState() : m_numOfObject(0)
{
	m_listObjectId = NULL;
}


LevelCompleteState::~LevelCompleteState()
{

}

void LevelCompleteState::Init()
{
	printf("Init Level Complete state...\n");
	Globals::currentState = STATE_LEVEL_COMPLETE;

	m_numOfObject = 4;
	m_listObjectId = new int[m_numOfObject];
	//--
	m_listObjectId[0] = LEVEL_COMPLETE_GROUND;
	m_listObjectId[1] = LEVEL_COMPLETE_RESTART;
	m_listObjectId[2] = LEVEL_COMPLETE_MAIN_MENU;
	m_listObjectId[3] = LEVEL_COMPLETE_NEXT_LEVEL;
	//--
	//GamePlayManager::GetInstance()->CompleteLevel();
}

void LevelCompleteState::Update(float deltaTime)
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
			case LEVEL_COMPLETE_RESTART:
				printf("Click level restart\n");
				StateManager::GetInstance()->SwitchState(new InGameState);
				break;
			case LEVEL_COMPLETE_MAIN_MENU:
				printf("Click main menu\n");
				GamePlayManager::GetInstance()->CompleteLevel();
				GamePlayManager::GetInstance()->IncreaseLevel();
				StateManager::GetInstance()->SwitchState(new MenuState);
				break;
			case LEVEL_COMPLETE_NEXT_LEVEL:
				printf("Click next level\n");
				GamePlayManager::GetInstance()->CompleteLevel();
				GamePlayManager::GetInstance()->IncreaseLevel();
				StateManager::GetInstance()->SwitchState(new InGameState);
				break;
			}
		}
	}
}

void LevelCompleteState::Draw()
{
	for (int i = 0; i < m_numOfObject; i++)
	{
		Object2DManager::GetInstance()->Draw(m_listObjectId[i]);
	}
}

void LevelCompleteState::Destroy()
{
	if (m_listObjectId != NULL)
	{
		delete[] m_listObjectId;
		m_listObjectId = NULL;
	}
}