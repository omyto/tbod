#pragma once
#include "GameState.h"

class StateManager : public GameState
{
private:
	static StateManager* m_instance;
	GameState *currentState;
public:
	StateManager();
	~StateManager();

	static StateManager* GetInstance();
	void Init();
	void Update(float deltaTime);
	void Draw();
	void Destroy();
	void SwitchState(GameState* state);
};

