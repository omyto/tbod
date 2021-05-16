#pragma once
#include "GameState.h"
#include "StateManager.h"

class GameOverState : public GameState
{
private:
	int m_numOfObject;
	int* m_listObjectId;
public:
	GameOverState();
	~GameOverState();

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Destroy();
};

