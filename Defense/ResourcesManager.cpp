#include "StdAfx.h"
#include "ResourcesManager.h"

ResourcesManager* ResourcesManager::m_instance = NULL;

ResourcesManager::ResourcesManager(void) : m_numOfModels(0), m_numOfShaders(0), m_numOfTextures2D(0), m_numOfTextures3D(0)
{
	m_listModels = NULL;
	m_listTextures2D = NULL;
	m_listTextures3D = NULL;
	m_listShaders = NULL;
	
	m_resourcesTarget = new char[strlen("../Resources/") + 1];
	strcpy(m_resourcesTarget, "../Resources/");
}

ResourcesManager::~ResourcesManager(void)
{
}

ResourcesManager* ResourcesManager::GetInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new ResourcesManager();
	}
	return m_instance;
}

void ResourcesManager::SetResourcesTarget(char * pResourcesTarget)
{
	if(m_resourcesTarget != NULL)
	{
		delete [] m_resourcesTarget;
		m_resourcesTarget = NULL;
	}
	m_resourcesTarget = new char[strlen(pResourcesTarget) + 1];
	strcmp(m_resourcesTarget, pResourcesTarget);
}

void ResourcesManager::Init(char* pFileTarget)
{
	FILE* pFile = NULL;
	//pFile = fopen(pFileTarget, "r");
	fopen_s(&pFile, pFileTarget, "r");
	while( !feof(pFile))
	{

		//Init home path resources
		char path[100];
		strcpy(path, m_resourcesTarget);
		unsigned int pathEnd = strlen(path);

		// Load models
		fscanf(pFile, "#Models: %u\n", &m_numOfModels);
		m_listModels = new Model[m_numOfModels];
		unsigned int modelID = -1;
		char modelTarget[100];
		for(unsigned int i = 0 ; i < m_numOfModels ; i++ )
		{
			fscanf(pFile, "ID %u\n", &modelID);
			fscanf(pFile, "FILE %s\n", modelTarget);
			strcat(path, modelTarget);
			m_listModels[modelID].InitModel(path);
			path[pathEnd] = 0;
		}
		fscanf(pFile, "\n");

		// Load textures 2D
		fscanf(pFile, "#2D Textures: %u\n", &m_numOfTextures2D);
		m_listTextures2D = new Texture[m_numOfTextures2D];
		unsigned int texture2DID = -1;
		char texture2DTarget[100];
		char texture2DTiling[50];
		for(unsigned int i = 0 ; i < m_numOfTextures2D; i++)
		{
			fscanf(pFile, "ID %u\n", &texture2DID);
			fscanf(pFile, "FILE %s\n", texture2DTarget);
			fscanf(pFile, "TILING %s\n", texture2DTiling);
			strcat(path, texture2DTarget);
			m_listTextures2D[texture2DID].Init2DTextures(path, texture2DTiling);
			path[pathEnd] = 0;
		}
		fscanf(pFile, "\n");

		// Load textures 3D
		fscanf(pFile, "#Cube Textures: %u\n", &m_numOfTextures3D);
		m_listTextures3D = new Texture[m_numOfTextures3D];
		unsigned int texture3DID = -1;
		char texture3DTarget[100];
		char texture3DTiling[50];
		for(unsigned int i = 0 ; i < m_numOfTextures3D ; i++)
		{
			fscanf(pFile, "ID %u\n", &texture3DID);
			fscanf(pFile, "FILE %s\n", texture3DTarget);
			fscanf(pFile, "TILING %s\n", texture3DTiling);
			strcat(path, texture3DTarget);
			m_listTextures3D[texture3DID].InitCubeTexture(path);
			path[pathEnd] = 0;
		}
		fscanf(pFile, "\n");

		//Load shaders
		fscanf(pFile, "#Shaders: %u\n", &m_numOfShaders);
		m_listShaders = new Shaders[m_numOfShaders];
		unsigned int shaderID = -1;
		char shaderVSTarget[100];
		char shaderFSTarget[100];
		unsigned int numOfShaderState = 0;
		GLenum* listShaderStates = NULL;
		char shaderState[20];
		for(unsigned int i = 0 ; i < m_numOfShaders ; i++)
		{
			fscanf(pFile, "ID %u\n", &shaderID);
			fscanf(pFile, "VS %s\n", shaderVSTarget);
			fscanf(pFile, "FS %s\n", shaderFSTarget);
			fscanf(pFile, "STATE %u\n", &numOfShaderState);
			listShaderStates = new GLenum[numOfShaderState];
			for(unsigned int shaderStateID = 0 ; shaderStateID < numOfShaderState ; shaderStateID++)
			{
				fscanf(pFile, "STATE %s\n", shaderState);
				if ( strcmp(shaderState, "CULLING") == 0 )
				{
					listShaderStates[shaderStateID] = GL_CULL_FACE;
				}
				if ( strcmp(shaderState, "DEPTH_TEST") == 0 )
				{
					listShaderStates[shaderStateID] = GL_DEPTH_TEST;
				}
				if ( strcmp(shaderState, "BLEND") == 0 )
				{
					listShaderStates[shaderStateID] = GL_BLEND;
				}
			}
			strcpy(path, shaderVSTarget);
			strcpy(shaderVSTarget, m_resourcesTarget);
			strcat(shaderVSTarget, path);

			strcpy(path, shaderFSTarget);
			strcpy(shaderFSTarget, m_resourcesTarget);
			strcat(shaderFSTarget, path);
			m_listShaders[i].Init(shaderVSTarget, shaderFSTarget);
			m_listShaders[i].SetStates(numOfShaderState, listShaderStates);
			delete [] listShaderStates;
		}

		while( !feof(pFile))
		{
			fscanf(pFile, "%*s\n");
		}

		// Clean memory
		
	}
	fclose(pFile);
}

Model* ResourcesManager::GetModelByID(GLuint i_ID)
{
	return &m_listModels[i_ID];
}

Texture* ResourcesManager::GetTexture2DByID(GLuint i_ID)
{
	return &m_listTextures2D[i_ID];
}

Texture* ResourcesManager::GetTexture3DByID(GLuint i_ID)
{
	return &m_listTextures3D[i_ID];
}

Shaders* ResourcesManager::GetShaderByID(GLuint i_ID)
{
	return &m_listShaders[i_ID];
}

FrameModel* ResourcesManager::GetFrameModelByID(GLuint i_ID)
{
	return m_listModels[i_ID].GetFrameModel();
}

void ResourcesManager::CleanUp()
{
	if(m_resourcesTarget != NULL)
	{
		delete [] m_resourcesTarget;
		m_resourcesTarget = NULL;
	}
	if(m_listModels != NULL)
	{
		delete [] m_listModels;
		m_listModels = NULL;
	}
	if(m_listTextures2D != NULL)
	{
		delete [] m_listTextures2D;
		m_listTextures2D = NULL;
	}
	if(m_listTextures3D != NULL)
	{
		delete [] m_listTextures3D;
		m_listTextures3D = NULL;
	}
	if(m_listShaders != NULL)
	{
		delete [] m_listShaders;
		m_listShaders = NULL;
	}
	if(m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}