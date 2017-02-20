#include "CMouseInput.h"

void CMouseInputState::OnLeftMousePressed()
{
	m_leftMouseDown = true;
	if (m_nLeftFramesDown == 0)
	{
		m_leftMousePressed = true;
	}

	m_nLeftFramesUp = 0;
}

void CMouseInputState::OnLeftMouseRelesed()
{
	m_leftMouseDown = false;
	if (m_nLeftFramesUp == 0)
	{
		m_leftMouseReleased = true;
	}

	m_nLeftFramesDown = 0;
}

void CMouseInputState::OnRightMousePressed()
{
	m_rightMouseDown = true;
	if (m_nRightFramesDown == 0)
	{
		m_rightMousePressed = true;
	}

	m_nRightFramesUp = 0;
}

void CMouseInputState::OnRightMouseReleased()
{
	m_rightMouseDown = false;
	if (m_nRightFramesUp == 0)
	{
		m_rightMouseReleased = true;
	}

	m_nRightFramesDown = 0;
}

void CMouseInputState::SetMouse(int x, int y)
{
	m_mouseX = x;
	m_mouseY = y;
}

void CMouseInputState::SetMousePrev(int x, int y)
{
	m_mouseXPrev = x;
	m_mouseYPrev = y;
}

void CMouseInputState::Update()
{
	m_leftMousePressed = false;
	m_leftMouseReleased = false;
	m_rightMousePressed = false;
	m_rightMouseReleased = false;

	if (m_leftMouseDown)
		m_nLeftFramesDown++;
	else
		m_nLeftFramesUp++;

	if (m_rightMouseDown)
		m_nRightFramesDown++;
	else
		m_nRightFramesUp++;

	SetMousePrev(m_mouseX, m_mouseY);
}

void CMouseInput::SetMouseState(CMouseInputState * state)
{
	m_mServ = state;
}
