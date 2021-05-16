#include "StdAfx.h"
#include "Camera.h"

Camera::Camera(void) : nearPlane(0.1f), farPlane(500.0f), fovPlane(1.0f)
{
	Init();
}

Camera::~Camera(void)
{
}
void Camera::Init(void)
{
	cameraMatrix.SetIdentity();
	speed = 1.0f;
	posW = Vector3( 0.0f, 2.0f, 0.5f);
	moveL = Vector4( 0.0f, 0.0f, 0.0f, 0.0f);
	rotL = Vector3( 0.0f, 0.0f, 0.0f);

	cameraMatrix.SetTranslation(posW);
	projectMatrix.SetPerspective( fovPlane, Globals::screenWidth/Globals::screenHeight, nearPlane, farPlane);
}

void Camera::Init(GLfloat i_near, GLfloat i_far, GLfloat i_fov, GLfloat i_speed)
{
	this->nearPlane = i_near;
	this->farPlane = i_far;
	this->fovPlane = i_fov;
	this->speed = i_speed;
	projectMatrix.SetPerspective( fovPlane, Globals::screenWidth/Globals::screenHeight, nearPlane, farPlane);
	Update();
}

void Camera::Free(void)
{
} 
Matrix Camera::GetCameraMatrix()
{
	return cameraMatrix;
}
Matrix Camera::GetProjectMatrix()
{
	return projectMatrix;
}
void Camera::MovementForward(GLfloat deltaTime)
{
	moveL = Vector4( 0.0f, 0.0f, -deltaTime * speed, 0.0f);
	Update();
}
void Camera::MovementBackward(GLfloat deltaTime)
{
	moveL = Vector4( 0.0f, 0.0f, deltaTime * speed, 0.0f);
	Update();
}
void Camera::MovementLeft(GLfloat deltaTime)
{
	moveL = Vector4(-deltaTime*speed, 0.0f, 0.0f, 0.0f);
	Update();
}
void Camera::MovementRight(GLfloat deltaTime)
{
	moveL = Vector4(deltaTime*speed, 0.0f, 0.0f, 0.0f);
	Update();
}
void Camera::RotationUp(GLfloat deltaTime)
{
	rotL.x += deltaTime * speed;
	Update();
}
void Camera::RotationUp(GLfloat deltaTime, GLfloat deltaRange)
{
	rotL.x += deltaTime * speed * deltaRange;
	Update();
}

void Camera::RotationDown(GLfloat deltaTime)
{
	rotL.x -= deltaTime * speed;
	Update();
}
void Camera::RotationDown(GLfloat deltaTime, GLfloat deltaRange)
{
	rotL.x -= deltaTime * speed * deltaRange;
	Update();
}

void Camera::RotationLeft(GLfloat deltaTime)
{
	rotL.y += deltaTime * speed;
	Update();
}
void Camera::RotationLeft(GLfloat deltaTime, GLfloat deltaRange)
{
	rotL.y += deltaTime * speed * deltaRange;
	Update();
}

void Camera::RotationRight(GLfloat deltaTime)
{
	rotL.y -= deltaTime * speed;
	Update();
}
void Camera::RotationRight(GLfloat deltaTime, GLfloat deltaRange)
{
	rotL.y -= deltaTime * speed * deltaRange;
	Update();
}
void Camera::SetRotationVector(Vector3 newRot)
{
	rotL = newRot;
}

void Camera::Update()
{
	Matrix rotXMatrix, rotYMatrix, rotZMatrix;
	Matrix translateMatrix;

	rotXMatrix.SetRotationX(rotL.x);
	rotYMatrix.SetRotationY(rotL.y);
	rotZMatrix.SetRotationZ(rotL.z);
	translateMatrix.SetTranslation(posW);

	Vector4 moveW = moveL * rotZMatrix * rotXMatrix * rotYMatrix * translateMatrix;

	posW += Vector3(moveW.x, moveW.y, moveW.z);

	Matrix translateMatrixInverse;
	Matrix rotXMatrixInverse, rotYMatrixInverse;

	translateMatrixInverse.SetTranslation(-posW);
	rotXMatrixInverse.SetRotationX(-rotL.x);
	rotYMatrixInverse.SetRotationY(-rotL.y);
	//rotZMatrixInverse.SetRotationZ(-rotL.z);

	cameraMatrix = translateMatrixInverse * rotYMatrixInverse * rotXMatrixInverse;

	moveL = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}
Vector3 Camera::GetPositionInWorld()
{
	return posW;
}
Vector3 Camera::GetRotationVector()
{
	return rotL;
}
