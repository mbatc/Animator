#pragma once

#include "CVector3.h"

class CTransform
{
public:
	CTransform(CVector3 trans = CVector3(0.0f,0.0f,0.0f), 
		CVector3 rot = CVector3(0.0f,0.0f,0.0f), 
		CVector3 scale = CVector3(1.0f,1.0f,1.0f));
	~CTransform();

	CVector3& GetTranslation();
	CVector3& GetRotation();
	CVector3& GetScale();

private:
	CVector3 m_translation;
	CVector3 m_rotation;
	CVector3 m_scale;
};