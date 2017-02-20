#include "CWindow.h"
#include "CSystem.h"

#include "Helper.h"

CSystem* PUMP::g_system = NULL;

LRESULT CALLBACK WndProcPump(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(PUMP::g_system)
		return PUMP::g_system->SystemWndProc(hWnd,msg,wParam,lParam);
	MessageBox(NULL,"g_system Not Initialized Correctly!","Error!",
		MB_OK | MB_ICONEXCLAMATION );
	return false;
}

CWindow::CWindow()
	:
	m_hWnd( 0 ),
	m_clientWidth(0),
	m_clientHeight(0),
	m_windowClassName(nullptr),
	m_windowName(nullptr)
{}

CWindow::~CWindow()
{
	MEMORY::SafeDeleteArr(m_windowClassName);
	MEMORY::SafeDeleteArr(m_windowName);
}

bool CWindow::Init(HINSTANCE hInstance, unsigned int clientWidth, unsigned int clientHeight )
{
	m_hInstance = hInstance;

	ZeroMemory(&wcex,sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wcex.hInstance = hInstance;


	wcex.lpfnWndProc = WndProcPump;
	wcex.lpszClassName = m_windowClassName;
	wcex.lpszMenuName = nullptr;
	wcex.style = 0;

	if(!RegisterClassEx(&wcex))
		return false;

	m_clientWidth = clientWidth;
	m_clientHeight = clientHeight;

	m_windowRect.bottom = clientHeight;
	m_windowRect.right = clientWidth;
	m_windowRect.left = 0;
	m_windowRect.top = 0;

	AdjustWindowRect(&m_windowRect, m_windowStyle, false);

	return true;
}

bool CWindow::CWndCreateWindow()
{
	int posX, posY, width, height;

	width = m_windowRect.right - m_windowRect.left;
	height = m_windowRect.bottom - m_windowRect.top;

	posX = (GetSystemMetrics(SM_CXSCREEN)/2) - width/2;
	posY = (GetSystemMetrics(SM_CYSCREEN)/2) - height/2;

	m_hWnd = CreateWindow(m_windowClassName,m_windowName,m_windowStyle, posX, posY,
		width, height, NULL, NULL, wcex.hInstance, NULL);

	if(!m_hWnd)
		return false;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

void CWindow::SetWindowClassName(char* name)
{
	STRING::Copy(m_windowClassName,name);
}

void CWindow::SetWindowName(char* name)
{
	STRING::Copy(m_windowName,name);
}

void CWindow::SetWindowStyle(DWORD style)
{
	m_windowStyle = style;
}

HWND CWindow::GetHandle()
{
	return m_hWnd;
}

UINT CWindow::GetHeight()
{
	return m_clientHeight;
}

UINT CWindow::GetWidth()
{
	return m_clientWidth;
}