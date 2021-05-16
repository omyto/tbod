#include "StdAfx.h"
#include "EffectManager.h"
EffectManager* EffectManager::m_instance = NULL;
EffectManager::EffectManager(void)
{
	m_numOfFBO = 0;
	m_listFBO = NULL;
	m_numOfShader = 0;
	m_listShader = NULL;
	m_numOfEffect = 0;
	m_listEffect = NULL;
	m_hvboID = 0;
	m_hiboID = 0;
	m_numOfIndices = 6;
	Init();
}

EffectManager::~EffectManager(void)
{
	glDeleteBuffers(1, &m_hvboID);
	glDeleteBuffers(1, &m_hiboID);
}
void EffectManager::Init()
{
	//Vertices Buffer
	Vertex vertices[4];
	vertices[0].pos.x = -1.0f;	vertices[0].pos.y =  1.0f;	vertices[0].pos.z =  0.0f;	vertices[0].uv.x = 0.0f;	vertices[0].uv.y = 1.0f;
	vertices[1].pos.x = -1.0f;	vertices[1].pos.y = -1.0f;	vertices[1].pos.z =  0.0f;	vertices[1].uv.x = 0.0f;	vertices[1].uv.y = 0.0f;
	vertices[2].pos.x =  1.0f;	vertices[2].pos.y = -1.0f;	vertices[2].pos.z =  0.0f;	vertices[2].uv.x = 1.0f;	vertices[2].uv.y = 0.0f;
	vertices[3].pos.x =  1.0f;	vertices[3].pos.y =  1.0f;	vertices[3].pos.z =  0.0f;	vertices[3].uv.x = 1.0f;	vertices[3].uv.y = 1.0f;
	glGenBuffers(1, &m_hvboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_hvboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//GLfloat verticesData[] = {
	//	-0.8f,	0.8f,	0.0f,
	//	-0.8f,	-0.8f,	0.0f,
	//	0.8f,	-0.8f,	0.0f,
	//	0.8f,	0.8f,	0.0f
	//};
	//GLfloat uvData[] = {
	//	0.0,	1.0,
	//	0.0,	0.0,
	//	1.0,	0.0,
	//	1.0,	1.0
	//};
	
	//Indices Data
	GLuint indicesData[] = {
		0, 1, 2,
		0, 2, 3
	};
	glGenBuffers(1, &m_hiboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hiboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
EffectManager* EffectManager::GetInstance()
{
	if ( m_instance == NULL)
	{
		m_instance = new EffectManager();
	}
	return m_instance;
}
void EffectManager::FreeInstance()
{
	if(m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}
void EffectManager::CleanUp()
{
	for (GLuint i = 0; i < m_numOfFBO; i++)
	{
		m_listFBO[i].CleanUp();
	}
	for (GLuint i = 0; i < m_numOfEffect; i++)
	{
		m_listEffect[i].CleanUp();
	}
	if (m_listFBO != NULL)
	{
		delete [] m_listFBO;
		m_listFBO = NULL;
	}
	if (m_listEffect != NULL)
	{
		delete [] m_listEffect;
		m_listEffect = NULL;
	}
	if (m_listShader != NULL)
	{
		delete [] m_listShader;
		m_listShader = NULL;
	}
}
void EffectManager::Load(char *fileTarget)
{
	FILE *pFile = fopen( fileTarget, "r");
	while ( ! feof(pFile) )
	{
		//#Shaders
		fscanf(pFile, "#Shaders: %u\n", &m_numOfShader);
		m_listShader = new Shaders[m_numOfShader];
		GLuint shaderID;
		char pathVS[50];
		char pathFS[50];
		char pathTmp[50];
		for (GLuint iShader = 0 ; iShader < m_numOfShader ; iShader++)
		{
			fscanf(pFile, "ID %u\n", &shaderID);
			fscanf(pFile, "VSFile: %s\n", pathTmp);
			sprintf(pathVS, "../Resources/Shaders/Effects/%s", pathTmp);
			fscanf(pFile, "FSFile: %s\n", pathTmp);
			sprintf(pathFS, "../Resources/Shaders/Effects/%s", pathTmp);
			m_listShader[iShader].Init(pathVS, pathFS);
		}

		fscanf(pFile, "\n");
		//#FBO
		fscanf(pFile, "#FBO\n");
		fscanf(pFile, "NoFBO %u\n", &m_numOfFBO);
		m_listFBO = new FBO[m_numOfFBO];
		for (GLuint iFBO = 0 ; iFBO < m_numOfFBO ; iFBO++)
		{
			m_listFBO[iFBO].Init();
		}

		fscanf(pFile, "\n");
		//#Effect
		fscanf(pFile, "#Effects: %u\n", &m_numOfEffect);
		m_listEffect = new Effect[m_numOfEffect];
		GLuint effectID = 0;
		char effectName[50];
		GLuint effectNumOfPass;
		GLuint passID;
		char passName[50];
		GLuint passShaderID;
		GLuint passTarget;
		char passTextureList[1024];
		char passOtherDataList[1024];

		for( GLuint iEffect = 0 ; iEffect < m_numOfEffect ; iEffect++)
		{
			fscanf(pFile, "\n");
			fscanf(pFile, "ID %u\n", &effectID);
			fscanf(pFile, "Name %s\n", effectName);
			fscanf(pFile, "NoPasses: %u\n", &effectNumOfPass);
			m_listEffect[iEffect].Init(effectID, effectName, effectNumOfPass);
			//Pass
			for(GLuint iPass = 0 ; iPass < effectNumOfPass ; iPass++)
			{
				fscanf(pFile, "PassID %u\n", &passID);
				fscanf(pFile, "PassName %s\n", passName);
				fscanf(pFile, "   ShaderID %u\n", &passShaderID);
				fscanf(pFile, "   Textures: ");
				fgets(passTextureList, 1024, pFile);
				fscanf(pFile, "   Target: %u\n", &passTarget);
				fscanf(pFile, "   OtherData: ");
				fgets(passOtherDataList, 1024, pFile);
				m_listEffect[iEffect].SetPassValue(passID, passName, passShaderID, passTarget, m_numOfFBO, passTextureList, passOtherDataList);
			}
		}
		while ( ! feof(pFile) )
		{
			fscanf(pFile, "%*s\n");
		}

	}
	fclose(pFile);
}
Shaders* EffectManager::GetShaderByID(GLuint i_ID)
{
	return &m_listShader[i_ID];
}
void EffectManager::BindFBOByID(GLuint i_ID)
{
	m_listFBO[i_ID].BindFBO();
}
void EffectManager::UnbindFBOByID(GLuint i_ID)
{
	m_listFBO[i_ID].UnbindFBO();
}
GLuint EffectManager::GetFBOColorTextureByID(GLuint i_ID)
{
	return m_listFBO[i_ID].GetColorTextureID();
}
GLuint EffectManager::GetFBODepthTextureByID(GLuint i_ID)
{
	return m_listFBO[i_ID].GetDepthTextureID();
}
GLuint EffectManager::GetVertextBufferID()
{
	return m_hvboID;
}
GLuint EffectManager::GetIndicesBufferID()
{
	return m_hiboID;
}
GLuint EffectManager::GetNumOfIndices()
{
	return m_numOfIndices;
}
void EffectManager::Draw(GLuint iEffect)
{
	m_listEffect[iEffect].Draw();
}