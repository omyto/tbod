#pragma once
#include "GameState.h"
#include "StateManager.h"
class LoadingState : public GameState
{
private:
	Shaders* m_shader;
	Texture* m_texture;
	GLuint m_vboID;
	GLuint m_iboID;
	GLuint m_numOfIndices;

	bool isLoadCompleted;
	float loadingTimes;
public:
	LoadingState();
	~LoadingState();

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Destroy();
};

