#pragma once
#include "StdAfx.h"
#include "SceneManager.h"

class GameState
{
public:
	virtual void Init() = 0;
	virtual void Update(float) = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;
};