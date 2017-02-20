#include "CSystem.h"
#include "Helper.h"
#include "SystemMessage.h"
#include "CFontFileContainer.h"

#define TESTMENU 1

CSystemMessagePipeline*		CSystem::g_MessagePipeline = new CSystemMessagePipeline;
CGraphicsInitBuffer*		CSystem::g_gfxInitPump = new CGraphicsInitBuffer;
CSystemResourceManager*		CSystem::g_resourceManager = new CSystemResourceManager;
CKeyboardInput				CSystem::g_keyboardInput;
CMouseInput					CSystem::g_mouseInput;
CGameSystemControlScheme	CSystem::g_controlScheme;
CSystemSpriteRenderPipeline	CSystem::g_spriteRenderPipeline;
CTextRenderer				CSystem::g_textRenderer;
CUIManager					CSystem::g_uiManager;
GlobalSetting				CSystem::g_properties;
CD3DGraphics				CSystem::m_gfx;
CTimer						CSystem::g_frameTimer;

CSystem::CSystem()
	:
	m_lastFrameAvg(0),
	m_totalFrameTime(0.0)
{}

CSystem::~CSystem()
{}

bool CSystem::Init(HINSTANCE hInstance, LPSTR pCmdLine)
{
	PUMP::g_system = this;

	if (strstr(pCmdLine, "md_dev")) {
		CSystem::g_properties.bDevTools = true;

		cout << "Command: md_dev - Developer Mode Enabled...\n";
	}

	if(!SystemCreateWindow(hInstance))
		return false;
	if(!m_gfx.Init(m_mainWindow.GetHandle(),m_mainWindow.GetWidth(),m_mainWindow.GetHeight()))
		return false;

	CFontFileContainer container;
	container.SetFilename("Engine\\Text Engine\\SysFont.tif");
	container.LoadCurrentFile(m_gfx);
	container.GetLoadedFont(m_testFont);

	g_textRenderer.Init(&m_gfx);
	g_textRenderer.SetFont(&m_testFont);

	g_uiManager.SetSystem(this);
	g_resourceManager->Init(&m_gfx);

	m_gfx.DEFAULTSPRITE.Init("Engine\\Resources\\DEFAULT.bmp", m_gfx);

	m_gameData.InitData("");
	m_pGameLogic = new CGameSystemLogic(m_gameData);
	m_pGameRenderer = new CGameSystemRender(m_gameData, m_gfx);
	g_keyboardInput.SetKeyboardState(&m_KeyboardInputState);
	g_mouseInput.SetMouseState(&m_mouseInputState);

	return true;
}

bool CSystem::SystemCreateWindow(HINSTANCE hInstance)
{
	m_mainWindow.SetWindowClassName("GameWindowClass");
	m_mainWindow.SetWindowName("Game Window");
	m_mainWindow.SetWindowStyle(WS_OVERLAPPEDWINDOW);

	if (!m_mainWindow.Init(hInstance, GetSystemMetrics(SM_CXSCREEN) / 1.2, GetSystemMetrics(SM_CYSCREEN) / 1.2))
	{
		MessageBox(NULL, "Failed To Initialize Window!","Error!",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if(!m_mainWindow.CWndCreateWindow() )
	{
		MessageBox(NULL, "Failed To Create Window!","Error!",
			MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	SYSTEM::AddMessage(_SM_WINDOW_CREATED, _SM_NO_MSG);

	return true;
}

int CSystem::Run()
{
	MSG msg;
	int ReturnCode = 0;
	HWND hwnd = m_mainWindow.GetHandle();
	while(GetNextMessage() != _SM_QUIT_GAME)
	{
		g_frameTimer.Start();
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (GetNextMessage() != 0)
		{
			ProccessAllMessages();
		}
		else if (g_gfxInitPump->MessageInQueue())
		{
			g_gfxInitPump->ProcessNext(m_gfx);
		}
		else
		{
			if(!DoGameProcess())
			{
				MessageBox(NULL,"Something Went Wrong!","Error!",MB_OK | MB_ICONEXCLAMATION);
				ReturnCode = -1;
				break;
			}	

			m_KeyboardInputState.Update();
			m_mouseInputState.Update();
			g_frameTimer.End();
		}
	}

	return ReturnCode;
}

LRESULT CSystem::SystemWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch(msg)
	{
	case WM_QUIT:
		PostQuitMessage(0);
		SYSTEM::AddMessage(_SM_QUIT_GAME,_SM_NO_MSG);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		SYSTEM::AddMessage(_SM_QUIT_GAME, _SM_NO_MSG);
		break;
	case WM_KEYDOWN:
		m_KeyboardInputState.OnKeyPressed(wParam);
		break;
	case WM_KEYUP:
		m_KeyboardInputState.OnKeyReleased(wParam);
		break;
	case WM_CHAR:
		m_KeyboardInputState.OnCharacterPressed(wParam);
		break;
	case WM_MOUSEMOVE:
		m_mouseInputState.SetMouse(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		m_mouseInputState.OnLeftMousePressed();
		break;
	case WM_LBUTTONUP:
		m_mouseInputState.OnLeftMouseRelesed();
		break;
	case WM_RBUTTONDOWN:
		m_mouseInputState.OnRightMousePressed();
		break;
	case WM_RBUTTONUP:
		m_mouseInputState.OnRightMouseReleased();
		break;
	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}

bool CSystem::DoGameProcess()
{
	char msg[256];

	m_totalFrameTime += g_frameTimer.GetTime();

	if (g_frameTimer.GetNTimers() == 60)
	{
		m_lastFrameAvg = (CLOCKS_PER_SEC / m_totalFrameTime) * 60;
		g_frameTimer.ResetNTimers();
		m_totalFrameTime = 0.0;
	}
	m_pGameLogic->doFrame(g_frameTimer.GetTime());
	g_uiManager.Update();

	m_gfx.BeginFrame();

	g_uiManager.Render(m_gfx);
	m_pGameRenderer->doFrame();
	m_gfx.EndFrame();

	return true;
}

unsigned int CSystem::GetNextMessage()
{
	unsigned int msg, subMsg;
	g_MessagePipeline->GetSystemMessage(&msg, &subMsg);
	if(msg != 0)
		g_MessagePipeline->RemoveMessage();

	SystemMessageProc(msg, subMsg);

	return msg;
}

void CSystem::ProccessAllMessages()
{
	while(true)
	{
		unsigned int msg = GetNextMessage();
		if(!msg)
			break;
	}
}

bool CSystem::SystemMessageProc(unsigned int msg, unsigned int subMsg)
{
	switch (msg)
	{
	case _SM_QUIT_GAME_ERROR:
		MessageBox(NULL,"An Error Occured In The Game!","Exiting Application",
			MB_OK | MB_ICONEXCLAMATION);
		SYSTEM::AddMessage(_SM_QUIT_GAME, _SM_NO_MSG);
		return true;
	case _SM_WINDOW_RESIZE:
		return true;
	case _SM_NO_MSG:
		return true;
	default:
		g_uiManager.MessageProc(msg, subMsg);
	}
	return false;
}

CGameSystemData* CSystem::GetGameData()
{
	return &m_gameData;
}