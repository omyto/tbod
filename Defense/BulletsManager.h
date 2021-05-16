#pragma once
#include "Bullet.h"

struct BulletNodeStruct
{
	Bullet data;
	BulletNodeStruct* next;
};
typedef BulletNodeStruct* BulletNodePointer;

class BulletsManager
{
private:
	int m_numOfBullet;
	BulletNodePointer p_bulletList;
	float m_timeLoadBullet;
	bool m_isNextBulletReady;
	const float TIME_TO_LOAD_BULLET;
	const float MAX_RANGE_MOVING;
public:
	BulletsManager();
	~BulletsManager();
	void Init();
	void Update(float deltaTime);
	void Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld);
	void CleanUp();
	void CreateBullet(Vector3 angle);
	//--- Bullet Node
	void CreateNewBulletNode(Vector3 angle);
	void UpdateBulletList(float deltaTime);
	void DrawBulletList(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld);
	//---
	bool IsCollisionWithObjectFrame(FrameModel* i_frM);
};

