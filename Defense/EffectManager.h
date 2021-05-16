#pragma once

#include "FBO.h"
#include "Shaders.h"
#include "Effect.h"
#include "Vertex.h"

class EffectManager
{
private:
	static EffectManager* m_instance;
	GLuint m_numOfFBO;
	FBO* m_listFBO;
	GLuint m_numOfShader;
	Shaders* m_listShader;
	GLuint m_numOfEffect;
	Effect* m_listEffect;
	GLuint m_hvboID;
	GLuint m_hiboID;
	GLuint m_numOfIndices;
public:
	EffectManager(void);
	~EffectManager(void);
	static EffectManager* GetInstance();
	static void FreeInstance();
	void Init();
	void Load(char *fileTarget);
	Shaders* GetShaderByID(GLuint i_ID);
	void Draw(GLuint iEffect);
	void CleanUp();
	void BindFBOByID(GLuint i_ID);
	void UnbindFBOByID(GLuint i_ID);
	GLuint GetFBOColorTextureByID(GLuint i_ID);
	GLuint GetFBODepthTextureByID(GLuint i_ID);
	GLuint GetVertextBufferID();
	GLuint GetIndicesBufferID();
	GLuint GetNumOfIndices();
};
