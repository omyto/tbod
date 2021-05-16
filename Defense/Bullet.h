#pragma once
#include "Object.h"
#include "BoxObject.h"

class Bullet : public Object
{
private:
	//float m_angleX;
	//float m_angleY;
	float m_speed;
	//float m_time;// thoi diem cua vien dan tinh tu luc ban
	Vector3 m_initPositon; //vi tri ban dau cua vien dan (vi tri khoi tao, chua chiu thay doi cua huong sung)
	Vector3 m_angle; //Huong sung khi ban -> huong vien dan di chuyen
	bool m_isDestroy;
public:
	Bullet();
	~Bullet();

	void Init(Vector3 angle);
	void Update(float deltaTime);
	float GetRangeMoving();
	bool IsDestroyBullet();
	bool IsCollisionWithObjectFrame(FrameModel* i_frM);
};

