#include "StdAfx.h"
#include "Object.h"
#include "SceneManager.h"
#include "GameDefineList.h"

Object::Object(void) : m_numOfTexture2D(0), m_numOfTexture3D(0), m_isChangeWorldMatrix(true)
{
	m_Model = NULL;
	m_shader = NULL;
	m_listTexture2D = NULL;
	m_listTexture3D = NULL;
	m_listLightID = NULL;

	m_position = Vector3( 0.0f, 0.0f, 0.0f);
	m_rotation = Vector3( 0.0f, 0.0f, 0.0f);
	m_scale = Vector3( 1.0f, 1.0f, 1.0f);

	m_worldMatrix.SetIdentity();

	p_frM = NULL;

	m_frameModel.max_x = 0.0f;
	m_frameModel.max_y = 0.0f;
	m_frameModel.max_z = 0.0f;

	m_frameModel.min_x = 0.0f;
	m_frameModel.min_y = 0.0f;
	m_frameModel.min_z = 0.0f;
}

Object::~Object(void)
{
}

void Object::Init()
{
	printf("Init Object\n");
}

void Object::Update(float deltaTime)
{
	CalculateWorldMatrix();
}

void Object::Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld)
{
	glUseProgram(m_shader->program);
	m_shader->EnableStates();

	glBindBuffer(GL_ARRAY_BUFFER, m_Model->GetVBOId());
	
	// Position of Object
	if(m_shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(m_shader->positionAttribute);
		glVertexAttribPointer(m_shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, pos));
	}
	// Normal vector of object
	if (m_shader->aNormal != -1)
	{
		glEnableVertexAttribArray(m_shader->aNormal);
		glVertexAttribPointer(m_shader->aNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, norm));
	}
	//Binormal vector
	if (m_shader->aBinormal != -1)
	{
		glEnableVertexAttribArray(m_shader->aBinormal);
		glVertexAttribPointer(m_shader->aBinormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, binorm));
	}
	//Tangent vector
	if (m_shader->aTangent != -1)
	{
		glEnableVertexAttribArray(m_shader->aTangent);
		glVertexAttribPointer(m_shader->aTangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, tangent));
	}
	// Color of Object
	if(m_shader->colorAttribute != -1)
	{
		glEnableVertexAttribArray(m_shader->colorAttribute);
		glVertexAttribPointer(m_shader->colorAttribute, 4,GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, color));
	}
	// WVP Matrix of Object
	if (m_shader->wvpMatrixUniform != -1)
	{
		Matrix wvpMatrix;
		wvpMatrix = m_worldMatrix * cameraMatrix * projectMatrix;
		glUniformMatrix4fv( m_shader->wvpMatrixUniform, 1, GL_FALSE, (GLfloat*) wvpMatrix.m);
	}
	// Textcoor of Object
	if (m_shader->uvLocation != -1 )
	{
		glEnableVertexAttribArray(m_shader->uvLocation);
		glVertexAttribPointer(m_shader->uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, uv));
	}
	// World Matrix of Object
	if (m_shader->uWorldMatrix != -1)
	{
		glUniformMatrix4fv(m_shader->uWorldMatrix, 1, GL_FALSE, (GLfloat*) m_worldMatrix.m);
	}
	// Position Camera In World
	if (m_shader->uPosCamW != -1)
	{
		float posCamW[] = {posCamWorld.x, posCamWorld.y, posCamWorld.z};
		glUniform3fv(m_shader->uPosCamW, 1, posCamW); 
	}

	//Bind texture 2D
	GLuint texture2DUnit = 0;
	for(texture2DUnit = 0; texture2DUnit < m_numOfTexture2D; texture2DUnit++)
	{
		glActiveTexture(GL_TEXTURE0 + texture2DUnit);
		glBindTexture(GL_TEXTURE_2D, m_listTexture2D[texture2DUnit]->GetTextureID());
		if (m_shader->uListTexturesLoc[texture2DUnit] != -1)
		{
			glUniform1i(m_shader->uListTexturesLoc[texture2DUnit], texture2DUnit);
		}
	}
	//Bind texture 3D
	for (GLuint texture3DUnit = 0; texture3DUnit < m_numOfTexture3D; texture3DUnit++)
	{
		glActiveTexture(GL_TEXTURE0 + texture2DUnit + texture3DUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_listTexture3D[texture3DUnit]->GetTextureID());
		//Cube Texture
		if (m_shader->uCubeTexture != -1)
		{
			glUniform1i(m_shader->uCubeTexture, texture2DUnit + texture3DUnit);
		}
	}

	//Fog
	if (m_shader->uFogColor != -1)
	{
		Vector4 v_fogColor = SceneManager::GetInstance()->GetFogColor();
		GLfloat fogColor[] = {v_fogColor.x, v_fogColor.y, v_fogColor.z, v_fogColor.w};
		glUniform4fv(m_shader->uFogColor, 1, fogColor);
	}
	if (m_shader->uFogStart != -1)
	{
		glUniform1f(m_shader->uFogStart, SceneManager::GetInstance()->GetFogStart());
	}
	if (m_shader->uFogRange != -1)
	{
		glUniform1f(m_shader->uFogRange, SceneManager::GetInstance()->GetFogRange());
	}
	//Time
	if (m_shader->uTime != -1)
	{
		//glDepthMask(GL_FALSE);
		glUniform1f(m_shader->uTime, SceneManager::GetInstance()->GetTotalGameTime());
	}
	//Phong Lighting
	if (m_numOfLights > 0)
	{
		if (m_shader->uAmbientColor != -1)
		{
			Vector4 ambientColor = SceneManager::GetInstance()->GetAmbientColor();
			glUniform4f(m_shader->uAmbientColor, ambientColor.x, ambientColor.y, ambientColor.z, ambientColor.w);
		}
		if (m_shader->uAmbientWeight != -1)
		{
			glUniform1f(m_shader->uAmbientWeight, SceneManager::GetInstance()->GetAmbientWeight());
		}
		for(unsigned int iLightUnit = 0 ; iLightUnit < m_numOfLights ; iLightUnit++)
		{
			if (m_shader->uLightLoc[iLightUnit].color != -1)
			{
				Vector4 lightColor = SceneManager::GetInstance()->GetLightByID(m_listLightID[iLightUnit]).GetColor();
				glUniform4f(m_shader->uLightLoc[iLightUnit].color, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			}
			if (m_shader->uLightLoc[iLightUnit].position != -1)
			{
				Vector3 lightPos = SceneManager::GetInstance()->GetLightByID(m_listLightID[iLightUnit]).GetPosition();
				glUniform3f(m_shader->uLightLoc[iLightUnit].position, lightPos.x, lightPos.y, lightPos.z);
			}
			if (m_shader->uLightLoc[iLightUnit].isDirection != -1)
			{
				bool isDirection = SceneManager::GetInstance()->GetLightByID(m_listLightID[iLightUnit]).GetIsDirection();
				glUniform1i(m_shader->uLightLoc[iLightUnit].isDirection, isDirection);
			}
		}
	}
	//Water
	if (m_shader->uDepthAjust != -1)
	{
		glUniform1f(m_shader->uDepthAjust, 1.0f/m_Model->GetMaxUV().x);
	}

	// Render
	if (m_Model->GetModelType() == MODEL_TYPE_NFG)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Model->GetIBOId());
		glDrawElements(GL_TRIANGLES, m_Model->GetNumOfIndices(), GL_UNSIGNED_INT, (GLvoid*)0);
	}
	else if (m_Model->GetModelType() == MODEL_TYPE_OBJ)
	{
		glDrawArrays(GL_TRIANGLES, 0, m_Model->GetNumOfVertexs());
	}
	else
	{
		printf("Error - Model type not implement\n");
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	for( GLuint texture2DUnit = 0; texture2DUnit < m_numOfTexture2D; texture2DUnit++)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	for (GLuint texture3DUnit = 0; texture3DUnit < m_numOfTexture3D; texture3DUnit++)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	if (m_Model->GetModelType() == MODEL_TYPE_NFG)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	m_shader->DisableStates();
}

void Object::CalculateWorldMatrix()
{
	if ( m_isChangeWorldMatrix == true)
	{
		Matrix scaleMatrix, translateMatrix, rotationXMatrix, rotationYMatrix, rotationZMatrix;
		scaleMatrix.SetScale(m_scale);
		translateMatrix.SetTranslation(m_position);
		rotationXMatrix.SetRotationX(m_rotation.x);
		rotationYMatrix.SetRotationY(m_rotation.y);
		rotationZMatrix.SetRotationZ(m_rotation.z);
		m_worldMatrix = scaleMatrix * rotationZMatrix * rotationXMatrix * rotationYMatrix * translateMatrix;
		m_isChangeWorldMatrix = false;
	}
}

void Object::SetModel(Model* i_model)
{
	m_Model = i_model;
}

void Object::SetShader(Shaders* i_shader)
{
	m_shader = i_shader;
}

void Object::SetTexture2D(unsigned int i_numTexture, Texture** i_listTexture)
{
	m_numOfTexture2D = i_numTexture;
	m_listTexture2D = i_listTexture;
}

void Object::SetTexture3D(unsigned int i_numTexture, Texture** i_listTexture)
{
	m_numOfTexture3D = i_numTexture;
	m_listTexture3D = i_listTexture;
}

void Object::SetPosition(Vector3 i_position)
{
	m_position = i_position;
	m_isChangeWorldMatrix = true;
}

void Object::SetPosition(GLfloat i_posX, GLfloat i_posY, GLfloat i_posZ)
{
	m_position = Vector3(i_posX, i_posY, i_posZ);
	m_isChangeWorldMatrix = true;
}

void Object::SetRotation(Vector3 i_rotation)
{
	m_rotation = i_rotation;
	m_isChangeWorldMatrix = true;
}

void Object::SetRotation(GLfloat i_rotX, GLfloat i_rotY, GLfloat i_rotZ)
{
	m_rotation = Vector3(i_rotX, i_rotY, i_rotZ);
	m_isChangeWorldMatrix = true;
}
void Object::SetRotationX(GLfloat i_rotX)
{
	m_rotation.x = i_rotX;
	m_isChangeWorldMatrix = true;
}
void Object::SetRotationY(GLfloat i_rotY)
{
	m_rotation.y = i_rotY;
	m_isChangeWorldMatrix = true;
}
void Object::SetRotationZ(GLfloat i_rotZ)
{
	m_rotation.z = i_rotZ;
	m_isChangeWorldMatrix = true;
}
void Object::SetScale(Vector3 i_scale)
{
	m_scale = i_scale;
	m_isChangeWorldMatrix = true;
}

void Object::SetScale(GLfloat i_scaleX, GLfloat i_scaleY, GLfloat i_scaleZ)
{
	m_scale = Vector3(i_scaleX, i_scaleY, i_scaleZ);
	m_isChangeWorldMatrix = true;
}

void Object::SetLights(GLuint i_numLight, GLuint* i_listLightID)
{
	m_numOfLights = i_numLight;
	m_listLightID = new GLuint[m_numOfLights];
	for(unsigned int i = 0 ; i < m_numOfLights ; i++)
	{
		m_listLightID[i] = i_listLightID[i];
	}
}

Vector3 Object::GetPosition()
{
	return m_position;
}

Vector3 Object::GetRotation()
{
	return m_rotation;
}

Vector3 Object::GetScale()
{
	return m_scale;
}
void Object::CleanUp()
{
	if (m_listTexture2D != NULL)
	{
		delete [] m_listTexture2D;
		m_listTexture2D = NULL;
	}
	if (m_listTexture3D != NULL)
	{
		delete [] m_listTexture3D;
		m_listTexture3D = NULL;
	}
	if (m_listLightID != NULL)
	{
		delete [] m_listLightID;
		m_listLightID = NULL;
	}
}

void Object::SetFrameModel(FrameModel* i_frM)
{
	p_frM = i_frM;
}

void Object::UpdateFrameModel()
{
	float temp;
	m_frameModel.max_x = m_position.x + (*p_frM).max_x * m_scale.x;
	m_frameModel.min_x = m_position.x + (*p_frM).min_x * m_scale.x;
	m_frameModel.max_y = m_position.y + (*p_frM).max_y * m_scale.y;
	m_frameModel.min_y = m_position.y + (*p_frM).min_y * m_scale.y;
	m_frameModel.max_z = m_position.z + (*p_frM).max_z * m_scale.z;
	m_frameModel.min_z = m_position.z + (*p_frM).min_z * m_scale.z;
	if (m_frameModel.max_x < m_frameModel.min_x)
	{
		temp = m_frameModel.max_x;
		m_frameModel.max_x = m_frameModel.min_x;
		m_frameModel.min_x = temp;
	}
	if (m_frameModel.max_y < m_frameModel.min_y)
	{
		temp = m_frameModel.max_y;
		m_frameModel.max_y = m_frameModel.min_y;
		m_frameModel.min_y = temp;
	}
	if (m_frameModel.max_z < m_frameModel.min_z)
	{
		temp = m_frameModel.max_z;
		m_frameModel.max_z = m_frameModel.min_z;
		m_frameModel.min_z = temp;
	}
}

FrameModel* Object::GetFrameModel()
{
	return &m_frameModel;
}