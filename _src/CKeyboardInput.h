#pragma once
#include <Windows.h>

struct KeyState
{
	bool isDown;
	bool isPressed;
	bool isReleased;

	int nFramesDown;
	int nFramesUp;
};

class CKeyboardInput;

class CKeyboardInputState
{
	friend CKeyboardInput;

public:
	CKeyboardInputState() 
	{
		ZeroMemory(keyState, sizeof(KeyState) * 256);
		in_caps = false;
		m_inputBuffer = 0;
		m_bufLen = 0;
	}

	void OnKeyPressed(UINT keyCode);
	void OnKeyReleased(UINT keyCode);
	void OnCharacterPressed(char c);
	void Update();

private:
	KeyState keyState[256];

	void AddToBuffer(char keyCode);
	bool in_caps;

	char* m_inputBuffer;
	int m_bufLen;
};

class CKeyboardInput
{
public:
	CKeyboardInput() {}
	~CKeyboardInput() {}

	void SetKeyboardState(CKeyboardInputState* state) { keyboard = state; }

	bool IsDown(UINT keyCode) { return keyboard->keyState[keyCode].isDown; }
	bool IsPressed(UINT keyCode) { return keyboard->keyState[keyCode].isPressed; }
	bool IsReleased(UINT keyCode) { return keyboard->keyState[keyCode].isReleased; }
	const char* InputBuffer() { return keyboard->m_inputBuffer; }

private:
	CKeyboardInputState* keyboard;
};