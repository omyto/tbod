#include "stdafx.h"
#include "InGameState.h"
#include "GameDefineList.h"
#include "Object2DManager.h"
#include "MenuState.h"
#include "GamePlayManager.h"
#include "LevelCompleteState.h"
#include "GameOverState.h"
#include "FontGame.h"

InGameState::InGameState()
{
}


InGameState::~InGameState()
{
}

void InGameState::Init()
{
	printf("Init In Game state...\n");
	Globals::currentState = STATE_IN_GAME;	

	printf("Init SM\n");
	int level = GamePlayManager::GetInstance()->GetCurrentLevel();
	GamePlayManager::GetInstance()->InitHpAmmo();
	GamePlayManager::GetInstance()->CalculatorTotalEnemy();
	SceneManager::GetInstance()->Init("../Resources/_SM.txt", level);

	Globals::isEnableMouseControl = true;
	Globals::mouseMoveDeltaX = 0;
	Globals::mouseMoveDeltaY = 0;
	Globals::mouseTotalRangeX = 0;
	Globals::mouseTotalRangeY = 0;
	SetCursorPos(Globals::screenWidth / 2, Globals::screenHeight / 2);
	//SceneManager::GetInstance()->SetCamRotVec(Vector3(0.0f, 0.0f, 0.0f));
}

void InGameState::Update(float deltaTime)
{
	if (Globals::keyPressed & KEY_MOVEMENT_BACKWARD_MASK)
	{
		SceneManager::GetInstance()->MoveCamBackward(deltaTime);
	}
	if (Globals::keyPressed & KEY_MOVEMENT_FORWARD_MASK)
	{
		SceneManager::GetInstance()->MoveCamForward(deltaTime);
	}
	if (Globals::keyPressed & KEY_MOVEMENT_LEFT_MASK)
	{
		SceneManager::GetInstance()->MoveCamLeft(deltaTime);
	}
	if (Globals::keyPressed & KEY_MOVEMENT_RIGHT_MASK)
	{
		SceneManager::GetInstance()->MoveCamRight(deltaTime);
	}
	if (Globals::keyPressed & KEY_ROTATION_LEFT_MASK)
	{
		SceneManager::GetInstance()->RotCamLeft(deltaTime);
	}
	if (Globals::keyPressed & KEY_ROTATION_UP_MASK)
	{
		SceneManager::GetInstance()->RotCamUp(deltaTime);
	}
	if (Globals::keyPressed & KEY_ROTATION_RIGHT_MASK)
	{
		SceneManager::GetInstance()->RotCamRight(deltaTime);
	}
	if (Globals::keyPressed & KEY_ROTATION_DOWN_MASK)
	{
		SceneManager::GetInstance()->RotCamDown(deltaTime);
	}

	//---
	if (Globals::isKeyPressed && Globals::keyPress == ESC_KEY)
	{
		Globals::currentState = STATE_IN_GAME_MENU;
	}
	//--
	if (Globals::currentState == STATE_IN_GAME)
	{
		if (Globals::isEnableCursor == true)
		{
			ShowCursor(false);
			Globals::isEnableCursor = false;
		}
		if (Globals::mouseType == ES_MOUSE_MOVE_FUNC && Globals::isEnableMouseControl)
		{
			float deltaRangeX = 100.0f * Globals::mouseMoveDeltaX / Globals::screenWidth;
			float deltaRangeY = 100.0f * Globals::mouseMoveDeltaY / Globals::screenHeight;
			if (Globals::mouseMoveDeltaX > 0)
			{
				SceneManager::GetInstance()->RotScreenViewRight(deltaTime, deltaRangeX);
			}
			if (Globals::mouseMoveDeltaX < 0)
			{
				SceneManager::GetInstance()->RotScreenViewLeft(deltaTime, -deltaRangeX);
			}
			if (Globals::mouseMoveDeltaY > 0)
			{
				if (SceneManager::GetInstance()->GetCamRotVec().x > -0.4f)
				{
					SceneManager::GetInstance()->RotScreenViewDown(deltaTime, deltaRangeY);
				}
			}
			if (Globals::mouseMoveDeltaY < 0)
			{
				if (SceneManager::GetInstance()->GetCamRotVec().x < 0.7f)
				{
					SceneManager::GetInstance()->RotScreenViewUp(deltaTime, -deltaRangeY);
				}
			}
			Globals::mouseType = 0;
			Globals::mouseTotalRangeX += Globals::mouseMoveDeltaX;
			Globals::mouseTotalRangeY += Globals::mouseMoveDeltaY;
			if (abs(Globals::mouseTotalRangeX) > 350 || abs(Globals::mouseTotalRangeY) > 200 ||
				Globals::mousePointX < 10 || Globals::mousePointX > 950 ||
				Globals::mousePointY < 10 || Globals::mousePointY > 710)
			{
				SetCursorPos(Globals::screenWidth / 2, Globals::screenHeight / 2);
				Globals::mouseTotalRangeX = 0;
				Globals::mouseTotalRangeY = 0;
			}
		}
		SceneManager::GetInstance()->Update(deltaTime);
	}
	if (Globals::currentState == STATE_IN_GAME_MENU)
	{
		if (Globals::mouseType == ES_LMOUSE_DOWN_FUNC)
		{
			Globals::mouseType = 0;
			int objIdClicked = Object2DManager::GetInstance()->GetObjectInGameMenuIdClicked();
			switch (objIdClicked)
			{
			case IN_GAME_MENU_RESUME:
				printf("Click on Resume\n");
				Globals::currentState = STATE_IN_GAME;
				break;
			case IN_GAME_MENU_RESTART:
				printf("Click on Restart\n");
				StateManager::GetInstance()->SwitchState(new InGameState);
				break;
			case IN_GAME_MENU_MAIN_MENU:
				printf("Click on Main Menu\n");
				StateManager::GetInstance()->SwitchState(new MenuState);
				break;
			}
		}
	}
	if (GamePlayManager::GetInstance()->GetCurrentEnemy() == 0)
	{
		StateManager::GetInstance()->SwitchState(new LevelCompleteState);
	}
	if (GamePlayManager::GetInstance()->GetCurrentHP() <= 0)
	{
		StateManager::GetInstance()->SwitchState(new GameOverState);
	}
}

void InGameState::Draw()
{
	SceneManager::GetInstance()->Draw();
	if (Globals::currentState == STATE_IN_GAME_MENU)
	{
		Object2DManager::GetInstance()->DrawInGameMenu();
	}
	//
	Object2DManager::GetInstance()->Draw(LEVEL_GROUND);
	int lv = GamePlayManager::GetInstance()->GetCurrentLevel();
	FontGame::GetInstance()->DrawNumber(lv, 130, 25, 2, 1);


	Object2DManager::GetInstance()->Draw(BULLETS_GROUND);
	int bullet = GamePlayManager::GetInstance()->GetCurrentAmmo();
	FontGame::GetInstance()->DrawNumber(bullet, 135, 580, 2, 1);

	Object2DManager::GetInstance()->Draw(ENEMIES_GROUND);
	int enemi = GamePlayManager::GetInstance()->GetCurrentEnemy();
	FontGame::GetInstance()->DrawNumber(enemi, 895, 25, 2, 1);

	Object2DManager::GetInstance()->Draw(HP_GROUND);
	int hp = GamePlayManager::GetInstance()->GetCurrentHP();
	FontGame::GetInstance()->DrawNumber(hp, 125, 515, 2, 1);


}

void InGameState::Destroy()
{
	SceneManager::GetInstance()->CleanUp();
	SceneManager::FreeInstance();
}