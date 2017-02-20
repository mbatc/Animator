#pragma once

#include "CComponent.h"
#include "CVector2.h"

#define NPIVOTS 15

#define PIVOT_TOP 0
#define PIVOT_BOTTOM 1
#define PIVOT_OTHER 2

#define CANIM_HEAD		"Head"
#define CANIM_TORSO		"Torso"
#define CANIM_BICEP_L	"Bicep_l"
#define CANIM_BICEP_R	"Bicep_r"
#define CANIM_FOREARM_L	"Forearm_l"
#define CANIM_FOREARM_R	"Forearm_r"
#define CANIM_HAND_L	"Hand_l"
#define CANIM_HAND_R	"Hand_r"
#define CANIM_THIGH_L	"Thigh_l"
#define CANIM_THIGH_R	"Thigh_r"
#define CANIM_CALF_L	"Calf_l"
#define CANIM_CALF_R	"Calf_r"
#define CANIM_FOOT_L	"Foot_l"
#define CANIM_FOOT_R	"Foot_r"

//The order of the ENUM is also the render order of the body parts
enum CANIM{
	HAND_L,
	THIGH_L,
	CALF_L,
	FOOT_L,
	BICEP_L,
	FOREARM_L,
	HEAD,
	TORSO,
	FOREARM_R,
	BICEP_R,
	FOOT_R,
	CALF_R,
	THIGH_R,
	HAND_R,
	SHOULDER,
	ERR
};

CANIM GetAnimPart(const char* str);
const char* GetAnimString(CANIM id);

void Rotate2DPoint(CVector2& v, CVector2 v1, CVector2 v2, float rot);
float GetAngle(CVector2 v1, CVector2 v2);

class CAnimPivot
{
public:
	CAnimPivot();
	CAnimPivot(CAnimPivot* parent, CVector2 point);
	void SET(CAnimPivot* parent, CAnimPivot* child, CVector2 point);

	void Update();

	CVector2 m_transformedPoint;
	CVector2 m_point;

	CAnimPivot* m_parent;
	CAnimPivot* m_child;

	float m_length;
	float m_rotation;
	float m_globalRotation;

	CAnimPivot* GetLastInBranch(int& nChildren);
};

struct SANIMSPRANCHOR
{
	CVector2 top;
	CVector2 bottom;
	CVector2 other;
};

class CCompActorAnim : public CGraphicsComponent
{
public:
	CCompActorAnim(char* filepath);
	
	bool InitGFX(CD3DGraphics& gfx);
	void Update(float deltaTime);
	void Render(CD3DGraphics& gfx);

	CAnimPivot* GetPivot(CANIM bodypart);
	CAnimPivot* GetPivot(const char* bodypart);
	CVector2    GetAnchor(CANIM bodypart, UINT type);

	float& GetY() { return m_y; }
	float& GetX() { return m_x; }
private:
	void SetDirectory(char* filepath);
	void LoadSprites(CD3DGraphics& gfx);
	void UpdateBody();
	void LoadActorInfo();

	float m_x, m_y;

	char* m_pFilepath;
	CAnimPivot	m_anchor;
	CAnimPivot* m_pPivot;
	CSprite*		m_pBody;
	SANIMSPRANCHOR*  m_pSprAnchor;

	bool m_isFlipped;
};