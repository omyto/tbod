#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Model.h"
#include "Shaders.h"
#include "Texture.h"
#include <string.h>
class ResourcesManager
{
private:
	static ResourcesManager* m_instance;
	GLuint m_numOfModels;
	GLuint m_numOfTextures2D;
	GLuint m_numOfTextures3D;
	GLuint m_numOfShaders;
	Model* m_listModels;
	Texture* m_listTextures2D;
	Texture* m_listTextures3D;
	Shaders* m_listShaders;
	char* m_resourcesTarget;
public:
	ResourcesManager(void);
	~ResourcesManager(void);
	static ResourcesManager* GetInstance();
	void Init(char* pFileTarget);
	void SetResourcesTarget(char * pResourcesTarget);
	Model* GetModelByID(GLuint i_ID);
	Texture* GetTexture2DByID(GLuint i_ID);
	Texture* GetTexture3DByID(GLuint i_ID);
	Shaders* GetShaderByID(GLuint i_ID);
	FrameModel* GetFrameModelByID(GLuint i_ID);
	void CleanUp();
};
