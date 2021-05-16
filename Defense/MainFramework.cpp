// Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stddef.h>
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "ResourcesManager.h"
#include "SceneManager.h"
#include "EffectManager.h"
#include "LoadingState.h"
#include "GameDefineList.h"
#include "Object2DManager.h"
#include "GamePlayManager.h"
#include "FontGame.h"


int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//Switch to Loading State
	StateManager::GetInstance()->SwitchState(new LoadingState);

	return 0;
}

void Draw ( ESContext *esContext )
{
	//Chuan bi cho hieu ung
	if (Globals::effect != EFFECT_NONE)
	{
		EffectManager::GetInstance()->BindFBOByID(0);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//------------ Draw ----------------
	StateManager::GetInstance()->Draw();

	//Draw from FBO to Screen buffer
	if (Globals::effect != EFFECT_NONE)
	{
		EffectManager::GetInstance()->UnbindFBOByID(0);
		EffectManager::GetInstance()->Draw(Globals::effect);
	}
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	if (Globals::currentState != STATE_IN_GAME && Globals::isEnableCursor == false)
	{
			ShowCursor(true);
			Globals::isEnableCursor = true;
			SetCursorPos(Globals::screenWidth / 2, Globals::screenHeight / 2);
			Globals::mouseTotalRangeX = 0;
			Globals::mouseTotalRangeY = 0;
	}
	StateManager::GetInstance()->Update(deltaTime);

	/* ------------------ Not at here --------------
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

	SceneManager::GetInstance()->Update(deltaTime);

	*/
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed)
	{
		Globals::keyPress = key;
		Globals::isKeyPressed = true;
		printf("Key press: %d\n", key);
		switch (key)
		{
		case KEY_MOVEMENT_FORWARD:
			Globals::keyPressed |= KEY_MOVEMENT_FORWARD_MASK;
			break;
		case KEY_MOVEMENT_BACKWARD:
			Globals::keyPressed |= KEY_MOVEMENT_BACKWARD_MASK;
			break;
		case KEY_MOVEMENT_LEFT:
			Globals::keyPressed |= KEY_MOVEMENT_LEFT_MASK;
			break;
		case KEY_MOVEMENT_RIGHT:
			Globals::keyPressed |= KEY_MOVEMENT_RIGHT_MASK;
			break;
		case KEY_ROTATION_LEFT:
			Globals::keyPressed |= KEY_ROTATION_LEFT_MASK;
			break;
		case KEY_ROTATION_UP:
			Globals::keyPressed |= KEY_ROTATION_UP_MASK;
			break;
		case KEY_ROTATION_RIGHT:
			Globals::keyPressed |= KEY_ROTATION_RIGHT_MASK;
			break;
		case KEY_ROTATION_DOWN:
			Globals::keyPressed |= KEY_ROTATION_DOWN_MASK;
			break;
		
		case KEY_EFFECT_NONE:
			Globals::effect = EFFECT_NONE;
			break;
		case KEY_EFFECT_BW:
			Globals::effect = EFFECT_BW;
			break;
		case KEY_EFFECT_BLUR:
			Globals::effect = EFFECT_BLUR;
			break;
		case KEY_EFFECT_BLOOM:
			Globals::effect = EFFECT_BLOOM;
			break;
		case KEY_EFFECT_DOF:
			Globals::effect = EFFECT_DOF;
			break;
		case KEY_EFFECT_EDGE_DETECTION:
			Globals::effect = EFFECT_EDGE_DETECTION;
			break;
		}
	}
	else
	{
		Globals::keyPress = 0;
		Globals::isKeyPressed = false;
		switch (key)
		{
		case KEY_MOVEMENT_FORWARD:
			Globals::keyPressed &= ~KEY_MOVEMENT_FORWARD_MASK;
			break;
		case KEY_MOVEMENT_BACKWARD:
			Globals::keyPressed &= ~KEY_MOVEMENT_BACKWARD_MASK;
			break;
		case KEY_MOVEMENT_LEFT:
			Globals::keyPressed &= ~KEY_MOVEMENT_LEFT_MASK;
			break;
		case KEY_MOVEMENT_RIGHT:
			Globals::keyPressed &= ~KEY_MOVEMENT_RIGHT_MASK;
			break;
		case KEY_ROTATION_LEFT:
			Globals::keyPressed &= ~KEY_ROTATION_LEFT_MASK;
			break;
		case KEY_ROTATION_UP:
			Globals::keyPressed &= ~KEY_ROTATION_UP_MASK;
			break;
		case KEY_ROTATION_RIGHT:
			Globals::keyPressed &= ~KEY_ROTATION_RIGHT_MASK;
			break;
		case KEY_ROTATION_DOWN:
			Globals::keyPressed &= ~KEY_ROTATION_DOWN_MASK;
			break;
		}
	}
}

void Mouse(ESContext *esContext, int x, int y, unsigned int mouseType)
{
	//printf("Mouse - x: %d, y: %d, mouseType: %u\n", x, y, mouseType);
	Globals::mouseType = mouseType;
	switch (mouseType)
	{
	case ES_LMOUSE_DOWN_FUNC:
		//if (Globals::isShowLogMousePointer)
			printf("Left Mouse down - x: %d, y: %d\n", x, y);
		Globals::mousePointX = x;
		Globals::mousePointY = y;
		break;
	case ES_LMOUSE_UP_FUNC:
		//if (Globals::isShowLogMousePointer)
			printf("Left Mouse up - x: %d, y: %d\n", x, y);
		Globals::mousePointX = x;
		Globals::mousePointY = y;
		break;
	case ES_RMOUSE_DOWN_FUNC:
		//if (Globals::isShowLogMousePointer)
			printf("Right Mouse down - x: %d, y: %d\n", x, y);
		Globals::mousePointX = x;
		Globals::mousePointY = y;
		break;
	case ES_RMOUSE_UP_FUNC:
		//if (Globals::isShowLogMousePointer)
			printf("Right Mouse up - x: %d, y: %d\n", x, y);
		Globals::mousePointX = x;
		Globals::mousePointY = y;
		break;
	case ES_MOUSE_MOVE_FUNC:
		if (Globals::isShowLogMousePointer)
			printf("Mouse move - x: %d, y: %d\n", x, y);
		if (Globals::currentState == STATE_IN_GAME)
		{
			Globals::mouseMoveDeltaX = (Globals::mousePointX == 0) ? 0 : x - Globals::mousePointX;
			Globals::mouseMoveDeltaY = (Globals::mousePointY == 0) ? 0 : y - Globals::mousePointY;
		}
		Globals::mousePointX = x;
		Globals::mousePointY = y;
		break;
	}
}

void CleanUp()
{
	ResourcesManager::GetInstance()->CleanUp();
	EffectManager::GetInstance()->CleanUp();
	EffectManager::FreeInstance();
	StateManager::GetInstance()->Destroy();
	Object2DManager::GetInstance()->CleanUp();
	Object2DManager::FreeInstance();
	GamePlayManager::GetInstance()->CleanUp();
	GamePlayManager::FreeInstance();
	FontGame::GetInstance()->CleanUp();
	FontGame::FreeInstance();
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Defense 3D", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc ( &esContext, Mouse );

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

