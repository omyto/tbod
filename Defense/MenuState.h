#pragma once
#include "GameState.h"
#include "StateManager.h"
#include "InGameState.h"
#include "AuthorState.h"
#include "Object2DManager.h"

class MenuState : public GameState
{
private:
	int m_numOfObject;
	int* m_listObjectId;
public:
	MenuState();
	~MenuState();

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Destroy();
	//bool IsClickInObject(Vector4 pos_size);
};

