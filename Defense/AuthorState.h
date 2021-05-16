#pragma once
#include "GameState.h"
#include "StateManager.h"
#include "Object2DManager.h"
class AuthorState : public GameState
{
private:
	int m_objectId;
public:
	AuthorState();
	~AuthorState();
	void Init();
	void Update(float deltaTime);
	void Draw();
	void Destroy();
};

