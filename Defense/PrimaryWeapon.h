#pragma once
#include "Object.h"

class PrimaryWeapon : public Object
{
private:
	float m_speedRotation;
public:
	void Init();
	void SetRotationSpeed(float i_speed);
	void RotFollowCameraUp(float deltaTime, float deltaRange);
	void RotFollowCameraDown(float deltaTime, float deltaRange);
	void RotFollowCameraLeft(float deltaTime, float deltaRange);
	void RotFollowCameraRight(float deltaTime, float deltaRange);

	PrimaryWeapon();
	~PrimaryWeapon();
};

