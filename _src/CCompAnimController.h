#pragma once

#include "CComponent.h"
#include "CAnimation.h"

class CCompAnimController : public CComponent
{
public:
	CCompAnimController();
	
	void Update(float deltaTime);
	void Render(CD3DGraphics& gfx);

	void Loop(bool b);
	void UpdatePivot(bool b) { m_bUpdatePivot = b; }
	void AddAnimation(char* filename, char* name);
	void SetAnimation(char* name);
	void SetSpeed(float speed);
	void SetActor(CCompActorAnim* actor) { m_pActor = actor; }

	float GetAnimRotation(CANIM a, float time);
	CAnimation* GetAnimation(char* name = NULL);
private:
	CAnimation*		m_pAnim;
	CCompActorAnim* m_pActor;

	bool m_bUpdatePivot;

	int m_nAnim;
	int m_activeAnim;
};