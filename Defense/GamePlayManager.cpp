#include "stdafx.h"
#include <string.h>
#include "GamePlayManager.h"

GamePlayManager* GamePlayManager::m_instance = NULL;
GamePlayManager::GamePlayManager() : m_continueLevel(0), m_currentLevel(0), m_currentEnemy(0)//,m_totalEnemy(0)
{
	m_pSaveFile = NULL;
}


GamePlayManager::~GamePlayManager()
{
}

GamePlayManager* GamePlayManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new GamePlayManager();
	}
	return m_instance;
}

void GamePlayManager::Init(char* pFileTarget)
{
	m_pSaveFile = new char[strlen(pFileTarget) + 1];
	strcpy(m_pSaveFile, pFileTarget);
	Load();
}

void GamePlayManager::Load()
{
	FILE* pFile = NULL;
	pFile = fopen(m_pSaveFile, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "#Continue Level: %d\n", &m_continueLevel);

		fclose(pFile);

		if (m_continueLevel < 1) m_continueLevel = 1;
		if (m_continueLevel > MAX_LEVEL) m_continueLevel = MAX_LEVEL;
	}
	else
	{
		m_continueLevel = 1;
	}
	m_currentLevel = 1;
}

void GamePlayManager::Save()
{
	FILE* pFile = NULL;
	pFile = fopen(m_pSaveFile, "w");
	if (pFile != NULL)
	{
		fprintf(pFile, "#Continue Level: %d\n", m_continueLevel);

		fclose(pFile);
	}
}

void GamePlayManager::CleanUp()
{
	if (m_pSaveFile != NULL)
	{
		delete[] m_pSaveFile;
		m_pSaveFile = NULL;
	}
}

void GamePlayManager::FreeInstance()
{
	if (m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void GamePlayManager::ContinueGame()
{
	m_currentLevel = m_continueLevel;
	//m_HP = 100;
	//m_Ammo = 10 * m_currentLevel;
	m_Score = m_SaveScore;
}
void GamePlayManager::NewGame()
{
	m_currentLevel = 1;
	m_continueLevel = 1;
	//m_HP = 100;
	//m_Ammo = 10 * m_currentLevel;
	m_Score = 0;
	Save();
}

void GamePlayManager::IncreaseLevel()
{
	if (m_currentLevel < MAX_LEVEL) m_currentLevel++;
	m_continueLevel = m_currentLevel;
	Save();
}

void GamePlayManager::CalculatorTotalEnemy()
{
	m_currentEnemy = GetNumOfRobot();
}

void GamePlayManager::CompleteLevel()
{
	m_SaveScore = m_Score;
}

void GamePlayManager::InitHpAmmo()
{
	m_HP = 100;
	m_Ammo = 10 * m_currentLevel;
}