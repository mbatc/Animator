#include "CGameSystemRender.h"
#include "CComponent.h"
#include "CSystem.h"
#include "Helper.h"

CGameSystemRender::CGameSystemRender(CGameSystemData& gameData, CD3DGraphics& gfx)
	:
	m_gameData(gameData),
	m_gfx(gfx)
{}

CGameSystemRender::~CGameSystemRender()
{}

bool CGameSystemRender::doFrame()
{
	if(!doObjects())
		return false;
	do2DRender();

	return true;
}

bool CGameSystemRender::doObjects()
{
	return m_gameData.m_pGameScene->Render(m_gfx);
}

bool CGameSystemRender::do2DRender()
{
	for (int i = 0; i < NSPRITE_RENDER_LAYERS; i++)
	{
		while (true)
		{
			CSprite* sprite = CSystem::g_spriteRenderPipeline.GetNext(i);
			if (!sprite)
				break;

			sprite->Render(m_gfx);
			MEMORY::SafeDelete(sprite);
		}
	}

	return true;
}