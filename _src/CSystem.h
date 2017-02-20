#pragma once

#include <Windows.h>

#include "CSystemMessagePipeline.h"
#include "CSystemResourceManager.h"
#include "CGraphicsInitBuffer.h"
#include "CWindow.h"
#include "CD3DGraphics.h"
#include "CUIMenu.h"
#include "CUIManager.h"

#include "CKeyboardInput.h"
#include "CMouseInput.h"

#include "CGameSystemLogic.h"
#include "CGameSystemRender.h"
#include "CGameSystemControlScheme.h"
#include "CTextRenderer.h"
#include "CSystemSpriteRenderPipeline.h"

#include "CTimer.h"

struct GlobalSetting
{
	GlobalSetting()
	{
		bDisplayNormals = false;
		bLockMouse = false;
		bWireframe = false;
		bGrid = false;
		bEditSnap = true;
		bDevTools = false;
	}

	bool bDisplayNormals;
	bool bWireframe;
	bool bLockMouse;
	bool bGrid;
	bool bEditSnap;
	bool bDevTools;
	bool bDevLevelTest;
};

class CSystem
{
public:
	CSystem();
	~CSystem();

	bool Init(HINSTANCE hInstance, LPSTR pCmdLine);
	
	int Run();

	CWindow& GetWindow() { return m_mainWindow; }

	LRESULT SystemWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	CGameSystemData* GetGameData();

	static CSystemMessagePipeline*		g_MessagePipeline;
	static CGraphicsInitBuffer*			g_gfxInitPump;
	static CSystemResourceManager*		g_resourceManager;
	static CKeyboardInput				g_keyboardInput;
	static CMouseInput					g_mouseInput;
	static CGameSystemControlScheme		g_controlScheme;
	static CSystemSpriteRenderPipeline	g_spriteRenderPipeline;
	static CTextRenderer				g_textRenderer;
	static CUIManager					g_uiManager;
	static CD3DGraphics					m_gfx;
	static GlobalSetting				g_properties;
	static CTimer						g_frameTimer;

private:
	bool SystemCreateWindow(HINSTANCE hInstance);
	bool DoGameProcess();

	unsigned int GetNextMessage();
	void ProccessAllMessages();
	bool SystemMessageProc(unsigned int msg, unsigned int subMsg);

	CWindow m_mainWindow;

	CUIMenu				m_testMenu;
	CGameSystemData		m_gameData;
	CGameSystemLogic*	m_pGameLogic;
	CGameSystemRender*	m_pGameRenderer;
	CKeyboardInputState m_KeyboardInputState;
	CMouseInputState	m_mouseInputState;
	CFont				m_testFont;

	int	m_lastFrameAvg;
	double	m_totalFrameTime;
};