#include "stdafx.h"
#include "PrimaryWeapon.h"
#include "GameDefineList.h"
#include "ResourcesManager.h"

PrimaryWeapon::PrimaryWeapon()
{
	m_speedRotation = 1.0f;
}


PrimaryWeapon::~PrimaryWeapon()
{
}

void PrimaryWeapon::Init()
{
	printf("Init Primary Weapon Object\n");
	//-------------------------------------
	m_Model = ResourcesManager::GetInstance()->GetModelByID(PRIMARY_WEAPON_MODEL_ID);
	p_frM = ResourcesManager::GetInstance()->GetFrameModelByID(PRIMARY_WEAPON_MODEL_ID);

	m_numOfTexture2D = 1;
	m_numOfTexture3D = 0;
	m_numOfLights = 0;

	m_listTexture2D = new Texture*[1];
	m_listTexture2D[0] = ResourcesManager::GetInstance()->GetTexture2DByID(PRIMARY_WEAPON_TEXTURE_ID);
	m_shader = ResourcesManager::GetInstance()->GetShaderByID(SHADER_3D_ID);

	m_rotation = Vector3(-1.52f, -3.14f, 0.0f);
	m_position = Vector3(0.0f, 1.75f, 0.5f);
	m_scale = Vector3(0.01f, 0.01f, 0.01f);
	//m_isChangeWorldMatrix = true; //It's set true when creat new object
}

void PrimaryWeapon::SetRotationSpeed(float i_speed)
{
	m_speedRotation = i_speed;
}

void PrimaryWeapon::RotFollowCameraUp(float deltaTime, float deltaRange)
{
	m_rotation.x -= deltaTime * deltaRange * m_speedRotation;
	//m_position.x -= deltaTime * deltaRange;
	m_isChangeWorldMatrix = true;
}
void PrimaryWeapon::RotFollowCameraDown(float deltaTime, float deltaRange)
{
	m_rotation.x += deltaTime * deltaRange * m_speedRotation;
	//m_position.x += deltaTime * deltaRange;
	m_isChangeWorldMatrix = true;
}
void PrimaryWeapon::RotFollowCameraLeft(float deltaTime, float deltaRange)
{
	m_rotation.y += deltaTime * deltaRange * m_speedRotation;
	//m_position.y += deltaTime * deltaRange;
	m_isChangeWorldMatrix = true;
}
void PrimaryWeapon::RotFollowCameraRight(float deltaTime, float deltaRange)
{
	m_rotation.y -= deltaTime * deltaRange * m_speedRotation;
	//m_position.y -= deltaTime * deltaRange;
	m_isChangeWorldMatrix = true;
}
