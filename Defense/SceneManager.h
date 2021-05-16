#pragma once
#include "Object.h"
#include "ResourcesManager.h"
#include "Camera.h"
#include "Globals.h"
#include "Lights.h"
#include "PrimaryWeapon.h"
#include "BulletsManager.h"
#include "BoxObject.h"
#include "Robot.h"

class SceneManager
{
private:
	static SceneManager* m_instance;
	unsigned int m_numOfObject;
	Object* m_listObjects;
	Camera* m_camera;
	Lights* m_listLight;
	PrimaryWeapon* m_primaryWeapon;
	BulletsManager* m_bulletManager;
	BoxObject* m_boxObject;

	int m_numOfRobot;
	Robot* m_RobotList;

	Vector4 m_fogColor;
	GLfloat m_fogStart;
	GLfloat m_fogRange;
	
	GLfloat m_gameTime;
	
	GLuint m_numOfLights;
	Vector4 m_lightAmbientColor;
	GLfloat m_lightAmbientWeight;

	void InitRobotList();
	void UpdateRobotList(float deltaTime);
	void CreateNewBoxObject();

	//int m_maxRobotInScene;
	int m_numOfRobotDisplay;
	Vector3* m_posListRobotDisplay;
public:
	SceneManager(void);
	~SceneManager(void);
	static SceneManager* GetInstance();
	void Init(char* pFileTarget, int level = 1);
	void Update(float deltaTime);
	void Draw();
	void CleanUp();
	static void FreeInstance();
	void MoveCamForward(float deltaTime);
	void MoveCamBackward(float deltaTime);
	void MoveCamLeft(float deltaTime);
	void MoveCamRight(float deltaTime);

	void RotCamLeft(float deltaTime);
	void RotCamLeft(float deltaTime, float deltaRange);
	void RotCamRight(float deltaTime);
	void RotCamRight(float deltaTime, float deltaRange);
	void RotCamUp(float deltaTime);
	void RotCamUp(float deltaTime, float deltaRange);
	void RotCamDown(float deltaTime);
	void RotCamDown(float deltaTime, float deltaRange);
	void SetCamRotVec(Vector3 newRot);
	Vector3 GetCamRotVec();

	void RotScreenViewLeft(float deltaTime, float deltaRange);
	void RotScreenViewRight(float deltaTime, float deltaRange);
	void RotScreenViewUp(float deltaTime, float deltaRange);
	void RotScreenViewDown(float deltaTime, float deltaRange);

	Vector4 GetFogColor();
	GLfloat GetFogStart();
	GLfloat GetFogRange();
	GLfloat GetTotalGameTime();
	Vector4 GetAmbientColor();
	Lights& GetLightByID(GLuint i_ID);
	GLfloat GetAmbientWeight();
	GLuint GetNumOfLights();
	Object* GetObjectByID(GLuint i_ID);
};
