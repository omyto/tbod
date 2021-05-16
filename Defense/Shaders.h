#pragma once
#include "../Utilities/utilities.h"
#include "ShaderPropertiesLoc.h"
#define MAX_TEXTURE_UNIT 32
#define MAX_LIGHT_UNIT 5
class Shaders 
{
private:
	GLenum* m_listStates;
	GLuint m_numOfStates;
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	GLint positionAttribute;
	GLint colorAttribute;
	GLint wvpMatrixUniform;
	GLint uvLocation;
	GLint aNormal;
	GLint aBinormal;
	GLint aTangent;

	GLint* uListTexturesLoc;

	//
	GLint uWorldMatrix;
	GLint uPosCamW;
	
	//Fog
	GLint uFogColor;
	GLint uFogStart;
	GLint uFogRange;
	//Cube
	GLint uCubeTexture;
	
	//Time
	GLint uTime;

	//Phong lighting
	GLint uAmbientColor;
	GLint uAmbientWeight;
	LightPropertiesLocation* uLightLoc;

	//Water
	GLint uDepthAjust;
	
	//Blur
	GLint uStep;

	//Bloom
	GLint uLimit;
	GLint uMix;
	//DOF
	GLint uNear;
	GLint uFar;
	GLint uFade;
	GLint uClarity;

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
	void SetStates(GLuint i_numStates, GLenum* i_listStates);
	void EnableStates();
	void DisableStates();
};