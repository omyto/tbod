#pragma once
#include "GameState.h"
#include "StateManager.h"

class InGameState : public GameState
{
private:
public:
	InGameState();
	~InGameState();

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Destroy();
};

