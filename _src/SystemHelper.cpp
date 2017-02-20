#include "SystemHelper.h"
#include "CGameSystemData.h"

#include "CSystem.h"

void SYSTEM::AddMessage(unsigned int msg, unsigned int subMsg, bool notify)
{
	CSystem::g_MessagePipeline->AddMessage(msg, subMsg,notify);
}

void SYSTEM::AddGraphicsInit(CGraphicsComponent* gfxComp)
{
	CSystem::g_gfxInitPump->AddToBuffer(gfxComp);
}

void SYSTEM::RenderText(char * text, int x, int y, float scale, RECT* lock)
{
	if (!text)
		return;
	CSystem::g_textRenderer.RenderText(text, x, y, scale, lock);
}

void SYSTEM::RenderText(char * text, float x, float y, float scale, RECT* lock)
{
	if (!text)
		return;
	CSystem::g_textRenderer.RenderText(text, x, y, scale, lock);
}

void SYSTEM::SetTextColor(D3DXVECTOR4 color)
{
	CSystem::g_textRenderer.SetColor(color);
}

void SYSTEM::RenderSprite(CSprite sprite, int layer)
{
	CSystem::g_spriteRenderPipeline.AddToQueue(sprite, layer);
}

int SYSTEM::GetWindowHeight()
{
	if (!PUMP::g_system)
		return -1;
	return PUMP::g_system->GetWindow().GetHeight();
}

int SYSTEM::GetWindowWidth()
{
	if (!PUMP::g_system)
		return -1;
	return PUMP::g_system->GetWindow().GetWidth();
}

HWND SYSTEM::GetMainWindowHandle()
{
	if (!PUMP::g_system)
		return HWND();
	return PUMP::g_system->GetWindow().GetHandle();
}

CGameSystemData * SYSTEM::GetGMData()
{
	return PUMP::g_system->GetGameData();
}

CTexture* SYSTEM::RESOURCE::GetTexture(char* filename)
{
	return CSystem::g_resourceManager->GetTexture(filename);
}
bool SYSTEM::RESOURCE::DoesResourceExist(SYSTEM_RESOURCE_TYPE type, char * filename)
{
	if (CSystem::g_resourceManager->DoesResourceExist(type, filename) < 0)
		return false;

	return true;
}

CGameobject SYSTEM::RESOURCE::GetGameobject(char* filename)
{
	return CSystem::g_resourceManager->GetGameobject(filename);
}

bool KEYBOARD::IsDown(UINT keyCode)
{
	return CSystem::g_keyboardInput.IsDown(keyCode);
}
bool KEYBOARD::IsUp(UINT keyCode)
{
	return !CSystem::g_keyboardInput.IsDown(keyCode);
}
bool KEYBOARD::IsReleased(UINT keyCode)
{
	return CSystem::g_keyboardInput.IsReleased(keyCode);
}
bool KEYBOARD::IsPressed(UINT keyCode)
{
	return CSystem::g_keyboardInput.IsPressed(keyCode);
}

const char * KEYBOARD::GetInBuffer()
{
	return CSystem::g_keyboardInput.InputBuffer();
}

CGameSystemControlScheme Control()
{
	return CSystem::g_controlScheme;
}

bool MOUSE::IsLeftMouseDown()
{
	return CSystem::g_mouseInput.IsLeftMouseDown();
}

bool MOUSE::IsRightMouseDown()
{
	return CSystem::g_mouseInput.IsRightMouseDown();
}

bool MOUSE::IsLeftMousePressed()
{
	return CSystem::g_mouseInput.IsLeftMousePressed();
}

bool MOUSE::IsRightMousePressed()
{
	return CSystem::g_mouseInput.IsRightMousePressed();
}

bool MOUSE::IsLeftMouseReleased()
{
	return CSystem::g_mouseInput.IsLeftMouseReleased();
}

bool MOUSE::IsRightMouseReleased()
{
	return CSystem::g_mouseInput.IsRightMouseReleased();
}

bool MOUSE::IsInRegion(RECT r)
{
	int mouse_x = CSystem::g_mouseInput.GetMouseX();
	int mouse_y = CSystem::g_mouseInput.GetMouseY();

	return (r.left < mouse_x && r.right > mouse_x &&
		r.top < mouse_y && r.bottom > mouse_y);
}

int MOUSE::GetMouseX()
{
	return CSystem::g_mouseInput.GetMouseX();
}

int MOUSE::GetMouseY()
{
	return CSystem::g_mouseInput.GetMouseY();
}

int MOUSE::GetMousePrevX()
{
	return CSystem::g_mouseInput.GetMousePrevX();
}

int MOUSE::GetMousePrevY()
{
	return CSystem::g_mouseInput.GetMousePrevY();
}

int MOUSE::GetChangeX()
{
	return GetMouseX() - GetMousePrevX();
}

int MOUSE::GetChangeY()
{
	return GetMouseY() - GetMousePrevY();
}

void MOUSE::SetPosition(int x, int y)
{
	int xOff = 0, yOff = 0;
	if (PUMP::g_system)
	{
		RECT wndRect;

		GetWindowRect(PUMP::g_system->GetWindow().GetHandle(), &wndRect);

		xOff = wndRect.left + 8;
		yOff = wndRect.top + 31;
	}

	CSystem::g_mouseInput.SetMouse(x, y);
	SetCursorPos(xOff + x, yOff + y);
}
