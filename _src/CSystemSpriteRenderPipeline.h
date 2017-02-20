#pragma once

#include "CD3DGraphics.h"
#include "CSprite.h"

#define NSPRITE_RENDER_LAYERS 10

class CSystemSpriteRenderPipeline;

class CSystemSpriteRenderInfo
{
	friend CSystemSpriteRenderPipeline;
public:
	CSystemSpriteRenderInfo(CSprite sprite);
	~CSystemSpriteRenderInfo();

	void AddToQueue(CSystemSpriteRenderInfo* add);
	void Clear();

private:
	CSystemSpriteRenderInfo* m_pNext;
	CSprite sprite;
};

class CSystemSpriteRenderPipeline
{
public:
	CSystemSpriteRenderPipeline();
	~CSystemSpriteRenderPipeline();

	void AddToQueue(CSprite sprite, int layer);

	CSprite* GetNext(int layer);
	
private:
	CSystemSpriteRenderInfo** m_pFirst;
};