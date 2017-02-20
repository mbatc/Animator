#include "CAnimation.h"
#include "Helper.h"

CAnimation::CAnimation()
	:
	m_end_time(0.0f),
	m_bLoop(false),
	m_current_time(2.5f),
	m_nKeyframe(0),
	m_pAnimName(0),
	m_pFilename(0),
	m_speed(1.0f)
{}

void CAnimation::Update(float deltaTime)
{
	if(m_current_time < m_end_time)
		m_current_time += deltaTime * m_speed;	

	if (m_bLoop)
		if (m_current_time >= m_end_time)
			m_current_time = 0.0f;
}

void CAnimation::Loop(bool b)
{
	m_bLoop = b;
}

void CAnimation::LoadAnimation(char * filename)
{
	if (!filename)
		return;
	STRING::Copy(m_pFilename, filename);

	FILE* pFile = fopen(m_pFilename, "r");
	if (!pFile)
		return;

	MEMORY::SafeDeleteArr(m_pKeyframe);
	m_nKeyframe = 0;

	do
	{
		if (feof(pFile))
			break;

		char* buffer = 0; FILEH::LOAD::SafeScanString(pFile, buffer);
		if (!strcmp(buffer, "kf"))
		{
			SKEYFRAME k;
			char* part = 0; FILEH::LOAD::SafeScanString(pFile, part);
			float r = 0.0f;
			float t = 0.0f;
			fscanf(pFile, "%f %f", &r, &t);

			k.m_rotation = r;
			k.m_time = t;
			k.m_body = GetAnimPart(part);

			AddKeyframe(k);
		}
		else if (!strcmp(buffer, "t"))
		{
			fscanf(pFile, "%f", &m_end_time);
		}

	} while (!feof(pFile));
}

void CAnimation::SetName(char * name)
{
	if (name)
		STRING::Copy(m_pAnimName, name);
	else
		STRING::Copy(m_pAnimName, "null");
}

const char * CAnimation::GetName()
{
	return m_pAnimName;
}

SKEYFRAME CAnimation::GetNextKeyFrame(CANIM part, bool first, float t)
{
	if (t < 0)
		t = m_current_time;
	SKEYFRAME frame = SKEYFRAME();
	frame.m_body = CANIM::ERR;

	float time = m_end_time + 1.0f;
	for (int i = 0; i < m_nKeyframe; i++)
	{
		if (m_pKeyframe[i].m_body == part)
		{
			if (m_pKeyframe[i].m_time >= t && m_pKeyframe[i].m_time < time)
			{
				time = m_pKeyframe[i].m_time;
				frame = m_pKeyframe[i];
			}
		}
	}

	if (frame.m_body == CANIM::ERR && first)
		frame = GetPreviousKeyFrame(part, false);

	return frame;
}

SKEYFRAME CAnimation::GetPreviousKeyFrame(CANIM part, bool first, float t)
{
	if (t < 0)
		t = m_current_time;
	SKEYFRAME frame = SKEYFRAME();
	frame.m_body = CANIM::ERR;

	float time = -1.0f;
	for (int i = 0; i < m_nKeyframe; i++)
	{
		if (m_pKeyframe[i].m_body == part)
		{
			if (m_pKeyframe[i].m_time <= t && m_pKeyframe[i].m_time > time)
			{
				time = m_pKeyframe[i].m_time;
				frame = m_pKeyframe[i];
			}
		}
	}

	if (frame.m_body == CANIM::ERR && first)
		frame = GetNextKeyFrame(part, false);

	return frame;
}

float CAnimation::GetTime()
{
	return m_current_time;
}

void CAnimation::SetTime(float t)
{
	m_current_time = t;
}

void CAnimation::SetSpeed(float t)
{
	m_speed = t;
}

void CAnimation::SetLength(float t)
{
	m_end_time = t;
}

void CAnimation::AddKeyframe(SKEYFRAME f)
{
	MEMORY::LIST::AddToList(m_pKeyframe, &f, m_nKeyframe);
}

int CAnimation::GetNKeyframe(CANIM a)
{
	int n = 0;
	for (int i = 0; i < m_nKeyframe; i++)
	{
		if (m_pKeyframe[i].m_body == a)
			n++;
	}
	return n;
}

SKEYFRAME * CAnimation::GetKeyframe(CANIM a, int i)
{
	int n = GetNKeyframe(a);

	if (i > n && i < 0)
		return NULL;
	
	n = -1;
	for (int ii = 0; ii < m_nKeyframe; ii++)
	{
		if (m_pKeyframe[ii].m_body == a)
			n++;

		if (n == i)
			return &m_pKeyframe[ii];
	}

	return NULL;
}

int CAnimation::GetKeyframeIndex(SKEYFRAME * kf)
{
	for (int i = 0; i < m_nKeyframe; i++)
	{
		if (&m_pKeyframe[i] == kf)
			return i;
	}
	return -1;
}

void CAnimation::DeleteKeyframe(int index)
{
	MEMORY::LIST::RemoveItem(m_pKeyframe, m_nKeyframe, index);
}

