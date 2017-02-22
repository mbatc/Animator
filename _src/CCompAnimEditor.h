#pragma once

#include "CComponent.h"
#include "CCompActorAnim.h"
#include "CCompAnimController.h"

#include "CUI.h"

#define UI_PIVOT_MENU		0x1000u
#define UI_PIVOT_LIST		0x1001u

#define UI_PROP_MENU		0x2000u
#define UI_PROP_NAME		0x2001u
#define UI_PROP_LENGTH		0x2002u
#define UI_PROP_TIME		0x2003u
#define UI_PROP_CURTIME		0x2004u
#define UI_PROP_PLAY		0x2005u
#define UI_PROP_LOOP		0x2006u

#define UI_SAVE_MENU		0x3000u
#define UI_SAVE_BT			0x3001u
#define UI_LOAD_BT			0x3002u

#define UI_PIVPROP_MENU		0x4000u
#define UI_ADDKEYFRAME_BT	0x4001u
#define UI_DELETE_FRAME_BT	0x4002u
#define UI_DUP_FRAME_BT		0x4003u
#define UI_KEYFRAME_ROT		0x4004u
#define UI_KEYFRAME_TIME	0x4005u
#define UI_KEYFRAME_LIST	0x4006u


class CCompAnimEditor : public CComponent
{
public:
	CCompAnimEditor();
	
	void InitUI();

	void Update(float deltaTime);
	void Render(CD3DGraphics& gfx);

	void SetActor(CCompActorAnim* pActor);
	void SetController(CCompAnimController* pController);

	void UIMANAGER_MSG_PROC(UINT msg, UINT subMsg);

	char* m_pFilename;
private:
	void Save();
	void Load();
	void Play();

	void OnPivotChange();
	void OnKeyframeChange();

	void AddKeyframe();
	void DeleteKeyframe();
	void DuplicateKeyframe();

	char* OpenSaveDialog();
	char* OpenLoadDialog();

	bool m_bRotate;
	bool m_bPreview;

	CCompActorAnim* m_pActor;
	CCompAnimController* m_pController;
	
	CAnimation* m_pAnimation;

	CUIList* m_pPivotList;
	CUIList* m_pFramelist;

	CUIMenu* m_pPivotMenu;
	CUIMenu* m_pAnimProp;
	CUIMenu* m_pSaveMenu;
	CUIMenu* m_pPivotProp;

	CSprite m_pivot_a;
	CSprite m_pivot;
};