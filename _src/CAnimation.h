#pragma once

#include "CVector2.h"
#include "CCompActorAnim.h"

struct SKEYFRAME
{
	CANIM m_body;
	float m_time;
	float m_rotation;
};

class CAnimation
{
public:
	CAnimation();

	void Update(float deltaTime);
	void Loop(bool b);
	void LoadAnimation(char* filename);

	void SetName(char* name);
	const char* GetName();

	SKEYFRAME GetNextKeyFrame(CANIM part, bool first = true, float time = -1.0f);
	SKEYFRAME GetPreviousKeyFrame(CANIM part, bool first = true, float time = -1.0f);

	float GetTime();
	void SetTime(float t);
	void SetSpeed(float t);
	void SetLength(float t);

	void AddKeyframe(SKEYFRAME f);

	int GetNKeyframe(CANIM a);
	int GetTotalKeyframeCount() { return m_nKeyframe; }
	SKEYFRAME* GetKeyframe(CANIM a, int i);
	int GetKeyframeIndex(SKEYFRAME* kf);

	SKEYFRAME* GetKeyframe(int i) { return &m_pKeyframe[i]; }
	float GetLength() { return m_end_time; }

	void DeleteKeyframe(int index);
private:
	SKEYFRAME* m_pKeyframe;
	int m_nKeyframe;

	float m_current_time;
	float m_end_time;

	float m_speed;

	char* m_pAnimName;
	char* m_pFilename;

	bool m_bLoop;
};