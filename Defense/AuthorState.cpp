#include "stdafx.h"
#include "AuthorState.h"
#include "GameDefineList.h"
#include "MenuState.h"
#include "FontGame.h"

AuthorState::AuthorState() : m_objectId(0)
{
}


AuthorState::~AuthorState()
{
}

void AuthorState::Init()
{
	printf("Init Author state...\n");
	Globals::currentState = STATE_AUTHOR;

	m_objectId = MAIN_SCREEN_TEXTURE_2D;
}

void AuthorState::Update(float deltaTime)
{
	if (Globals::mouseType == ES_LMOUSE_DOWN_FUNC)//Khi co su kien click chuot
	{
		Globals::mouseType = 0;
		StateManager::GetInstance()->SwitchState(new MenuState);
	}
}

void AuthorState::Draw()
{
	Object2DManager::GetInstance()->Draw(m_objectId);
	// FontGame::GetInstance()->DrawString("DEVELOP AND DESIGN BY OMYTO", 200, 350, 2.5f, 2);
	FontGame::GetInstance()->DrawString("OMYTO", 400, 410, 2.5f, 1);
	// FontGame::GetInstance()->DrawString("SPECIAL THANKS TO TS NGUYEN TAN KHOI", 180, 470, 2.5f, 2);
}

void AuthorState::Destroy()
{

}