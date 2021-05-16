#pragma once
#include "Object.h"

class EnemyBullet : public Object
{
private:
	float m_speed;
	//Vector3 m_initPos; //Vi tri cua robot tai thoi diem ban
	Vector3 m_direction; //Huong di chuyen cua dan
	bool m_isFire; //Dan co dc ban ra hay ko
	bool m_isFail; //Dan co ban trat hay khong
	float m_timeReload;
public:
	EnemyBullet();
	~EnemyBullet();

	void Init();
	void Fire(Vector3 pos, Vector3 target);
	void Update(float deltaTime);
	void Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld);

	void SetIsFire(bool status){ m_isFire = status; }
	bool CanFire(float deltaTime);
};

