#pragma once
#include "GameState.h"
#include "StateManager.h"

class HelpState : public GameState
{
private:
	int m_objectId;
public:
	HelpState();
	~HelpState();

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Destroy();
};

