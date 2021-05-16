#include "stdafx.h"
#include "Robot.h"
#include "GameDefineList.h"
#include "ResourcesManager.h"

Robot::Robot()
{
	m_isDisplay = false;
	m_isDestroy = false;
	m_bullet = NULL;
}


Robot::~Robot()
{
}

void Robot::Init()
{
	m_Model = ResourcesManager::GetInstance()->GetModelByID(ROBOT_MODEL_ID);
	p_frM = ResourcesManager::GetInstance()->GetFrameModelByID(ROBOT_MODEL_ID);

	m_numOfTexture2D = 1;
	m_numOfTexture3D = 0;
	m_numOfLights = 0;

	m_listTexture2D = new Texture*[m_numOfTexture2D];
	m_listTexture2D[0] = ResourcesManager::GetInstance()->GetTexture2DByID(ROBOT_TEXTURE_ID);

	m_shader = ResourcesManager::GetInstance()->GetShaderByID(SHADER_3D_ID);

	m_scale = Vector3(0.002f, 0.002f, 0.002f);
	//m_position = pos;

	//--
	m_bullet = new EnemyBullet();
	m_bullet->Init();
}

void Robot::Update(float deltaTime)
{
	if (m_isDisplay)
	{
		Object::Update(deltaTime);
		if (m_bullet->CanFire(deltaTime))
		{
			m_bullet->Fire(m_position, Vector3(0.0f, 2.0f, 0.5f));
		}
		m_bullet->Update(deltaTime);
	}
}

void Robot::Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld)
{
	Object::Draw(cameraMatrix, projectMatrix, posCamWorld);
	m_bullet->Draw(cameraMatrix, projectMatrix, posCamWorld);
}

void Robot::CleanUp()
{
	if (m_bullet != NULL)
	{
		m_bullet->CleanUp();
		delete m_bullet;
		m_bullet = NULL;
	}
	Object::CleanUp();
}
