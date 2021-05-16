#include "StdAfx.h"
#include "Pass.h"
#include "EffectManager.h"
#define EFFECT_BLUR_STEP_ID 0
#define EFFECT_BLOOM_MIX_ID 0
#define EFFECT_BLOOM_LIMIT_ID 0
#define EFFECT_DOF_NEAR_ID 1
#define EFFECT_DOF_FAR_ID 2
#define EFFECT_DOF_FADE_ID 3
#define EFFECT_DOF_CLARITY_ID 4
Pass::Pass(void)
{
	m_ID = 0;
	m_name = NULL;
	m_shaderID = 0;
	m_shader = NULL;
	m_numOfTexture = 0;
	m_listTextureID = NULL;
	m_listTextureKind = NULL;
	m_target = 0;
	m_numOfOtherData = 0;
	m_otherData = NULL;
}

Pass::~Pass(void)
{
}
void Pass::SetID(GLuint i_ID)
{
	m_ID = i_ID;
}
void Pass::SetName(char* i_name)
{
	m_name = new char[strlen(i_name) + 1];
	strcpy(m_name, i_name);
}
void Pass::SetShaderID(GLuint i_shaderID)
{
	m_shaderID = i_shaderID;
	m_shader = EffectManager::GetInstance()->GetShaderByID(m_shaderID);
}
void Pass::SetTextureID(GLuint i_numOfFBO, char* i_listTextureStr)
{
	m_numOfTexture = i_numOfFBO;
	m_listTextureID = new GLuint[m_numOfTexture];
	m_listTextureKind = new char[m_numOfTexture];
	bool isReadComplete = false;
	GLuint iTexture = 0;
	GLuint iChar = 0;
	char character = ' ';
	while ( !isReadComplete )
	{
		character = i_listTextureStr[iChar];
		if ( character != ' ' && character != ',' )
		{
			if ( character != '0' )
			{
				m_listTextureID[iTexture] = character - '0';
				iChar++;
				m_listTextureKind[iTexture] = i_listTextureStr[iChar];
				iTexture++;
			}
			else
			{
				m_listTextureID[iTexture] = 0;
				m_listTextureKind[iTexture] = ' ';
				iTexture++;
			}
		}
		iChar++;
		if ( iTexture == m_numOfTexture || iChar == strlen(i_listTextureStr) )
		{
			isReadComplete = true;
		}
	}
}
void Pass::SetTarget(GLuint i_target)
{
	m_target = i_target;
}
void Pass::SetOtherData(char* i_listOtherDataStr)
{
	m_numOfOtherData = 1;
	//Count num of other data
	for(GLuint i = 0 ; i <= strlen(i_listOtherDataStr) ; i++)
	{
		if (i_listOtherDataStr[i] == ',')
		{
			m_numOfOtherData++;
		}
	}
	m_otherData = new GLfloat[m_numOfOtherData];
	char strTmp[50];
	GLuint iStrStart = 0;
	GLuint iStrStop = 0;
	GLuint iData = 0;
	GLuint iStr = 0;
	for (GLuint i = 0; i <= strlen(i_listOtherDataStr); i++)
	{
		if (i_listOtherDataStr[i] != ',')
		{
			strTmp[iStr++] = i_listOtherDataStr[i];
		}
		else
		{
			strTmp[iStr] = '\0';
			iStr = 0;
			sscanf(strTmp, "%f", &m_otherData[iData++]);
		}
	}
	strTmp[iStr] = '\0';
	sscanf(strTmp, "%f", &m_otherData[iData]);
}
void Pass::CleanUp()
{
	if (m_name != NULL) delete m_name;
	if (m_listTextureID != NULL) delete [] m_listTextureID;
	if (m_listTextureKind != NULL) delete [] m_listTextureKind;
	if (m_otherData != NULL) delete [] m_otherData;
}
void Pass::Draw()
{
	if (m_target != 0) //Draw to FBO if target != 0, draw to screen if target == 0
	{
		EffectManager::GetInstance()->BindFBOByID(m_target - 1);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, EffectManager::GetInstance()->GetVertextBufferID());
	glUseProgram(m_shader->program);
	//Position
	if (m_shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(m_shader->positionAttribute);
		glVertexAttribPointer(m_shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));
	}
	//UV
	if (m_shader->uvLocation != -1)
	{
		glEnableVertexAttribArray(m_shader->uvLocation);
		glVertexAttribPointer(m_shader->uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));
	}
	//Blur step
	if (m_shader->uStep != -1)
	{
		float x = 1.0f / Globals::screenWidth;
		float y = 1.0f / Globals::screenHeight;
		float z = sqrt(2.0f) / 2.0f * x;
		float w = sqrt(2.0f) / 2.0f * y;
		float k = m_otherData[EFFECT_BLUR_STEP_ID];
		glUniform4f(m_shader->uStep, k * x, k * y, k * z, k * w);
	}
	//Bloom limit
	if (m_shader->uLimit != -1)
	{
		glUniform1f(m_shader->uLimit, m_otherData[EFFECT_BLOOM_LIMIT_ID]);
	}
	//Bloom mix
	if (m_shader->uMix != -1)
	{
		glUniform1f(m_shader->uMix, m_otherData[EFFECT_BLOOM_MIX_ID]);
	}
	//DOF
	if (m_shader->uNear != -1)
	{
		glUniform1f(m_shader->uNear, m_otherData[EFFECT_DOF_NEAR_ID]);
	}
	if (m_shader->uFar != -1)
	{
		glUniform1f(m_shader->uFar, m_otherData[EFFECT_DOF_FAR_ID]);
	}
	if (m_shader->uFade != -1)
	{
		glUniform1f(m_shader->uFade, m_otherData[EFFECT_DOF_FADE_ID]);
	}
	if (m_shader->uClarity != -1)
	{
		glUniform1f(m_shader->uClarity, m_otherData[EFFECT_DOF_CLARITY_ID]);
	}
	//Texture
	for (GLuint iTextureUnit = 0 ; iTextureUnit < m_numOfTexture ; iTextureUnit++)
	{
		if (m_listTextureID[iTextureUnit] != 0) //Use
		{
			GLuint textureID = 0;
			if (m_listTextureKind[iTextureUnit] == 'c') //Color texture
			{
				textureID = EffectManager::GetInstance()->GetFBOColorTextureByID(m_listTextureID[iTextureUnit] - 1);
			}
			else // Depth texture
			{
				textureID = EffectManager::GetInstance()->GetFBODepthTextureByID(m_listTextureID[iTextureUnit] - 1);
			}
			glActiveTexture(GL_TEXTURE0 + iTextureUnit);
			glBindTexture(GL_TEXTURE_2D, textureID);
			if (m_shader->uListTexturesLoc[iTextureUnit] != -1)
			{
				glUniform1i(m_shader->uListTexturesLoc[iTextureUnit], iTextureUnit);
			}
		}
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EffectManager::GetInstance()->GetIndicesBufferID());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Bind to default system
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	for (GLuint iTextureUnit = 0 ; iTextureUnit < m_numOfTexture ; iTextureUnit++)
	{
		if (m_listTextureID[iTextureUnit] != 0) //Use
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	if (m_target != 0) //Unbind FBO if using
	{
		EffectManager::GetInstance()->UnbindFBOByID(m_target - 1);
	}
}

