#include "stdafx.h"
#include "HelpState.h"
#include "GameDefineList.h"
#include "Object2DManager.h"
#include "MenuState.h"

HelpState::HelpState()
{
}


HelpState::~HelpState()
{
}


void HelpState::Init()
{
	printf("Init Author state...\n");
	Globals::currentState = STATE_HELP;

	m_objectId = HELP_OBJECT_2D;
}
void HelpState::Update(float deltaTime)
{
	if (Globals::mouseType == ES_LMOUSE_DOWN_FUNC)//Khi co su kien click chuot
	{
		Globals::mouseType = 0;
		StateManager::GetInstance()->SwitchState(new MenuState);
	}
}
void HelpState::Draw()
{
	Object2DManager::GetInstance()->Draw(m_objectId);
}
void HelpState::Destroy()
{

}