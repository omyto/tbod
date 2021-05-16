#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"

struct FrameModel
{
	float max_x;
	float max_y;
	float max_z;
	float min_x;
	float min_y;
	float min_z;
};

class Model
{
private:
	GLuint m_hVertexBuffer, m_hIndexBuffer;
	unsigned int m_noIndices;
	unsigned int m_noVertexs;
	unsigned int m_ID;
	Vector2 m_uvMax;
	void InitModelNFG(char * pFileNFGTarget);
	void InitModelOBJ(char * pFileOBJTarget);
	int m_modelType;
	FrameModel m_frM;
public:
	Model(void);
	~Model(void);
	void InitModel( char * pFileTarget);
	//void CleanUp();
	GLuint GetVBOId();
	GLuint GetIBOId();
	GLuint GetNumOfIndices();
	GLuint GetNumOfVertexs();
	Vector2 GetMaxUV();
	int GetModelType();
	FrameModel* GetFrameModel();
};
