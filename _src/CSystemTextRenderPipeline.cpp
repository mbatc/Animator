#include "CSystemSpriteRenderPipeline.h"

#include "StringHelper.h"
#include "MemoryHelper.h"

CSystemSpriteRenderPipeline::CSystemSpriteRenderPipeline()
	:
	m_pFirst(NULL)
{
	m_pFirst = new CSystemSpriteRenderInfo*[NSPRITE_RENDER_LAYERS];

	for (int i = 0; i < NSPRITE_RENDER_LAYERS; i++)
	{
		m_pFirst[i] = NULL;
	}
}

CSystemSpriteRenderPipeline::~CSystemSpriteRenderPipeline()
{
	for (int i = 0; i < NSPRITE_RENDER_LAYERS; i++)
	{
		if (m_pFirst[i])
			m_pFirst[i]->Clear();
	}

	MEMORY::SafeDeleteArr(m_pFirst);
}

void CSystemSpriteRenderPipeline::AddToQueue(CSprite sprite, int layer)
{
	if (!sprite.WithinLock())
		return;

	if (layer < 0)
		return;
	if (layer >= NSPRITE_RENDER_LAYERS)
		return;

	if (!m_pFirst[layer])
		m_pFirst[layer] = new CSystemSpriteRenderInfo(sprite);
	else
		m_pFirst[layer]->AddToQueue(new CSystemSpriteRenderInfo(sprite));
}

CSprite * CSystemSpriteRenderPipeline::GetNext(int layer)
{
	if (m_pFirst[layer])
	{
		CSprite* ret = new CSprite(m_pFirst[layer]->sprite);

		CSystemSpriteRenderInfo* temp = m_pFirst[layer]->m_pNext;

		delete m_pFirst[layer];
		m_pFirst[layer] = temp;

		return ret;
	}

	return NULL;
}

CSystemSpriteRenderInfo::CSystemSpriteRenderInfo(CSprite sprite)
	:
	sprite(sprite),
	m_pNext(NULL)
{}

CSystemSpriteRenderInfo::~CSystemSpriteRenderInfo()
{}

void CSystemSpriteRenderInfo::AddToQueue(CSystemSpriteRenderInfo * add)
{
	if (m_pNext)
		m_pNext->AddToQueue(add);
	else
		m_pNext = add;
}

void CSystemSpriteRenderInfo::Clear()
{
	if (m_pNext)
		m_pNext->Clear();
		
	MEMORY::SafeDelete(m_pNext);
}
