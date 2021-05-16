#include "StdAfx.h"
#include "Effect.h"

Effect::Effect(void)
{
	m_ID = -1;
	m_name = NULL;
	m_numOfPass = 0;
	m_listPass = NULL;
}

Effect::~Effect(void)
{
}
void Effect::Init(GLuint i_ID, char* i_name, GLuint i_numOfPass)
{
	m_ID = i_ID;
	m_name = new char[strlen(i_name) + 1];
	strcpy(m_name, i_name);
	m_numOfPass = i_numOfPass;
	m_listPass = new Pass[m_numOfPass];
}
void Effect::SetPassValue(GLuint i_passID, char* i_passName, GLuint i_passShaderID, GLuint i_passTarget, GLuint i_numOfFBO, char* i_passTextureList, char* i_passOtherDataList)
{
	m_listPass[i_passID].SetID(i_passID);
	m_listPass[i_passID].SetName(i_passName);
	m_listPass[i_passID].SetShaderID(i_passShaderID);
	m_listPass[i_passID].SetTarget(i_passTarget);
	m_listPass[i_passID].SetTextureID(i_numOfFBO, i_passTextureList);
	m_listPass[i_passID].SetOtherData(i_passOtherDataList);
}
void Effect::Draw()
{
	for(GLuint iPass = 0 ; iPass < m_numOfPass ; iPass++)
	{
		m_listPass[iPass].Draw();
	}
}
void Effect::CleanUp()
{
	for(GLuint i = 0; i < m_numOfPass ; i++)
	{
		m_listPass[i].CleanUp();
	}
	if (m_name != NULL)
	{
		delete m_name;
		m_name = NULL;
	}
	if (m_listPass != NULL)
	{
		delete [] m_listPass;
		m_listPass = NULL;
	}
}