#include "stdafx.h"
#include <cmath>
#include "Bullet.h"
#include "GameDefineList.h"
#include "ResourcesManager.h"

Bullet::Bullet()
{
	printf("Creat Bullet\n");
	//m_angleX = 0.0f;
	//m_angleY = 0.0f;
	m_speed = 4.0f;
	//m_time = 0.0f;
	m_initPositon = Vector3(0.0f, 1.75f, 0.5f);
	m_angle = Vector3(0.0f, 0.0f, 0.0f);
	m_isDestroy = false;
}


Bullet::~Bullet()
{
}

void Bullet::Init(Vector3 angle)
{
	printf("Init Bullet\n");
	//---------------------
	m_Model = ResourcesManager::GetInstance()->GetModelByID(BULLET_MODEL_ID);
	p_frM = ResourcesManager::GetInstance()->GetFrameModelByID(BULLET_MODEL_ID);

	m_numOfTexture2D = 1;
	m_numOfTexture3D = 0;
	m_numOfLights = 0;

	m_listTexture2D = new Texture*[m_numOfTexture2D];
	m_listTexture2D[0] = ResourcesManager::GetInstance()->GetTexture2DByID(BULLET_TEXTURE_ID);
	m_shader = ResourcesManager::GetInstance()->GetShaderByID(SHADER_3D_ID);

	m_scale = Vector3(0.0002f, 0.0002f, 0.0002f);
	m_angle = angle;// rotL camera
	printf("init Bullet - m_angle: %f, %f, %f\n", m_angle.x, m_angle.y, m_angle.z);
	//m_position = m_initPositon;
	//m_rotation
	
	//m_position.x = m_initPositon.x + sin(-m_angle.y);
	//m_position.y = m_initPositon.y + sin(m_angle.x);
	//m_position.z = m_initPositon.z + cos(m_angle.x) * cos(m_angle.y);

	Matrix rotXMatrix, rotYMatrix, rotZMatrix;
	Matrix translateMatrix;

	rotXMatrix.SetRotationX(m_angle.x);
	rotYMatrix.SetRotationY(m_angle.y);
	rotZMatrix.SetRotationZ(m_angle.z);

	m_position = m_initPositon;

	translateMatrix.SetTranslation(m_position);

	Vector4 moveW = Vector4(0.0f, 0.0f, -1.0f, 0.0f) * rotZMatrix * rotXMatrix * rotYMatrix * translateMatrix;

	m_position += Vector3(moveW.x, moveW.y, moveW.z);

	printf("init Bullet - m_position: %f, %f, %f\n", m_position.x, m_position.y, m_position.z);
}

void Bullet::Update(float deltaTime)
{
	//--------------------
	Matrix rotXMatrix, rotYMatrix, rotZMatrix;
	Matrix translateMatrix;

	rotXMatrix.SetRotationX(m_angle.x);
	rotYMatrix.SetRotationY(m_angle.y);
	rotZMatrix.SetRotationZ(m_angle.z);

	//m_position = m_initPositon;

	translateMatrix.SetTranslation(m_position);
	float deltaRange = 0.0f;
	if (deltaTime < 1.0f) deltaRange = deltaTime;
	Vector4 moveW = Vector4(0.0f, 0.0f, -0.5f * deltaRange * m_speed, 0.0f) * rotZMatrix * rotXMatrix * rotYMatrix * translateMatrix;

	m_position += Vector3(moveW.x, moveW.y, moveW.z);
	
	m_isChangeWorldMatrix = true;
	//--------------------
	Object::Update(deltaTime);
}

float Bullet::GetRangeMoving()
{
	if (abs(m_position.z) > abs(m_position.x) && abs(m_position.z) > abs(m_position.y))
	{
		return abs(m_position.z);
	}
	else if (abs(m_position.y) > abs(m_position.x))
	{
		return abs(m_position.y);
	}
	else
	{
		return abs(m_position.x);
	}
}

bool Bullet::IsDestroyBullet()
{
	return m_isDestroy;
}

bool Bullet::IsCollisionWithObjectFrame(FrameModel* i_frM)
{
	if (m_position.x < (*i_frM).max_x && m_position.x >(*i_frM).min_x &&
		m_position.y < (*i_frM).max_y && m_position.y >(*i_frM).min_y &&
		m_position.z < (*i_frM).max_z && m_position.z >(*i_frM).min_z)
	{
		m_isDestroy = true;
		return true;
	}
	return false;
}