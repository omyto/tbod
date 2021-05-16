#include "StdAfx.h"
#include "Model.h"
#include <string>
#include <fstream>
#include "GameDefineList.h"

Model::Model(void) : m_hIndexBuffer(0), m_hVertexBuffer(0), m_ID(0), m_noIndices(0), m_noVertexs(0), m_modelType(-1)
{
	m_uvMax = Vector2(0.0, 0.0);

	m_frM.max_x = 0.0f;
	m_frM.max_y = 0.0f;
	m_frM.max_z = 0.0f;

	m_frM.min_x = 0.0f;
	m_frM.min_y = 0.0f;
	m_frM.min_z = 0.0f;
}

Model::~Model(void)
{
	glDeleteBuffers(1, &m_hVertexBuffer);
	glDeleteBuffers(1, &m_hIndexBuffer);
}

void Model::InitModelNFG(char * pFileNFGTarget)
{
	m_modelType = MODEL_TYPE_NFG;
	FILE *pFileNFG = NULL;
	//pFileNFG = fopen(pFileNFGTarget, "r");
	fopen_s(&pFileNFG, pFileNFGTarget, "r");
	if (pFileNFG != NULL)
	{
		//while(!feof(fileNFG))
		{
			unsigned int numOfVertices = 0;
			fscanf_s(pFileNFG, "NrVertices: %u\n", &numOfVertices);
			Vertex *verticesData = new Vertex[numOfVertices];
			for (unsigned int i = 0; i < numOfVertices; i++)
			{
				fscanf_s(pFileNFG, "%*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];\n",
					&verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z,
					&verticesData[i].norm.x, &verticesData[i].norm.y, &verticesData[i].norm.z,
					&verticesData[i].binorm.x, &verticesData[i].binorm.y, &verticesData[i].binorm.z,
					&verticesData[i].tangent.x, &verticesData[i].tangent.y, &verticesData[i].tangent.z,
					&verticesData[i].uv.x, &verticesData[i].uv.y);
				//Get max UV
				if (verticesData[i].uv.x > m_uvMax.x)
				{
					m_uvMax.x = verticesData[i].uv.x;
				}
				if (verticesData[i].uv.y > m_uvMax.y)
				{
					m_uvMax.y = verticesData[i].uv.y;
				}
				//Get frame Model
				if (m_frM.max_x < verticesData[i].pos.x) m_frM.max_x = verticesData[i].pos.x;
				if (m_frM.max_y < verticesData[i].pos.y) m_frM.max_y = verticesData[i].pos.y;
				if (m_frM.max_z < verticesData[i].pos.z) m_frM.max_z = verticesData[i].pos.z;

				if (m_frM.min_x > verticesData[i].pos.x) m_frM.min_x = verticesData[i].pos.x;
				if (m_frM.min_y > verticesData[i].pos.y) m_frM.min_y = verticesData[i].pos.y;
				if (m_frM.min_z > verticesData[i].pos.z) m_frM.min_z = verticesData[i].pos.z;
			}
			//

			fscanf_s(pFileNFG, "NrIndices: %u\n", &m_noIndices);
			GLuint *indicesData = new GLuint[m_noIndices];
			for (unsigned int i = 0; i < m_noIndices; i += 3)
			{
				fscanf_s(pFileNFG, "%*d.    %d,    %d,    %d", &indicesData[i], &indicesData[i + 1], &indicesData[i + 2]);
			}

			fclose(pFileNFG);

			// Create vertex buffer
			glGenBuffers(1, &m_hVertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Create indices buffer
			glGenBuffers(1, &m_hIndexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIndexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_noIndices * sizeof(GLuint), indicesData, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			delete[] verticesData;
			delete[] indicesData;
		}
	}
}
void Model::InitModelOBJ(char * pFileOBJTarget)
{
	m_modelType = MODEL_TYPE_OBJ;

	int countT = 0, countN = 0, countP = 0, countF = 0;
	Vector3 *pos, *textcoor, *normal;
	Vertex *verticesData;

	std::fstream f;
	f.open(pFileOBJTarget);
	if (f)
	{
		std::string line;
		while (!f.eof())
		{
			getline(f, line);
			if (line.data()[0] == 'v')
			{
				if (line.data()[1] == ' ') countP++;
				else if (line.data()[1] == 't') countT++;
				else if (line.data()[1] == 'n') countN++;
			}
			else if (line.data()[0] == 'f') countF++;
		}
		f.close();

		pos = new Vector3[countP];
		textcoor = new Vector3[countT];
		normal = new Vector3[countN];
		verticesData = new Vertex[countF * 3];
		countP = 0;
		countT = 0;
		countF = 0;
		countN = 0;
		int arr[9];

		std::fstream file(pFileOBJTarget);

		while (!file.eof())
		{
			getline(file, line);
			if (line.data()[0] == 'v')
			{
				if (line.data()[1] == ' ')
				{
					sscanf(line.data(), "v  %f %f %f", &pos[countP].x, &pos[countP].y, &pos[countP].z);
					//Get frame model
					if (m_frM.max_x < pos[countP].x) m_frM.max_x = pos[countP].x;
					if (m_frM.max_y < pos[countP].y) m_frM.max_y = pos[countP].y;
					if (m_frM.max_z < pos[countP].z) m_frM.max_z = pos[countP].z;

					if (m_frM.min_x > pos[countP].x) m_frM.min_x = pos[countP].x;
					if (m_frM.min_y > pos[countP].y) m_frM.min_y = pos[countP].y;
					if (m_frM.min_z > pos[countP].z) m_frM.min_z = pos[countP].z;
					//------
					countP++;
				}
				else if (line.data()[1] == 't')
				{
					sscanf(line.data(), "vt  %f %f %f", &textcoor[countT].x, &textcoor[countT].y, &textcoor[countT].z);
					countT++;
				}
				else if (line.data()[1] == 'n')
				{
					sscanf(line.data(), "vn  %f %f %f", &normal[countN].x, &normal[countN].y, &normal[countN].z);
					countN++;
				}
			}
			else if (line.data()[0] == 'f')
			{
				sscanf(line.data(), "f  %d/%d/%d %d/%d/%d %d/%d/%d", &arr[0], &arr[1], &arr[2], &arr[3], &arr[4], &arr[5], &arr[6], &arr[7], &arr[8]);

				//---------- Point 1

				int verticesDataID = countF * 3 + 0;
				verticesData[verticesDataID].pos.x = pos[arr[0] - 1].x;
				verticesData[verticesDataID].pos.y = pos[arr[0] - 1].y;
				verticesData[verticesDataID].pos.z = pos[arr[0] - 1].z;

				verticesData[verticesDataID].uv.x = textcoor[arr[1] - 1].x;
				verticesData[verticesDataID].uv.y = textcoor[arr[1] - 1].y;

				if (countN > 0)
				{
					verticesData[verticesDataID].norm.x = normal[arr[2] - 1].x;
					verticesData[verticesDataID].norm.y = normal[arr[2] - 1].y;
					verticesData[verticesDataID].norm.z = normal[arr[2] - 1].z;
				}

				//---------- Point 2

				verticesDataID++;
				verticesData[verticesDataID].pos.x = pos[arr[3] - 1].x;
				verticesData[verticesDataID].pos.y = pos[arr[3] - 1].y;
				verticesData[verticesDataID].pos.z = pos[arr[3] - 1].z;

				verticesData[verticesDataID].uv.x = textcoor[arr[4] - 1].x;
				verticesData[verticesDataID].uv.y = textcoor[arr[4] - 1].y;

				if (countN > 0)
				{
					verticesData[verticesDataID].norm.x = normal[arr[5] - 1].x;
					verticesData[verticesDataID].norm.y = normal[arr[5] - 1].y;
					verticesData[verticesDataID].norm.z = normal[arr[5] - 1].z;
				}

				//---------- Point 3

				verticesDataID++;
				verticesData[verticesDataID].pos.x = pos[arr[6] - 1].x;
				verticesData[verticesDataID].pos.y = pos[arr[6] - 1].y;
				verticesData[verticesDataID].pos.z = pos[arr[6] - 1].z;

				verticesData[verticesDataID].uv.x = textcoor[arr[7] - 1].x;
				verticesData[verticesDataID].uv.y = textcoor[arr[7] - 1].y;
				
				if (countN > 0)
				{
					verticesData[verticesDataID].norm.x = normal[arr[8] - 1].x;
					verticesData[verticesDataID].norm.y = normal[arr[8] - 1].y;
					verticesData[verticesDataID].norm.z = normal[arr[8] - 1].z;
				}

				countF++;
			}
		}
		file.close();

		m_noVertexs = countF * 3;

		glGenBuffers(1, &m_hVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_noVertexs, verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (verticesData != NULL) delete[] verticesData;
		if (pos != NULL) delete[] pos;
		if (textcoor != NULL) delete[]textcoor;
		if (normal != NULL) delete[] normal;
	}
}

int Model::GetModelType()
{
	return m_modelType;
}

void Model::InitModel(char * pFileTarget)
{
	std::string strFileTarget(pFileTarget);
	int pos = strFileTarget.find(".obj");
	if (strFileTarget.find(".obj") != std::string::npos)
	{
		printf("Init file model obj: %s\n", pFileTarget);
		InitModelOBJ(pFileTarget);
	}
	else if (strFileTarget.find(".nfg") != std::string::npos)
	{
		printf("Init file model nfg: %s\n", pFileTarget);
		InitModelNFG(pFileTarget);
	}
	else
	{
		printf("Error model file type: %s\n", pFileTarget);
	}
}
//void Model::CleanUp()
//{
//	glDeleteBuffers(1, &m_hVertexBuffer);
//	glDeleteBuffers(1, &m_hIndexBuffer);
//}
GLuint Model::GetVBOId()
{
	return m_hVertexBuffer;
}
GLuint Model::GetIBOId()
{
	return m_hIndexBuffer;
}
GLuint Model::GetNumOfIndices()
{
	return m_noIndices;
}
Vector2 Model::GetMaxUV()
{
	return m_uvMax;
}
GLuint Model::GetNumOfVertexs()
{
	return m_noVertexs;
}
FrameModel* Model::GetFrameModel()
{
	return &m_frM;
}