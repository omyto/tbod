#include "stdafx.h"
#include "MenuState.h"
#include "GameDefineList.h"
#include "GamePlayManager.h"
#include "HelpState.h"

MenuState::MenuState() : m_numOfObject(0)
{
	m_listObjectId = NULL;
}


MenuState::~MenuState()
{
}

void MenuState::Init()
{
	printf("Init Menu state...\n");
	Globals::currentState = STATE_MAIN_MENU;

	m_numOfObject = 5;
	m_listObjectId = new int[m_numOfObject];
	//--
	m_listObjectId[0] = MAIN_SCREEN_TEXTURE_2D;
	m_listObjectId[1] = MAIN_MENU_CONTINUE;
	m_listObjectId[2] = MAIN_MENU_NEW;
	m_listObjectId[3] = MAIN_MENU_HELP;
	m_listObjectId[4] = MAIN_MENU_AUTHOR;
	//--
}

void MenuState::Update(float deltaTime)
{
	//printf("Menu State update\n");
	if (Globals::mouseType == ES_LMOUSE_DOWN_FUNC)//Khi co su kien click chuot
	{
		Globals::mouseType = 0;
		int objClickedId = -1;
		for (int i = 1; i < m_numOfObject; i++)//Don't test first object, it's background
		{
			//bool isClick = IsClickInObject(Object2DManager::GetInstance()->GetPosSizeObject(m_listObjectId[i]));
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
			case MAIN_MENU_CONTINUE:
				printf("Click continue button\n");
				GamePlayManager::GetInstance()->ContinueGame();
				StateManager::GetInstance()->SwitchState(new InGameState);
				break;
			case MAIN_MENU_NEW:
				printf("Click new button\n");
				GamePlayManager::GetInstance()->NewGame();
				StateManager::GetInstance()->SwitchState(new InGameState);
				break;
			case MAIN_MENU_AUTHOR:
				printf("Click author button\n");
				StateManager::GetInstance()->SwitchState(new AuthorState);
				break;
			case MAIN_MENU_HELP:
				printf("Click help button\n");
				StateManager::GetInstance()->SwitchState(new HelpState);
				break;
			default:
				break;
			}
		}
	}
}

void MenuState::Draw()
{
	for (int i = 0; i < m_numOfObject; i++)
	{
		Object2DManager::GetInstance()->Draw(m_listObjectId[i]);
	}
}

void MenuState::Destroy()
{
	if (m_listObjectId != NULL)
	{
		delete[] m_listObjectId;
		m_listObjectId = NULL;
	}
}
//bool MenuState::IsClickInObject(Vector4 pos_size)
//{
//	int x1 = pos_size.x;
//	int y1 = pos_size.y;
//	int x2 = pos_size.x + pos_size.z;
//	int y2 = pos_size.y + pos_size.w;
//	if (Globals::mousePointX > x1 && Globals::mousePointX < x2 &&
//		Globals::mousePointY > y1 && Globals::mousePointY < y2)
//	{
//		return true;
//	}
//	return false;
//}