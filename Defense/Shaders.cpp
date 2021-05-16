#include <stdafx.h>
#include "Shaders.h"

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	m_listStates = NULL;
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	colorAttribute = glGetAttribLocation(program, "a_color"); 
	wvpMatrixUniform = glGetUniformLocation(program, "u_WVP");
	uvLocation = glGetAttribLocation(program, "a_uv");
	aNormal = glGetAttribLocation(program, "a_normL");
	aBinormal = glGetAttribLocation(program, "a_binormL");
	aTangent = glGetAttribLocation(program, "a_tangentL");

	uListTexturesLoc = new GLint[MAX_TEXTURE_UNIT];

	for( GLuint i = 0 ; i < MAX_TEXTURE_UNIT ; i++)
	{
		char nameTexture[15];
		sprintf(nameTexture, "u_texture%u", i);
		uListTexturesLoc[i] = glGetUniformLocation(program, nameTexture);
	}
	//uTextureLoc = glGetUniformLocation(program, "u_texture");

	//
	uWorldMatrix = glGetUniformLocation(program, "u_W");
	uPosCamW = glGetUniformLocation(program, "u_posCamW");

	// Fog
	uFogColor = glGetUniformLocation(program, "fogColor");
	uFogStart = glGetUniformLocation(program, "fogStart");
	uFogRange = glGetUniformLocation(program, "fogRange");
	//Cube
	uCubeTexture = glGetUniformLocation(program, "u_cubeTexture");
	//Time
	uTime = glGetUniformLocation(program, "u_time");
	//Phong Lighting
	uAmbientColor = glGetUniformLocation(program, "u_ambientColor");
	uAmbientWeight = glGetUniformLocation(program, "u_ambientWeight");
	uLightLoc = new LightPropertiesLocation[MAX_LIGHT_UNIT];
	for(unsigned int iLightUnit = 0 ; iLightUnit < MAX_LIGHT_UNIT ; iLightUnit++)
	{
		char uLightStr[50];
		sprintf(uLightStr, "u_light[%u].color", iLightUnit);
		uLightLoc[iLightUnit].color = glGetUniformLocation(program, uLightStr);
		sprintf(uLightStr, "u_light[%u].position", iLightUnit);
		uLightLoc[iLightUnit].position = glGetUniformLocation(program, uLightStr);
		sprintf(uLightStr, "u_light[%u].isDirection", iLightUnit);
		uLightLoc[iLightUnit].isDirection = glGetUniformLocation(program, uLightStr);
	}
	//Water
	uDepthAjust = glGetUniformLocation(program, "u_depthAjust");
	//Blur
	uStep = glGetUniformLocation(program, "u_step");
	//Bloom
	uLimit = glGetUniformLocation(program, "u_limit");
	uMix = glGetUniformLocation(program, "u_mix");
	//DOF
	uNear = glGetUniformLocation(program, "u_near");
	uFar = glGetUniformLocation(program, "u_far");
	uFade = glGetUniformLocation(program, "u_fade");
	uClarity = glGetUniformLocation(program, "u_clarity");

	return 0;
}

Shaders::~Shaders()
{
	if (m_listStates != NULL) delete [] m_listStates;
	if (uListTexturesLoc != NULL) delete [] uListTexturesLoc;
	if (uLightLoc != NULL) delete [] uLightLoc;
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void Shaders::SetStates(GLuint i_numStates, GLenum* i_listStates)
{
	m_numOfStates = i_numStates;
	m_listStates = new GLenum[m_numOfStates];
	for (unsigned int i = 0 ; i < m_numOfStates ; i++)
	{
		m_listStates[i] = i_listStates[i];
	}
}
void Shaders::EnableStates()
{
	for(GLuint i = 0 ; i < m_numOfStates ; i++)
	{
		switch ( m_listStates[i] )
		{
		case GL_CULL_FACE:
			glEnable(GL_CULL_FACE);
			break;
		case GL_DEPTH_TEST:
			glEnable(GL_DEPTH_TEST);
			break;
		case GL_BLEND:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
	}
}
void Shaders::DisableStates()
{
	for(GLuint i = 0 ; i < m_numOfStates ; i++)
	{
		switch ( m_listStates[i] )
		{
		case GL_CULL_FACE:
			glDisable(GL_CULL_FACE);
			break;
		case GL_DEPTH_TEST:
			glDisable(GL_DEPTH_TEST);
			break;
		case GL_BLEND:
			glDisable(GL_BLEND);
			break;
		}
	}
}