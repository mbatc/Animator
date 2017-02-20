#include "CCompAnimController.h"
#include "Helper.h"

CCompAnimController::CCompAnimController()
	:
	CComponent(CANIMCONTROL),
	m_bUpdatePivot(true)
{}

void CCompAnimController::Update(float deltaTime)
{
	CAnimation* a = &m_pAnim[m_activeAnim];
	a->Update(deltaTime);
	if (m_bUpdatePivot)
	{
		for (int i = 0; i < NPIVOTS; i++)
		{
			SKEYFRAME f = a->GetNextKeyFrame((CANIM)i);
			if (f.m_body == CANIM::ERR)
				continue;

			SKEYFRAME p = a->GetPreviousKeyFrame((CANIM)i);
			float dt = f.m_time - p.m_time;
			float ct = a->GetTime() - p.m_time;

			float r = 0.0f;
			if (dt <= 0)
				r = f.m_rotation;
			else
				r = (ct / dt)*(f.m_rotation - p.m_rotation) + p.m_rotation;

			m_pActor->GetPivot((CANIM)i)->m_rotation = r;
		}
	}
}

void CCompAnimController::Render(CD3DGraphics & gfx)
{}

void CCompAnimController::Loop(bool b)
{
	m_pAnim[m_nAnim - 1].Loop(b);
}

void CCompAnimController::AddAnimation(char * filename, char * name)
{
	MEMORY::LIST::AddToList(m_pAnim, new CAnimation(), m_nAnim);
	m_pAnim[m_nAnim - 1].LoadAnimation(filename);
	m_pAnim[m_nAnim - 1].SetName(name);
}

void CCompAnimController::SetAnimation(char * name)
{
	for (int i = 0; i < m_nAnim; i++)
	{
		if (!strcmp(m_pAnim[i].GetName(), name))
			m_activeAnim = i;
	}
}

void CCompAnimController::SetSpeed(float speed)
{
	for (int i = 0; i < m_nAnim; i++)
	{
		m_pAnim[i].SetSpeed(speed);
	}
}

float CCompAnimController::GetAnimRotation(CANIM a, float time)
{
	SKEYFRAME f = m_pAnim[m_activeAnim].GetNextKeyFrame(a, true, time);
	if (f.m_body == CANIM::ERR)
		return 0.0f;

	SKEYFRAME p = m_pAnim[m_activeAnim].GetPreviousKeyFrame(a, true, time);
	float dt = f.m_time - p.m_time;
	float ct = time - p.m_time;

	float r = 0.0f;
	if (dt <= 0)
		r = f.m_rotation;
	else
		r = (ct / dt)*(f.m_rotation - p.m_rotation) + p.m_rotation;

	return r;
}

CAnimation * CCompAnimController::GetAnimation(char * name)
{
	if (name == NULL)
		return &m_pAnim[m_activeAnim];
	for (int i = 0; i < m_nAnim; i++)
	{
		if (!strcmp(m_pAnim[i].GetName(), name))
			return &m_pAnim[i];
	}

	return NULL;
}
