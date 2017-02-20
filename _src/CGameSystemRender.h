#pragma once

#include "CGameSystemData.h"
#include "CD3DGraphics.h"
#include "CSprite.h"
#include "CTextRenderer.h"

class CGameSystemRender
{
public:
	CGameSystemRender(CGameSystemData& m_gameData, CD3DGraphics& gfx);
	~CGameSystemRender();

	bool doFrame();
private:
	bool doObjects();
	bool do2DRender();

	CGameSystemData& m_gameData;
	CD3DGraphics& m_gfx;

	CSprite testSprite;
};