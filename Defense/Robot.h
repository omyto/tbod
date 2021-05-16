#pragma once
#include "Object.h"
#include "EnemyBullet.h"

class Robot : public Object
{
private:
	bool m_isDisplay;
	bool m_isDestroy;
	EnemyBullet* m_bullet;
public:
	Robot();
	~Robot();
	void Init();
	void Update(float deltaTime);
	void Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld);
	void CleanUp();
	void SetIsDisplay(bool i) { m_isDisplay = i; }
	bool GetIsDisplay(){ return m_isDisplay; }
	bool GetIsDestroy() { return m_isDestroy; }
	void SetIsDestroy(bool i){ m_isDestroy = i; }
};

