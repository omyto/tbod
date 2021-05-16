#pragma once
#include "Pass.h"
class Effect
{
private:
	GLuint m_ID;
	char* m_name;
	GLuint m_numOfPass;
	Pass* m_listPass;
public:
	Effect(void);
	~Effect(void);
	void Init(GLuint i_ID, char* i_name, GLuint i_numOfPass);
	void SetPassValue(GLuint i_passID, char* i_passName, GLuint i_passShaderID, GLuint i_passTarget, GLuint i_numOfFBO, char* i_passTextureList, char* i_passOtherDataList);
	void Draw();
	void CleanUp();
};
