#include "stdafx.h"
#include "BulletsManager.h"
#include "GamePlayManager.h"

BulletsManager::BulletsManager() : TIME_TO_LOAD_BULLET(0.5f), MAX_RANGE_MOVING(15.0f)
{
	printf("Create Bullets Manager\n");
	m_numOfBullet = 0;
	p_bulletList = NULL;
	m_timeLoadBullet = 0.0f;
	m_isNextBulletReady = true;
}


BulletsManager::~BulletsManager()
{
}

void BulletsManager::Init()
{
	printf("Init Bullets Manager\n");
}
void BulletsManager::Update(float deltaTime)
{
	if (m_isNextBulletReady == false)
	{
		m_timeLoadBullet += deltaTime;
		if (m_timeLoadBullet >= TIME_TO_LOAD_BULLET)
		{
			m_timeLoadBullet = 0.0f;
			m_isNextBulletReady = true;
		}
	}
	UpdateBulletList(deltaTime);
}
void BulletsManager::Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld)
{
	DrawBulletList(cameraMatrix, projectMatrix, posCamWorld);
}
void BulletsManager::CreateBullet(Vector3 angle)
{
	if (m_isNextBulletReady)
	{
		printf("Create Bullet\n");
		CreateNewBulletNode(angle);
		m_isNextBulletReady = false;
		GamePlayManager::GetInstance()->DecreaseAmmo();
	}
}
void BulletsManager::CleanUp()
{
	if (p_bulletList != NULL)
	{
		BulletNodePointer p;
		p = p_bulletList;
		while (p != NULL)
		{
			BulletNodePointer tmp = p;
			p->data.CleanUp();
			p = p->next;
			delete tmp;
		}
	}
	//delete p_bulletList;
	p_bulletList = NULL;
}

//--- Bullet Node

void BulletsManager::CreateNewBulletNode(Vector3 angle)
{
	if (p_bulletList == NULL)
	{
		p_bulletList = new BulletNodeStruct;
		p_bulletList->data.Init(angle);
		p_bulletList->next = NULL;
	}
	else
	{
		BulletNodePointer p_tmp = new BulletNodeStruct;
		p_tmp->data.Init(angle);
		p_tmp->next = p_bulletList;
		p_bulletList = p_tmp;
	}
	m_numOfBullet++;
}
void BulletsManager::UpdateBulletList(float deltaTime)
{
	BulletNodePointer p, before_p;
	int i = 0;
	p = p_bulletList;
	before_p = p;
	while (p != NULL)
	{
		//printf("Update Bullet: %d\n", i++);
		p->data.Update(deltaTime);
		if (p->data.GetRangeMoving() > MAX_RANGE_MOVING || p->data.IsDestroyBullet())
		{
			if (p == p_bulletList)
			{
				BulletNodePointer q = p;
				p_bulletList = p_bulletList->next;
				p = p_bulletList;
				before_p = p;
				q->data.CleanUp();
				delete q;
			}
			else
			{
				BulletNodePointer q = p;
				p = p->next;
				before_p->next = p;	
				q->data.CleanUp();
				delete q;
			}
			m_numOfBullet--;
		}
		else
		{
			before_p = p;
			p = p->next;
		}
	}
}

void BulletsManager::DrawBulletList(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld)
{
	BulletNodePointer p;
	int i = 0;
	p = p_bulletList;
	while (p != NULL)
	{
		//printf("Draw Bullet: %d\n", i++);
		p->data.Draw(cameraMatrix, projectMatrix, posCamWorld);
		p = p->next;
	}
}

bool BulletsManager::IsCollisionWithObjectFrame(FrameModel* i_frM)
{
	BulletNodePointer p;
	int i = 0;
	p = p_bulletList;
	while (p != NULL)
	{
		if (p->data.IsCollisionWithObjectFrame(i_frM) == true)
		{
			return true;
		}
		p = p->next;
	}
	return false;
}