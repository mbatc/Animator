#include "CKeyboardInput.h"
#include "MemoryHelper.h"
#include "StringHelper.h"

void CKeyboardInputState::OnKeyPressed(UINT keyCode)
{
	if (keyCode < 0)
		return;
	if (keyCode >= 256)
		return;

	if (keyState[keyCode].nFramesDown == 0)
	{
		keyState[keyCode].isPressed = true;
	}
	keyState[keyCode].isDown = true;
	keyState[keyCode].nFramesUp = 0;

}

void CKeyboardInputState::OnKeyReleased(UINT keyCode)
{
	if (keyCode < 0)
		return;
	if (keyCode >= 256)
		return;

	if (keyState[keyCode].nFramesUp == 0)
	{
		keyState[keyCode].isReleased = true;
	}
	keyState[keyCode].isDown = false;
	keyState[keyCode].nFramesDown = 0;

}

void CKeyboardInputState::OnCharacterPressed(char c)
{
	AddToBuffer(c);
}

void CKeyboardInputState::Update()
{
	for (int i = 0; i < 256; i++)
	{
		keyState[i].isPressed = false;
		keyState[i].isReleased = false;
		if (keyState[i].isDown)
			keyState[i].nFramesDown++;
		else
			keyState[i].nFramesUp++;
	}

	if (m_inputBuffer)
		delete[] m_inputBuffer;
	m_inputBuffer = 0;
	m_bufLen = 0;
}

void CKeyboardInputState::AddToBuffer(char keyCode)
{
	char c[2] = { keyCode,0 };

	if (m_inputBuffer)
		STRING::Append(m_inputBuffer, m_inputBuffer, c);
	else
		STRING::Copy(m_inputBuffer, c);
}
