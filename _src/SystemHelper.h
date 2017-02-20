#pragma once
#include "CSystemResourceManager.h"
#include "CGameSystemControlScheme.h"
#include "CTexture.h"
#include "CGameobject.h"
#include "CComponent.h"
#include "CSprite.h"

class CGameSystemData;

namespace SYSTEM
{
	void AddMessage(unsigned int msg, unsigned int subMsg, bool notify = false);
	void AddGraphicsInit(CGraphicsComponent* component);

	void RenderText(char* text, int x, int y, float scale = 1.0f, RECT* lock = NULL);
	void RenderText(char* text, float x, float y, float scale = 1.0f, RECT* lock = NULL);
	void SetTextColor(D3DXVECTOR4 color);
	void RenderSprite(CSprite sprite, int layer);

	namespace RESOURCE
	{
		CTexture* GetTexture(char* filename);
		CGameobject GetGameobject(char* filename);

		bool DoesResourceExist(SYSTEM_RESOURCE_TYPE type, char* filename);
	}

	int GetWindowHeight();
	int GetWindowWidth();

	HWND GetMainWindowHandle();

	CGameSystemData* GetGMData();
}

namespace KEYBOARD
{
	bool IsDown(UINT keyCode);
	bool IsUp(UINT keyCode);
	bool IsReleased(UINT keyCode);
	bool IsPressed(UINT keyCode);
	const char* GetInBuffer();
}

namespace MOUSE
{
	bool IsLeftMouseDown();
	bool IsRightMouseDown();
	bool IsLeftMousePressed();
	bool IsRightMousePressed();
	bool IsLeftMouseReleased();
	bool IsRightMouseReleased();

	bool IsInRegion(RECT r);

	int GetMouseX();
	int GetMouseY();

	int GetMousePrevX();
	int GetMousePrevY();

	int GetChangeX();
	int GetChangeY();

	void SetPosition(int x, int y);
}

CGameSystemControlScheme Control();