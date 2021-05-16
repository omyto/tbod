#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Shaders.h"
#include "Texture.h"
#include "Model.h"
#include "Vertex.h"
#include <stddef.h>
class Object
{
protected:
	Model* m_Model;
	Shaders* m_shader;
	unsigned int m_numOfTexture2D;
	unsigned int m_numOfTexture3D;
	Texture** m_listTexture2D;
	Texture** m_listTexture3D;
	GLuint* m_listLightID;
	GLuint m_numOfLights;
	Matrix m_worldMatrix;
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	bool m_isChangeWorldMatrix;
	FrameModel *p_frM, m_frameModel;
public:
	Object(void);
	~Object(void);
	void Init();
	void Update(float deltaTime);
	void Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld);
	void CalculateWorldMatrix();
	void SetModel(Model* i_model);
	void SetShader(Shaders* i_shader);
	void SetTexture2D(unsigned int i_numTexture, Texture** i_listTexture);
	void SetTexture3D(unsigned int i_numTexture, Texture** i_listTexture);
	void SetPosition(Vector3 i_position);
	void SetPosition(GLfloat i_posX, GLfloat i_posY, GLfloat i_posZ);
	void SetRotation(Vector3 i_rotation);
	void SetRotation(GLfloat i_rotX, GLfloat i_rotY, GLfloat i_rotZ);
	void SetRotationX(GLfloat i_rotX);
	void SetRotationY(GLfloat i_rotY);
	void SetRotationZ(GLfloat i_rotZ);
	void SetScale(Vector3 i_scale);
	void SetScale(GLfloat i_scaleX, GLfloat i_scaleY, GLfloat i_scaleZ);
	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetScale();
	void CleanUp();
	void SetLights(GLuint i_numLight, GLuint* i_listLightID);
	void SetFrameModel(FrameModel* i_frM);
	void UpdateFrameModel();
	FrameModel* GetFrameModel();
};
