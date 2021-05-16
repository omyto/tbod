#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Shaders.h"
#include <stddef.h>
class Pass
{
private:
	GLuint m_ID;
	char* m_name;
	GLuint m_shaderID;
	Shaders* m_shader;
	GLuint m_numOfTexture;
	GLuint* m_listTextureID;
	char* m_listTextureKind;
	GLuint m_target;
	GLuint m_numOfOtherData;
	GLfloat* m_otherData;
public:
	Pass(void);
	~Pass(void);
	void SetID(GLuint i_ID);
	void SetName(char* i_name);
	void SetShaderID(GLuint i_shaderID);
	void SetTextureID(GLuint i_numOfFBO, char* i_listTextureStr);
	void SetTarget(GLuint i_target);
	void SetOtherData(char* i_listOtherDataStr);
	void Draw();
	void CleanUp();
};
