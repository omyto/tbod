#include "stdafx.h"
#include "BoxObject.h"
#include "ResourcesManager.h"
#include "GameDefineList.h"

BoxObject::BoxObject()
{
	m_isShow = false;
	m_timeToHaveBox = 2.0f;
}


BoxObject::~BoxObject()
{
}

void BoxObject::Init()
{
	m_Model = ResourcesManager::GetInstance()->GetModelByID(BOX_MODEL_ID);
	p_frM = ResourcesManager::GetInstance()->GetFrameModelByID(BOX_MODEL_ID);

	m_numOfTexture2D = 1;
	m_numOfTexture3D = 0;
	m_numOfLights = 0;

	m_listTexture2D = new Texture*[1];
	m_listTexture2D[0] = NULL;
	m_healthTexture = ResourcesManager::GetInstance()->GetTexture2DByID(HEALTH_BOX_TEXTURE_ID);
	m_ammoTexture = ResourcesManager::GetInstance()->GetTexture2DByID(AMMO_BOX_TEXTURE_ID);
	
	m_shader = ResourcesManager::GetInstance()->GetShaderByID(SHADER_3D_ID);

	m_rotation = Vector3(0.785f, 0.785f, 0.785f);
	m_scale = Vector3(0.1f, 0.1f, 0.1f);
}

void BoxObject::Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld)
{
	if (m_isShow)
	{
		Object::Draw(cameraMatrix, projectMatrix, posCamWorld);
	}
}
void BoxObject::Update(float deltaTime)
{
	if (m_isShow)
	{
		m_rotation.x += deltaTime;
		m_rotation.y += deltaTime;
		m_rotation.z += deltaTime;
		m_isChangeWorldMatrix = true;
		Object::Update(deltaTime);
	}
	else
	{
		m_timeToHaveBox -= 0.5f * deltaTime;
	}
}

bool BoxObject::IsColisionBullet(Vector3 posBullet)
{
	if (!m_isShow) return false;
	UpdateFrameModel();
	if (posBullet.x < m_frameModel.max_x && posBullet.x > m_frameModel.min_x &&
		posBullet.y < m_frameModel.max_y && posBullet.y > m_frameModel.min_y &&
		posBullet.z < m_frameModel.max_z && posBullet.z > m_frameModel.min_z)
	{
		return true;
	}
	return false;
}

void BoxObject::SetType(int type)
{
	m_type = type;
	if (type == HEALTH_BOX)
	{
		m_listTexture2D[0] = m_healthTexture;
	}
	else
	{
		m_listTexture2D[0] = m_ammoTexture;
	}
}

void BoxObject::SetLocation(Vector3 i_pos)
{
	m_position = i_pos;
}

bool BoxObject::CanHaveBox()
{
	if (m_timeToHaveBox < 0.0f && m_isShow == false)
	{
		m_timeToHaveBox = 2.0f;
		return true;
	}
	return false;
}