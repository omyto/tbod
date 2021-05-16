#pragma once
#include "GameState.h"
#include "StateManager.h"

class LevelCompleteState : public GameState
{
private:
	int m_numOfObject;
	int* m_listObjectId;
public:
	LevelCompleteState();
	~LevelCompleteState();

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Destroy();
};

