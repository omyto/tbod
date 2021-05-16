#pragma once
#include "stdafx.h"
#include "StateManager.h"

StateManager* StateManager::m_instance = NULL;
StateManager::StateManager()
{
	currentState = NULL;
}


StateManager::~StateManager()
{
}

StateManager* StateManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new StateManager();
	}
	return m_instance;
}

void StateManager::SwitchState(GameState* state)
{
	if (currentState != NULL)
	{
		currentState->Destroy();
		delete currentState;
		currentState = 0;
	}
	currentState = state;
	currentState->Init();
}
void StateManager::Init()
{
	//Don't need init because it's singleton class
}

void StateManager::Update(float deltaTime)
{
	if (currentState != NULL)
	{
		currentState->Update(deltaTime);
	}
}

void StateManager::Draw()
{
	if (currentState != NULL)
	{
		currentState->Draw();
	}
}

void StateManager::Destroy()
{
	if (currentState != NULL)
	{
		currentState->Destroy();
		delete currentState;
		currentState = 0;
	}
	if (m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}