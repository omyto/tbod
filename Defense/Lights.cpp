#include "StdAfx.h"
#include "Lights.h"
#include "SceneManager.h"
Lights::Lights(void) : m_angle(0.0f)
{
}

Lights::~Lights(void)
{
}
void Lights::Init(GLuint i_ID, Vector3 i_posDir, GLenum i_type, Vector4 i_color, GLenum i_movType, GLfloat i_radius, GLfloat i_speed, GLuint i_objID)
{
	m_ID = i_ID;
	m_vectorPosDir = i_posDir;
	m_posW = i_posDir;
	m_type = i_type;
	m_color = i_color;
	m_movingType = i_movType;
	m_radius = i_radius;
	m_speed = i_speed;
	m_object = SceneManager::GetInstance()->GetObjectByID(i_objID);
}
void Lights::Update(float deltaTime)
{
	if (m_movingType == LIGHT_MOVING_CIRCLE)
	{
		m_angle += deltaTime * m_speed;
		Matrix translateMatrix, rotationMatrix, newPosMatrix;
		translateMatrix.SetTranslation(m_radius, 0.0, 0.0);
		rotationMatrix.SetRotationY(m_angle);
		newPosMatrix = translateMatrix * rotationMatrix;
		
		Vector4 newPosW = Vector4(1.0, 1.0, 1.0, 1.0) * newPosMatrix;
		m_vectorPosDir = Vector3(newPosW.x, newPosW.y, newPosW.z) + m_posW;
	}
	if ( m_type == LIGHT_TYPE_POINT)
	{
		m_object->SetPosition(m_vectorPosDir);
	}

}
GLuint Lights::GetID()
{
	return m_ID;
}
GLenum Lights::GetType()
{
	return m_type;
}
Vector3 Lights::GetPosition()
{
	return m_vectorPosDir;
}
Vector3 Lights::GetDirection()
{
	return m_vectorPosDir;
}
Vector4 Lights::GetColor()
{
	return m_color;
}
GLenum Lights::GetMovingType()
{
	return m_movingType;
}
GLfloat Lights::GetRadius()
{
	return m_radius;
}
GLfloat Lights::GetSpeed()
{
	return m_speed;
}
bool Lights::GetIsDirection()
{
	if (m_type == LIGHT_TYPE_DIRECTION)
	{
		return true;
	}
	return false;
}