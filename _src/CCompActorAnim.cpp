#include "CCompActorAnim.h"
#include "CGameobject.h"
#include "CSprite.h"
#include "Helper.h"

CANIM GetAnimPart(const char* str)
{
	if (!strcmp(str, CANIM_BICEP_L))
		return CANIM::BICEP_L;
	if (!strcmp(str, CANIM_CALF_L))
		return CANIM::CALF_L;
	if (!strcmp(str, CANIM_FOOT_L))
		return CANIM::FOOT_L;
	if (!strcmp(str, CANIM_FOREARM_L))
		return CANIM::FOREARM_L;
	if (!strcmp(str, CANIM_HAND_L))
		return CANIM::HAND_L;
	if (!strcmp(str, CANIM_THIGH_L))
		return CANIM::THIGH_L;
	if (!strcmp(str, CANIM_BICEP_R))
		return CANIM::BICEP_R;
	if (!strcmp(str, CANIM_CALF_R))
		return CANIM::CALF_R;
	if (!strcmp(str, CANIM_FOOT_R))
		return CANIM::FOOT_R;
	if (!strcmp(str, CANIM_FOREARM_R))
		return CANIM::FOREARM_R;
	if (!strcmp(str, CANIM_HAND_R))
		return CANIM::HAND_R;
	if (!strcmp(str, CANIM_THIGH_R))
		return CANIM::THIGH_R;
	if (!strcmp(str, CANIM_HEAD))
		return CANIM::HEAD;
	if (!strcmp(str, CANIM_TORSO))
		return CANIM::TORSO;

	return CANIM::ERR;
}

const char * GetAnimString(CANIM id)
{
	switch (id)
	{
	case CANIM::BICEP_L:
		return CANIM_BICEP_L;
	case CANIM::BICEP_R:
		return CANIM_BICEP_R;
	case CANIM::CALF_L:
		return CANIM_CALF_L;
	case CANIM::CALF_R:
		return CANIM_CALF_R;
	case CANIM::FOOT_L:
		return CANIM_FOOT_L;
	case CANIM::FOOT_R:
		return CANIM_FOOT_R;
	case CANIM::FOREARM_L:
		return CANIM_FOREARM_L;
	case CANIM::FOREARM_R:
		return CANIM_FOREARM_R;
	case CANIM::HAND_L:
		return CANIM_HAND_L;
	case CANIM::HAND_R:
		return CANIM_HAND_R;
	case CANIM::HEAD:
		return CANIM_HEAD;
	case CANIM::THIGH_L:
		return CANIM_THIGH_L;
	case CANIM::THIGH_R:
		return CANIM_THIGH_R;
	case CANIM::TORSO:
		return CANIM_TORSO;
	}
	return nullptr;
}

void Rotate2DPoint(CVector2& v, CVector2 v1, CVector2 v2, float rot)
{
	float s = sinf(rot);
	float c = cosf(rot);

	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;

	float xnew = v.x * c - v.y * s;
	float ynew = v.x * s + v.y * c;

	v.x = xnew + v2.x;
	v.y = ynew + v2.y;
}

float GetAngle(CVector2 v1, CVector2 v2)
{
	if (v1.x == v2.x && v1.y == v2.y)
		return 0.0f;

	CVector2 a(v2.x - v1.x, v2.y - v1.y);
	
	float cos_theta = a.x / a.Magnitude();
	return acosf(cos_theta);
}

CCompActorAnim::CCompActorAnim(char * filepath)
	:
	CGraphicsComponent(CACTORANIM),
	m_anchor(NULL,CVector2(0,0)),
	m_isFlipped(false)
{
	SetDirectory(filepath);
	m_x = 0.0f;
	m_y = 0.0f;
}

bool CCompActorAnim::InitGFX(CD3DGraphics & gfx)
{
	LoadActorInfo();
	LoadSprites(gfx);

	m_pPivot = new CAnimPivot[NPIVOTS];

	m_pPivot[CANIM::HEAD].SET(GetPivot(CANIM::TORSO), NULL,GetAnchor(CANIM::HEAD, PIVOT_TOP) - GetAnchor(CANIM::HEAD, PIVOT_BOTTOM));

	m_pPivot[CANIM::TORSO].SET(&m_anchor, NULL,GetAnchor(CANIM::TORSO, PIVOT_OTHER) - GetAnchor(CANIM::TORSO, PIVOT_BOTTOM));
	m_pPivot[CANIM::SHOULDER].SET(GetPivot(CANIM::TORSO),NULL,GetAnchor(CANIM::TORSO, PIVOT_TOP) - GetAnchor(CANIM::TORSO, PIVOT_OTHER));
	
	m_pPivot[CANIM::BICEP_L].SET(GetPivot(CANIM::SHOULDER), GetPivot(CANIM::FOREARM_L), GetAnchor(CANIM::BICEP_L, PIVOT_BOTTOM) - GetAnchor(CANIM::BICEP_L, PIVOT_TOP));
	m_pPivot[CANIM::FOREARM_L].SET(GetPivot(CANIM::BICEP_L), GetPivot(CANIM::HAND_L), GetAnchor(CANIM::FOREARM_L, PIVOT_BOTTOM) - GetAnchor(CANIM::FOREARM_L, PIVOT_TOP));
	m_pPivot[CANIM::HAND_L].SET(GetPivot(CANIM::FOREARM_L), NULL, GetAnchor(CANIM::HAND_L, PIVOT_BOTTOM) - GetAnchor(CANIM::HAND_L, PIVOT_TOP));

	m_pPivot[CANIM::BICEP_R].SET(GetPivot(CANIM::SHOULDER), GetPivot(CANIM::FOREARM_R), GetAnchor(CANIM::BICEP_R, PIVOT_BOTTOM) - GetAnchor(CANIM::BICEP_R, PIVOT_TOP));
	m_pPivot[CANIM::FOREARM_R].SET(GetPivot(CANIM::BICEP_R), GetPivot(CANIM::HAND_R), GetAnchor(CANIM::FOREARM_R, PIVOT_BOTTOM) - GetAnchor(CANIM::FOREARM_R, PIVOT_TOP));
	m_pPivot[CANIM::HAND_R].SET(GetPivot(CANIM::FOREARM_R), NULL, GetAnchor(CANIM::HAND_R, PIVOT_BOTTOM) - GetAnchor(CANIM::HAND_R, PIVOT_TOP));

	m_pPivot[CANIM::THIGH_L].SET(&m_anchor, GetPivot(CANIM::CALF_L), GetAnchor(THIGH_L, PIVOT_BOTTOM) - GetAnchor(THIGH_L, PIVOT_TOP));
	m_pPivot[CANIM::CALF_L].SET(GetPivot(THIGH_L), GetPivot(CANIM::FOOT_L), GetAnchor(CALF_L, PIVOT_BOTTOM) - GetAnchor(CALF_L, PIVOT_TOP));
	m_pPivot[CANIM::FOOT_L].SET(GetPivot(CALF_L), NULL, GetAnchor(FOOT_L, PIVOT_BOTTOM) - GetAnchor(FOOT_L, PIVOT_TOP));

	m_pPivot[CANIM::THIGH_R].SET(&m_anchor, GetPivot(CANIM::CALF_R), GetAnchor(THIGH_R, PIVOT_BOTTOM) - GetAnchor(THIGH_R, PIVOT_TOP));
	m_pPivot[CANIM::CALF_R].SET(GetPivot(THIGH_R), GetPivot(CANIM::FOOT_R), GetAnchor(CALF_R, PIVOT_BOTTOM) - GetAnchor(CALF_R, PIVOT_TOP));
	m_pPivot[CANIM::FOOT_R].SET(GetPivot(CALF_R), NULL, GetAnchor(FOOT_R, PIVOT_BOTTOM) - GetAnchor(FOOT_R, PIVOT_TOP));

	return true;
}

void CCompActorAnim::Update(float deltaTime)
{
	m_anchor.m_point = CVector2(m_parent->GetTransform().GetTranslation().x + m_x, 
		m_parent->GetTransform().GetTranslation().y + m_y);

	CAnimPivot* shoulder = GetPivot(CANIM::SHOULDER);
	CAnimPivot* torso = GetPivot(CANIM::TORSO);

	m_anchor.m_child = torso;
	torso->m_child = shoulder;
	shoulder->m_child = GetPivot(CANIM::BICEP_L);
	m_anchor.Update();
	shoulder->m_child = GetPivot(CANIM::BICEP_R);
	m_anchor.Update();

	torso->m_child = GetPivot(CANIM::HEAD);
	m_anchor.Update();


	m_anchor.m_child = GetPivot(CANIM::THIGH_L);
	m_anchor.Update();
	m_anchor.m_child = GetPivot(CANIM::THIGH_R);
	m_anchor.Update();

	UpdateBody();
}

void CCompActorAnim::Render(CD3DGraphics & gfx)
{
	if (m_isFlipped)
	{
		for (int i = NPIVOTS - 2; i >= 0; i--)
		{
			SYSTEM::RenderSprite(m_pBody[i], DEFAULT_LAYER - 1);
		}
	}
	else
	{
		for (int i = 0; i < NPIVOTS - 1; i++)
		{
			SYSTEM::RenderSprite(m_pBody[i], DEFAULT_LAYER - 1);
		}
	}
}

void CCompActorAnim::UpdateBody()
{
	for (int i = 0; i < NPIVOTS; i++)
	{
		if (!GetAnimString((CANIM)i))
			break;

		CAnimPivot* p = m_pPivot[i].m_parent;
		/*m_pBody[i].SetRotation(GetAngle(p->m_transformedPoint, 
			m_pPivot[i].m_transformedPoint));*/
		m_pBody[i].SetRotation(m_pPivot[i].m_globalRotation);
		
		//Need to load pivot location data before scaling can happen
		m_pBody[i].SetScaleX(1.0f);
		m_pBody[i].SetScaleY(1.0f);
		m_pBody[i].SetPosition(p->m_transformedPoint.x, p->m_transformedPoint.y);
	}
}

void CCompActorAnim::LoadActorInfo()
{
	MEMORY::SafeDeleteArr(m_pSprAnchor);
	m_pSprAnchor = new SANIMSPRANCHOR[NPIVOTS];

	char* filename = 0;
	STRING::Append(filename, m_pFilepath, "info.ini");
	if (!filename)
	{
		cout << "CCompActorAnim: Could not load Actor: No Filename Set\n";
		return;
	}

	FILE* pFile = fopen(filename, "r");

	if (!pFile)
	{
		cout << "CCompActorAnim: Could Not Open Animation Info file: " << filename << "\n";
		return;
	}

	do
	{
		if (!feof(pFile))
		{
			char* buffer = 0;
			FILEH::LOAD::SafeScanString(pFile, buffer);
			if (!buffer)
				break;

			CANIM t = GetAnimPart(buffer);
			if (t == CANIM::ERR)
				continue;

			float tx = 0, ty = 0, bx = 0, by = 0, ox = 0, oy = 0;

			fscanf(pFile, "%f %f %f %f %f %f", &tx, &ty, &bx, &by, &ox, &oy);
			
			if (tx < 0 && ty < 0)
			{
				tx = bx;
				ty = 0;
			}

			if (by < 0 && ty < 0)
			{
				by = tx;
				by = m_pBody[t].GetHeight();
			}

			m_pSprAnchor[t].bottom = CVector2(bx, by);
			m_pSprAnchor[t].top = CVector2(tx, ty);
			m_pSprAnchor[t].other = CVector2(ox, oy);
		}
	} while (!feof(pFile));
}

CAnimPivot * CCompActorAnim::GetPivot(CANIM bodypart)
{
	return &m_pPivot[bodypart];
}

CAnimPivot * CCompActorAnim::GetPivot(const char * bodypart)
{
	CANIM i = GetAnimPart(bodypart);
	if (!i)
		return NULL;
	return &m_pPivot[i];
}

CVector2 CCompActorAnim::GetAnchor(CANIM bodypart, UINT type)
{
	switch (type)
	{
	case PIVOT_TOP:
		return m_pSprAnchor[bodypart].top;
	case PIVOT_BOTTOM:
		return m_pSprAnchor[bodypart].bottom;
	case PIVOT_OTHER:
		return m_pSprAnchor[bodypart].other;
	}
	return CVector2(0.0f,0.0f);
}

void CCompActorAnim::SetDirectory(char * filepath)
{
	MEMORY::SafeDeleteArr(m_pFilepath);
	if (!filepath)
		return;
	
	STRING::Copy(m_pFilepath, filepath);
}

void CCompActorAnim::LoadSprites(CD3DGraphics& gfx)
{
	if (!m_pFilepath)
		return;

	MEMORY::SafeDeleteArr(m_pBody);
	m_pBody = new CSprite[NPIVOTS - 1];
	for (int i = 0; i < NPIVOTS - 1; i++)
	{
		char* filename = 0;
		STRING::Append(filename, m_pFilepath, GetAnimString((CANIM)i));
		STRING::Append(filename, filename, ".png");

		m_pBody[i].Init(filename,gfx);
		m_pBody[i].SetAnchor(m_pSprAnchor[i].top.x, m_pSprAnchor[i].top.y);

		if (i == CANIM::TORSO || i == CANIM::HEAD)
			m_pBody[i].SetAnchor(m_pSprAnchor[i].bottom.x, m_pSprAnchor[i].bottom.y);
	}
}


CAnimPivot::CAnimPivot()
	:
	m_point(0.0f,0.0f),
	m_parent(0),
	m_child(0),
	m_rotation(0.0f)
{}

CAnimPivot::CAnimPivot(CAnimPivot * parent, CVector2 point)
{
	m_point = point;
	m_parent = parent;
	m_rotation = 0.0f;
	m_child = NULL;
}

void CAnimPivot::SET(CAnimPivot * parent, CAnimPivot * child, CVector2 point)
{
	m_parent = parent;
	m_child = child;
	m_point = point;
}

void CAnimPivot::Update()
{
	m_length = sqrtf(m_point.x*m_point.x + m_point.y*m_point.y);

	if (m_child)
		m_child->Update();
	m_transformedPoint = m_point;
	if(m_parent)
		Rotate2DPoint(m_transformedPoint, m_transformedPoint, CVector2(0.0f, 0.0f), m_rotation);
	m_globalRotation = m_rotation;

	int nChildren = 0;
	CAnimPivot* last = GetLastInBranch(nChildren);

	for (int i = nChildren - 1; i >= 0; i--)
	{
		CAnimPivot* p = last->m_parent;
		Rotate2DPoint(last->m_transformedPoint, last->m_transformedPoint, CVector2(0.0f, 0.0f), m_rotation);

		last->m_transformedPoint.x += m_transformedPoint.x;
		last->m_transformedPoint.y += m_transformedPoint.y;
		last->m_globalRotation += m_rotation;

		last = p;
	}
}

CAnimPivot * CAnimPivot::GetLastInBranch(int & nChildren)
{
	if (m_child)
	{
		nChildren++;
		return m_child->GetLastInBranch(nChildren);
	}
	return this;
}
