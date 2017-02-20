#pragma once

#include <Windows.h>

class CSystem;

namespace PUMP
{
	extern CSystem* g_system;
}

class CWindow
{
public:
	CWindow();
	~CWindow();

	void SetWindowName(char* name);
	void SetWindowClassName(char* name);
	void SetWindowStyle(DWORD style);
	bool Init(HINSTANCE hInstance, unsigned int clientWidth, unsigned int clientHeight);
	bool CWndCreateWindow();

	HWND GetHandle();
	UINT GetWidth();
	UINT GetHeight();
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	WNDCLASSEX wcex;

	char* m_windowName;
	char* m_windowClassName;

	unsigned int m_clientWidth;
	unsigned int m_clientHeight;
	RECT m_windowRect;

	DWORD m_windowStyle;
};