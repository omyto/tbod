#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Object.h"
enum LightType{
	LIGHT_TYPE_DIRECTION,
	LIGHT_TYPE_POINT,
	LIGHT_MOVING_NONE,
	LIGHT_MOVING_CIRCLE
};
class Lights
{
private:
	GLuint m_ID;
	Vector3 m_vectorPosDir;
	Vector3 m_posW;
	GLenum m_type;
	GLenum m_movingType;
	Vector4 m_color;
	GLfloat m_radius;
	GLfloat m_speed;
	GLfloat m_angle;
	Object* m_object;
public:
	Lights(void);
	~Lights(void);
	void Init(GLuint i_ID, Vector3 i_posDir, GLenum i_type, Vector4 i_color, GLenum i_movType, GLfloat i_radius, GLfloat i_speed, GLuint i_objID);
	void Update(float deltaTime);
	GLuint GetID();
	GLenum GetType();
	Vector3 GetPosition();
	Vector3 GetDirection();
	Vector4 GetColor();
	GLenum GetMovingType();
	GLfloat GetRadius();
	GLfloat GetSpeed();
	bool GetIsDirection();
};
