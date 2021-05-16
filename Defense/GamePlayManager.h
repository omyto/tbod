#pragma once

#define MAX_LEVEL 15
#define NUM_ROBOT_INCREASE_BY_LEVEL 2

class GamePlayManager
{
private:
	static GamePlayManager* m_instance;
	int m_continueLevel;
	int m_currentLevel;
	//int m_totalEnemy;
	int m_currentEnemy;
	int m_HP;
	int m_Ammo;
	int m_Score;
	int m_SaveScore;
	char* m_pSaveFile;
public:
	GamePlayManager();
	~GamePlayManager();

	static GamePlayManager* GetInstance();
	static void FreeInstance();

	void Init(char* pFileTarget);
	void Load();
	void Save();
	void CleanUp();

	int GetContinueLevel() { return m_continueLevel; }
	int GetCurrentLevel() { return m_currentLevel; }
	int GetNumOfRobot(){ return m_currentLevel * NUM_ROBOT_INCREASE_BY_LEVEL; }
	int GetCurrentEnemy() { return m_currentEnemy; }
	
	void DecreaseCurrentEnemy(){ m_currentEnemy--; }
	void CalculatorTotalEnemy();

	void IncreaseHP(){ m_HP += 50; }
	void IncreaseAmmo(){ m_Ammo += 20; }
	void IncreaseScore(){ m_Score += 10; }

	void DecreaseHP(){ m_HP -= 10; }
	void DecreaseAmmo(){ m_Ammo--; }

	int GetCurrentHP(){ return m_HP; }
	int GetCurrentAmmo() { return m_Ammo; }
	int GetCurrentScore() { return m_Score; }

	void ContinueGame();
	void NewGame();
	void IncreaseLevel();
	void CompleteLevel();
	void InitHpAmmo();
};

