#include "VitaInput.h"
#include <string.h> // for memset
#include "ServerSettings.h"

#define HANDLEBUTTON(btn, settings)\
	if(ButtonPressed((btn)))\
		SendKeyPress((unsigned int)settings->m_KeyBinding[(btn)]);\
	else if(ButtonReleased((btn)))\
		SendKeyRelease((unsigned int)settings->m_KeyBinding[(btn)]);

CVitaInput::CVitaInput(void)
{
	memset(&m_CurState, 0, sizeof(GamePadPacketData));
	memset(&m_PrevState, 0, sizeof(GamePadPacketData));

	
    // Set up a generic keyboard event.
    m_KeyEvent.type = INPUT_KEYBOARD;
    m_KeyEvent.ki.wScan = 0;
    m_KeyEvent.ki.time = 0;
    m_KeyEvent.ki.dwExtraInfo = 0;
}


CVitaInput::~CVitaInput(void)
{
}

void CVitaInput::UpdateButtonState(GamePadPacketData &data)
{
	unsigned int GPSize = sizeof(GamePadPacketData);
	memcpy_s(&m_PrevState, GPSize, &m_CurState, GPSize);
	memcpy_s(&m_CurState, GPSize, &data, GPSize);

	// handle each button event
	CServerSettings* settings = CServerSettings::GetInstance();
	HANDLEBUTTON(DPadLeft, settings);
	HANDLEBUTTON(DPadUp, settings);
	HANDLEBUTTON(DPadRight, settings);
	HANDLEBUTTON(DPadDown, settings);
	HANDLEBUTTON(Square, settings);
	HANDLEBUTTON(Triangle, settings);
	HANDLEBUTTON(Circle, settings);
	HANDLEBUTTON(Cross, settings);
	HANDLEBUTTON(Start, settings);
	HANDLEBUTTON(Select, settings);
	HANDLEBUTTON(L, settings);
	HANDLEBUTTON(R, settings);
}


bool CVitaInput::ButtonDown(GamePadButtons btn)
{
	return (m_CurState.unButtonState & btn) != 0 ? true : false;
}

bool CVitaInput::ButtonPressed(GamePadButtons btn)
{
	bool prev = (m_PrevState.unButtonState & btn) != 0 ? true : false;
	bool cur = (m_CurState.unButtonState & btn) != 0 ? true : false;

	return (prev == false) && (cur == true);
}

bool CVitaInput::ButtonReleased(GamePadButtons btn)
{
	bool prev = (m_PrevState.unButtonState & btn) != 0 ? true : false;
	bool cur = (m_CurState.unButtonState & btn) != 0 ? true : false;

	return (prev == true) && (cur == false);
}

void CVitaInput::SendKeyPress(unsigned int vkKey)
{
    m_KeyEvent.ki.wVk = vkKey;
    m_KeyEvent.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &m_KeyEvent, sizeof(INPUT));
	printf("Key Pressed: %d - %c\n", vkKey, (char)vkKey);
}

void CVitaInput::SendKeyRelease(unsigned int vkKey)
{
    m_KeyEvent.ki.wVk = vkKey;
    m_KeyEvent.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &m_KeyEvent, sizeof(INPUT));
	printf("Key Released: %d - %c\n", vkKey, (char)vkKey);
}
