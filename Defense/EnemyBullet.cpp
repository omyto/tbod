#include "stdafx.h"
#include "EnemyBullet.h"
#include "ResourcesManager.h"
#include "GameDefineList.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "GamePlayManager.h"

EnemyBullet::EnemyBullet()
{
	m_speed = 2.0f;
	m_isFail = true;
	m_isFire = false;
	m_timeReload = 0.0f;
}


EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Init()
{
	printf("Init Enemy Bullet\n");
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

	//srand((unsigned)time(NULL));
	m_timeReload = 2.0f + rand() % 3;//Thoi gian de kiem tra co nen ban ko trong khoang 2 - 4 don vi
}

void EnemyBullet::Update(float deltaTime)
{
	if (m_isFire)
	{
		m_position = m_position + m_direction * m_speed * deltaTime;
		m_isChangeWorldMatrix = true;
		Object::Update(deltaTime);

		if (m_position.z > 0.5f)//Camera position in Oz
		{
			if (m_isFail)
			{
				printf("Trat roi babe\n");
			}
			else
			{
				printf("Trung mia roi\n");
				GamePlayManager::GetInstance()->DecreaseHP();
			}
			m_isFire = false;
		}
	}
}

void EnemyBullet::Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld)
{
	if (m_isFire)
	{
		Object::Draw(cameraMatrix, projectMatrix, posCamWorld);
	}
}

void EnemyBullet::Fire(Vector3 pos, Vector3 target)
{
	m_isFire = true;
	Vector3 deltaRange = target - Vector3(0.0f, 2.0f, 0.5f);
	if (abs(deltaRange.x) < 0.2f && abs(deltaRange.y) < 0.2f && abs(deltaRange.z) < 0.2f)
	{
		m_isFail = false;
	}
	else
	{
		m_isFail = true;
	}
	//m_initPos = pos;//?
	m_position = pos;
	m_direction = target - pos;
	m_direction = m_direction / 3.5f;
}

bool EnemyBullet::CanFire(float deltaTime)
{
	if (m_isFire == false)
	{
		if (m_timeReload <= 0.0f)
		{
			//srand((unsigned)time(NULL));
			m_timeReload = 2.0f + rand() % 3;
			int canFire = rand() % (GamePlayManager::GetInstance()->GetNumOfRobot() + 10);
			if (canFire == 1)
			{
				return true;
			}
			return false;
		}
		m_timeReload -= 0.25f;
	}
	return false;
}