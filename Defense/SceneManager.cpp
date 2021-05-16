#include "StdAfx.h"
#include "SceneManager.h"
#include "GameDefineList.h"
#include "GamePlayManager.h"
#include <stdlib.h>
#include <time.h>

SceneManager* SceneManager::m_instance = NULL;
SceneManager::SceneManager(void) : m_numOfObject(0), m_gameTime(0.0f), m_numOfLights(0), m_numOfRobot(0)
{
	m_listObjects = NULL;
	m_camera = NULL;
	m_listLight = NULL;
	m_primaryWeapon = NULL;
	m_bulletManager = NULL;
	m_boxObject = NULL;
	m_RobotList = NULL;
	m_numOfRobotDisplay = 0;
	m_posListRobotDisplay = NULL;
}

SceneManager::~SceneManager(void)
{
}

SceneManager* SceneManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new SceneManager(); 
	}
	return m_instance;
}
void SceneManager::Init(char* pFileTarget, int level)
{
	FILE* pFile = NULL;
	pFile = fopen(pFileTarget, "r");

	fscanf(pFile, "#Objects: %u\n", &m_numOfObject);
	m_listObjects = new Object[m_numOfObject];

	unsigned int m_objectID = -1;
	unsigned int m_modelID = -1;
	unsigned int m_shaderID = -1;
	unsigned int m_texture2DID = -1;
	unsigned int m_texture3DID = -1;
	unsigned int m_numOfTexture2D = 0;
	unsigned int m_numOfTexture3D = 0;
	unsigned int numOfLights = 0;
	unsigned int* listLightID = NULL;
	//Model* m_model = NULL;
	//Shaders* m_shader = NULL;
	Texture** m_listTexture2D = NULL;
	Texture** m_listTexture3D = NULL;
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_sacle;

	for(unsigned int i = 0 ; i < m_numOfObject ; i++)
	{
		fscanf(pFile, "\n");
		fscanf(pFile, "ID %u\n", &m_objectID);

		fscanf(pFile, "MODEL %u\n", &m_modelID);
		m_listObjects[m_objectID].SetModel( ResourcesManager::GetInstance()->GetModelByID(m_modelID) );
		//Add set frame model
		m_listObjects[m_objectID].SetFrameModel(ResourcesManager::GetInstance()->GetFrameModelByID(m_modelID));
		//end add

		fscanf(pFile, "TEXTURES %u\n", &m_numOfTexture2D);
		m_listTexture2D = new Texture*[m_numOfTexture2D];
		for(unsigned int countTexture2D = 0; countTexture2D < m_numOfTexture2D; countTexture2D++)
		{
			fscanf(pFile, "TEXTURE %u\n", &m_texture2DID);
			m_listTexture2D[countTexture2D] = ResourcesManager::GetInstance()->GetTexture2DByID(m_texture2DID);
		}
		m_listObjects[m_objectID].SetTexture2D( m_numOfTexture2D, m_listTexture2D );

		fscanf(pFile, "CUBETEXTURES %u\n", &m_numOfTexture3D);
		m_listTexture3D = new Texture*[m_numOfTexture3D];
		for(unsigned int countTexture3D = 0; countTexture3D < m_numOfTexture3D; countTexture3D++)
		{
			fscanf(pFile, "CUBETEX %u\n", &m_texture3DID);
			m_listTexture3D[countTexture3D] = ResourcesManager::GetInstance()->GetTexture3DByID(m_texture3DID);
		}
		m_listObjects[m_objectID].SetTexture3D( m_numOfTexture3D, m_listTexture3D );

		fscanf(pFile, "SHADER %u\n", &m_shaderID);
		m_listObjects[m_objectID].SetShader( ResourcesManager::GetInstance()->GetShaderByID(m_shaderID) );

		fscanf(pFile, "LIGHTS %u\n", &numOfLights);
		listLightID = new unsigned int[numOfLights];
		for(unsigned int countLight = 0; countLight < numOfLights; countLight++)
		{
			fscanf(pFile, "LIGHT %u\n", &listLightID[countLight]);
		}
		m_listObjects[m_objectID].SetLights(numOfLights, listLightID);
		fscanf(pFile, "POSITION %f, %f, %f\n", &m_position.x, &m_position.y, &m_position.z);
		m_listObjects[m_objectID].SetPosition( m_position );
		fscanf(pFile, "ROTATION %f, %f, %f\n", &m_rotation.x, &m_rotation.y, &m_rotation.z);
		m_listObjects[m_objectID].SetRotation( m_rotation );
		fscanf(pFile, "SCALE %f, %f, %f\n", &m_sacle.x, &m_sacle.y, &m_sacle.z);
		m_listObjects[m_objectID].SetScale( m_sacle );
		delete [] listLightID;
	}

	//Lights
	fscanf(pFile, "#LIGHTS\n");
	fscanf(pFile, "AmbientColor %f, %f, %f, %f\n", &m_lightAmbientColor.x, &m_lightAmbientColor.y, &m_lightAmbientColor.z, &m_lightAmbientColor.w);
	fscanf(pFile, "AmbientWeight %f\n", &m_lightAmbientWeight);
	fscanf(pFile, "LightsCount: %u\n", &m_numOfLights);
	m_listLight = new Lights[m_numOfLights];
	GLuint lightID = -1;
	Vector3 lightPosDir;
	char lightTypeStr[30];
	GLenum lightType;
	Vector4 lightColor;
	char lightMovTypeStr[30];
	GLenum lightMovType;
	GLfloat lightRadius;
	GLfloat lightSpeed;
	GLuint lightObjectID;
	for(unsigned int iLight = 0 ; iLight < m_numOfLights ; iLight++)
	{
		fscanf(pFile, "\n");
		fscanf(pFile, "ID %u\n", &lightID);
		fscanf(pFile, "POS/DIR %f, %f, %f\n", &lightPosDir.x, &lightPosDir.y, &lightPosDir.z);
		fscanf(pFile, "TYPE %s\n", lightTypeStr);
		if ( strcmp(lightTypeStr, "DIRECTION") == 0 )
		{
			lightType = LIGHT_TYPE_DIRECTION;
		}
		if ( strcmp(lightTypeStr, "POINT") == 0 )
		{
			lightType = LIGHT_TYPE_POINT;
		}
		fscanf(pFile, "COLOR %f, %f, %f, %f\n", &lightColor.x, &lightColor.y, &lightColor.z, &lightColor.w);
		fscanf(pFile, "MOVING %s\n", lightMovTypeStr);
		if ( strcmp(lightMovTypeStr, "NONE")==0 )
		{
			lightMovType = LIGHT_MOVING_NONE;
		}
		if ( strcmp(lightMovTypeStr, "CIRCLE")==0 )
		{
			lightMovType = LIGHT_MOVING_CIRCLE;
		}
		fscanf(pFile, "RADIUS %f\n", &lightRadius);
		fscanf(pFile, "SPEED %f\n", &lightSpeed);
		fscanf(pFile, "OBJECT %u\n", &lightObjectID);
		m_listLight[iLight].Init(lightID, lightPosDir, lightType, lightColor, lightMovType, lightRadius, lightSpeed, lightObjectID);
	}

	fscanf(pFile, "\n");

	//Camera
	GLfloat m_near;
	GLfloat m_far;
	GLfloat m_fov;
	GLfloat m_speed;
	fscanf(pFile, "#CAMERA\n");
	fscanf(pFile, "NEAR %f\n", &m_near);
	fscanf(pFile, "FAR %f\n", &m_far);
	fscanf(pFile, "FOV %f\n", &m_fov);
	fscanf(pFile, "SPEED %f\n", &m_speed);
	m_camera = new Camera();
	m_camera->Init(m_near, m_far, m_fov, m_speed);

	fscanf(pFile, "\n");
	//Fog
	fscanf(pFile, "#FOG\n");
	fscanf(pFile, "COLOR %f, %f, %f, %f\n", &m_fogColor.x, &m_fogColor.y, &m_fogColor.z, &m_fogColor.w);
	fscanf(pFile, "START %f\n", &m_fogStart);
	fscanf(pFile, "RANGE %f\n", &m_fogRange);

	fclose(pFile);

	//--- Init In Game Object
	srand((unsigned)time(NULL));
	printf("Init in game object\n");
	m_primaryWeapon = new PrimaryWeapon();
	m_primaryWeapon->Init();
	m_primaryWeapon->SetRotationSpeed(m_speed);

	m_bulletManager = new BulletsManager();
	m_bulletManager->Init();

	m_boxObject = new BoxObject();
	m_boxObject->Init();
	//m_boxObject->SetType(HEALTH_BOX);
	//m_boxObject->SetLocation(Vector3(0.0f, 2.0f, -2.5f));
	//m_boxObject->SetIsDisplay(true);

	InitRobotList();
	
	//delete pFileLevel;
}

void SceneManager::Update(float deltaTime)
{
	//--- Update screen
	m_gameTime += deltaTime;
	Vector3 rot[3];
	rot[0] = m_listObjects[0].GetRotation();

	//rot[0].y += deltaTime;

	for( unsigned int i = 0 ; i < m_numOfObject; i++)
	{
		if( i ==  0) 
		{
			m_listObjects[i].SetRotation(rot[i]);
		}
		m_listObjects[i].Update(deltaTime);
	}

	for(unsigned int iLightUnit = 0 ; iLightUnit < m_numOfLights ; iLightUnit++)
	{
		if(iLightUnit % 2)
		{
			m_listLight[iLightUnit].Update(deltaTime);
		}
		else
		{
			m_listLight[iLightUnit].Update(-deltaTime);
		}
	}

	//--- Create Bullet
	if (Globals::mouseType == ES_LMOUSE_DOWN_FUNC && GamePlayManager::GetInstance()->GetCurrentAmmo() > 0)
	{
		m_bulletManager->CreateBullet(m_camera->GetRotationVector());
	}

	//--- Update In Game Object
	m_primaryWeapon->Update(deltaTime);
	m_bulletManager->Update(deltaTime);
	m_boxObject->Update(deltaTime);
	UpdateRobotList(deltaTime);

	//-- Create box object
	if (m_boxObject->CanHaveBox())
	{
		CreateNewBoxObject();
	}

	//-- Update frame model - only using for box, robot, warship, ...
	m_boxObject->UpdateFrameModel();
	for (int i = 0; i < m_numOfRobot; i++)
	{
		m_RobotList[i].UpdateFrameModel();
	}

	//-- Xu ly va cham
	//** Va cham giua bullet va box
	if (m_boxObject->GetIsDisplay())
	{
		bool isBoxCollisionWithBullet = m_bulletManager->IsCollisionWithObjectFrame(m_boxObject->GetFrameModel());
		if (isBoxCollisionWithBullet)
		{
			printf("Box Collisionnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n");
			//m_boxObject->SetIsDisplay(false);
			//if (m_boxObject->GetType() == HEALTH_BOX) m_boxObject->SetType(AMMO_BOX);
			//else m_boxObject->SetType(HEALTH_BOX);
			if (m_boxObject->GetType() == HEALTH_BOX)
			{
				GamePlayManager::GetInstance()->IncreaseHP();
			}
			else
			{
				GamePlayManager::GetInstance()->IncreaseAmmo();
			}
			m_boxObject->SetIsDisplay(false);
		}
	}
	//** Va cham giua robot va bullet
	for (int i = 0; i < m_numOfRobot; i++)
	{
		if (m_RobotList[i].GetIsDisplay())
		{
			bool isCollision = m_bulletManager->IsCollisionWithObjectFrame(m_RobotList[i].GetFrameModel());
			if (isCollision)
			{
				printf("Robot Collisionnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n");
				m_RobotList[i].SetIsDisplay(false);
				m_RobotList[i].SetIsDestroy(true);
				GamePlayManager::GetInstance()->DecreaseCurrentEnemy();
			}
		}
	}
}

void SceneManager::Draw()
{
	//--- Draw In Game Screen
	//for( unsigned int i = 0 ; i < m_numOfObject ; i++)
	//{
	//	if( i == 9)//Fire
	//	{
	//		glDepthMask(GL_FALSE);
	//	}
	//	if (i != 1)// Model 1 now is set primary weapon, split in diffrence object
	//	m_listObjects[i].Draw( m_camera->GetCameraMatrix(), m_camera->GetProjectMatrix(), m_camera->GetPositionInWorld());

	//	if( i == 9)
	//	{
	//		glDepthMask(GL_TRUE);
	//	}
	//}
	m_listObjects[3].Draw(m_camera->GetCameraMatrix(), m_camera->GetProjectMatrix(), m_camera->GetPositionInWorld());
	m_listObjects[4].Draw(m_camera->GetCameraMatrix(), m_camera->GetProjectMatrix(), m_camera->GetPositionInWorld());

	//--- Draw In Game Object
	m_primaryWeapon->Draw(m_camera->GetCameraMatrix(), m_camera->GetProjectMatrix(), m_camera->GetPositionInWorld());
	m_bulletManager->Draw(m_camera->GetCameraMatrix(), m_camera->GetProjectMatrix(), m_camera->GetPositionInWorld());
	m_boxObject->Draw(m_camera->GetCameraMatrix(), m_camera->GetProjectMatrix(), m_camera->GetPositionInWorld());
	for (int i = 0; i < m_numOfRobot; i++)
	{
		if (m_RobotList[i].GetIsDisplay())
		{
			m_RobotList[i].Draw(m_camera->GetCameraMatrix(), m_camera->GetProjectMatrix(), m_camera->GetPositionInWorld());
		}
	}
}

void SceneManager::CleanUp()
{
	if( m_listObjects != NULL)
	{
		for(unsigned int i = 0 ; i < m_numOfObject ; i++)
		{
			m_listObjects[i].CleanUp();
		}
		delete [] m_listObjects;
		m_listObjects = NULL;
	}
	if (m_camera != NULL)
	{
		delete m_camera;
		m_camera = NULL;
	}
	if (m_listLight != NULL)
	{
		delete [] m_listLight;
		m_listLight = NULL;
	}
	if (m_primaryWeapon != NULL)
	{
		m_primaryWeapon->CleanUp();
		delete m_primaryWeapon;
		m_primaryWeapon = NULL;
	}
	if (m_bulletManager != NULL)
	{
		m_bulletManager->CleanUp();
		delete m_bulletManager;
		m_bulletManager = NULL;
	}
	if (m_boxObject != NULL)
	{
		m_boxObject->CleanUp();
		delete m_boxObject;
		m_boxObject = NULL;
	}
	if (m_RobotList != NULL)
	{
		for (int i = 0; i < m_numOfRobot; i++)
		{
			m_RobotList[i].CleanUp();
		}
		delete[] m_RobotList;
		m_RobotList = NULL;
	}
	if (m_posListRobotDisplay != NULL)
	{
		delete [] m_posListRobotDisplay;
		m_posListRobotDisplay = NULL;
	}
}

void SceneManager::FreeInstance()
{
	if (m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void SceneManager::MoveCamForward(float deltaTime)
{
	m_camera->MovementForward(deltaTime);
}
void SceneManager::MoveCamBackward(float deltaTime)
{
	m_camera->MovementBackward(deltaTime);
}
void SceneManager::MoveCamLeft(float deltaTime)
{
	m_camera->MovementLeft(deltaTime);
}
void SceneManager::MoveCamRight(float deltaTime)
{
	m_camera->MovementRight(deltaTime);
}
void SceneManager::RotCamLeft(float deltaTime)
{
	m_camera->RotationLeft(deltaTime);
}
void SceneManager::RotCamLeft(float deltaTime, float deltaRange)
{
	m_camera->RotationLeft(deltaTime, deltaRange);
}

void SceneManager::RotCamRight(float deltaTime)
{
	m_camera->RotationRight(deltaTime);
}
void SceneManager::RotCamRight(float deltaTime, float deltaRange)
{
	m_camera->RotationRight(deltaTime, deltaRange);
}

void SceneManager::RotCamUp(float deltaTime)
{
	m_camera->RotationUp(deltaTime);
}
void SceneManager::RotCamUp(float deltaTime, float deltaRange)
{
	m_camera->RotationUp(deltaTime, deltaRange);
}

void SceneManager::RotCamDown(float deltaTime)
{
	m_camera->RotationDown(deltaTime);
}
void SceneManager::RotCamDown(float deltaTime, float deltaRange)
{
	m_camera->RotationDown(deltaTime, deltaRange);
}
void SceneManager::SetCamRotVec(Vector3 newRot)
{
	m_camera->SetRotationVector(newRot);
}
Vector3 SceneManager::GetCamRotVec()
{
	return m_camera->GetRotationVector();
}

Vector4 SceneManager::GetFogColor()
{
	return m_fogColor;
}
GLfloat SceneManager::GetFogStart()
{
	return m_fogStart;
}
GLfloat SceneManager::GetFogRange()
{
	return m_fogRange;
}
GLfloat SceneManager::GetTotalGameTime()
{
	return m_gameTime;
}
Vector4 SceneManager::GetAmbientColor()
{
	return m_lightAmbientColor;
}
Lights& SceneManager::GetLightByID(GLuint i_ID)
{
	return m_listLight[i_ID];
}
GLfloat SceneManager::GetAmbientWeight()
{
	return m_lightAmbientWeight;
}
GLuint SceneManager::GetNumOfLights()
{
	return m_numOfLights;
}
Object* SceneManager::GetObjectByID(GLuint i_ID)
{
	return &m_listObjects[i_ID];
}

void SceneManager::RotScreenViewLeft(float deltaTime, float deltaRange)
{
	RotCamLeft(deltaTime, deltaRange);
	m_primaryWeapon->RotFollowCameraLeft(deltaTime, deltaRange);
}
void SceneManager::RotScreenViewRight(float deltaTime, float deltaRange)
{
	RotCamRight(deltaTime, deltaRange);
	m_primaryWeapon->RotFollowCameraRight(deltaTime, deltaRange);
}
void SceneManager::RotScreenViewUp(float deltaTime, float deltaRange)
{
	RotCamUp(deltaTime, deltaRange);
	m_primaryWeapon->RotFollowCameraUp(deltaTime, deltaRange);
}
void SceneManager::RotScreenViewDown(float deltaTime, float deltaRange)
{
	RotCamDown(deltaTime, deltaRange);
	m_primaryWeapon->RotFollowCameraDown(deltaTime, deltaRange);
}

void SceneManager::InitRobotList()
{
	//srand((unsigned)time(NULL));

	m_numOfRobot = GamePlayManager::GetInstance()->GetNumOfRobot();
	m_RobotList = new Robot[m_numOfRobot];

	m_posListRobotDisplay = new Vector3[5];//So robot toi da cung xuat hien trong 1 man choi la 5
	
	for (int i = 0; i < m_numOfRobot; i++)
	{
		m_RobotList[i].Init();
		if (i < 5) // So robot toi da xuat hien cung luc trong game
		{
			bool isOk = false;
			int pos_z;
			int pos_x;
			while (isOk == false)
			{
				pos_z = -5 + (rand() % 4);// -1 den -5
				pos_x = -2 + pos_z + (rand() % ((2 - pos_z) * 2 + 1));//
				isOk = true;
				for (int j = 0; j < m_numOfRobotDisplay; j++)
				{
					if (pos_x == (int) m_posListRobotDisplay[j].x && pos_z == (int) m_posListRobotDisplay[j].z)
					{
						isOk = false;
					}					
				}
			}
			m_numOfRobotDisplay++;
			m_RobotList[i].SetPosition((float)pos_x, 1.25f, (float)pos_z);
			m_RobotList[i].SetIsDisplay(true);
			m_posListRobotDisplay[i].x = (float) pos_x;
			m_posListRobotDisplay[i].z = (float) pos_z;
		}
	}

	//m_RobotList[0].SetPosition(-7.0f, 1.25f, -5.0f);		m_RobotList[0].SetIsDisplay(true);
	//m_RobotList[1].SetPosition(-8.0f, 1.25f, -6.0f);		m_RobotList[1].SetIsDisplay(true);
}

void SceneManager::UpdateRobotList(float deltaTime)
{
	m_numOfRobotDisplay = 0;
	for (int i = 0; i < m_numOfRobot; i++)
	{
		m_RobotList[i].Update(deltaTime);
		if (m_RobotList[i].GetIsDisplay())
		{
			m_posListRobotDisplay[m_numOfRobotDisplay].x = m_RobotList[i].GetPosition().x;
			m_posListRobotDisplay[m_numOfRobotDisplay].z = m_RobotList[i].GetPosition().z;
			m_numOfRobotDisplay++;
		}
	}
	while (m_numOfRobotDisplay < 5 && GamePlayManager::GetInstance()->GetCurrentEnemy() >= 5)
	{
		bool isOk = false;
		int pos_z;
		int pos_x;
		while (isOk == false)
		{
			pos_z = -5 + (rand() % 4);// -1 den -5
			pos_x = -2 + pos_z + (rand() % ((2 - pos_z) * 2 + 1));//
			isOk = true;
			for (int j = 0; j < m_numOfRobotDisplay; j++)
			{
				if (pos_x == (int)m_posListRobotDisplay[j].x && pos_z == (int)m_posListRobotDisplay[j].z)
				{
					isOk = false;
				}
			}
		}
		int k = 0;
		while (m_RobotList[k].GetIsDisplay() == true || m_RobotList[k].GetIsDestroy() == true)
		{
			k++;
		}
		m_posListRobotDisplay[m_numOfRobotDisplay].x = (float)pos_x;
		m_posListRobotDisplay[m_numOfRobotDisplay].z = (float)pos_z;
		m_numOfRobotDisplay++;
		m_RobotList[k].SetPosition((float)pos_x, 1.25f, (float)pos_z);
		m_RobotList[k].SetIsDisplay(true);
	}
}

void SceneManager::CreateNewBoxObject()
{
	//bool isOk = false;
	//int pos_z;
	//int pos_x;
	//while (isOk == false)
	//{
	//	pos_z = -5 + (rand() % 4);// -1 den -5
	//	pos_x = -2 + pos_z + (rand() % ((2 - pos_z) * 2 + 1));//
	//	isOk = true;
	//	for (int j = 0; j < m_numOfRobotDisplay; j++)
	//	{
	//		if (pos_x == (int) m_posListRobotDisplay[j].x && pos_z == (int) m_posListRobotDisplay[j].z)
	//		{
	//			isOk = false;
	//		}					
	//	}
	//}
	//---
	//m_boxObject->SetType(HEALTH_BOX);
	//m_boxObject->SetLocation(Vector3(0.0f, 2.0f, -2.5f));
	//m_boxObject->SetIsDisplay(true);
	//---
	int pos_z = -4 + (rand() % 3);// -2 den -4
	int pos_x = -2 + pos_z + (rand() % ((2 - pos_z) * 2 + 1));//
	int type = (rand() % 2 == 0) ? HEALTH_BOX : AMMO_BOX;
	m_boxObject->SetType(type);
	m_boxObject->SetLocation(Vector3(pos_z + 0.5f, 2.0f, pos_z + 0.5f));
	m_boxObject->SetIsDisplay(true);
}