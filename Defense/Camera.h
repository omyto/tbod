#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Globals.h"
class Camera
{
private:
	Matrix cameraMatrix;
	Matrix projectMatrix;
	GLfloat speed;
	Vector3 posW;
	Vector4 moveL;
	Vector3 rotL;
	GLfloat fovPlane;
	GLfloat nearPlane;
	GLfloat farPlane;
public:
	Camera(void);
	~Camera(void);
	void Init();
	void Init(GLfloat i_near, GLfloat i_far, GLfloat i_fov, GLfloat i_speed);
	void Free();
	Matrix GetCameraMatrix();
	Matrix GetProjectMatrix();
	Vector3 GetPositionInWorld();
	Vector3 GetRotationVector();
	void MovementForward(GLfloat deltaTime);
	void MovementBackward(GLfloat deltaTime);
	void MovementLeft(GLfloat deltaTime);
	void MovementRight(GLfloat deltaTime);
	void RotationUp(GLfloat deltaTime);
	void RotationUp(GLfloat deltaTime, GLfloat deltaRange);
	void RotationDown(GLfloat deltaTime);
	void RotationDown(GLfloat deltaTime, GLfloat deltaRange);
	void RotationLeft(GLfloat deltaTime);
	void RotationLeft(GLfloat deltaTime, GLfloat deltaRange);
	void RotationRight(GLfloat deltaTime);
	void RotationRight(GLfloat deltaTime, GLfloat deltaRange);
	void Update();
	void SetRotationVector(Vector3 newRot);
};
