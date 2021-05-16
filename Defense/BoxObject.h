#pragma once
#include "Object.h"
class BoxObject : public Object
{
private:
	int m_type;
	bool m_isShow;
	Texture* m_healthTexture;
	Texture* m_ammoTexture;
	float m_timeToHaveBox;
public:
	BoxObject();
	~BoxObject();
	void Init();
	void Draw(Matrix &cameraMatrix, Matrix& projectMatrix, Vector3& posCamWorld);
	void Update(float deltaTime);
	bool IsColisionBullet(Vector3 posBullet);
	bool GetIsDisplay(){ return m_isShow; }
	void SetIsDisplay(bool i) { m_isShow = i; }
	void SetType(int type);
	int GetType() { return m_type; }
	void SetLocation(Vector3 i_pos);
	bool CanHaveBox();
};

