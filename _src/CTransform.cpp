#include "CTransform.h"
#include <cmath>

CTransform::CTransform(CVector3 trans, CVector3 rot, CVector3 scale)
	:
	m_translation(trans),
	m_scale(scale),
	m_rotation(rot)
{}

CTransform::~CTransform()
{}

CVector3& CTransform::GetRotation()
{
	return m_rotation;
}

CVector3& CTransform::GetTranslation()
{
	return m_translation;
}

CVector3& CTransform::GetScale()
{
	return m_scale;
}