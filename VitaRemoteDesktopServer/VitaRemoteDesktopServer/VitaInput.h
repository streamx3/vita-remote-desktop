#pragma once

#include <Windows.h> // for INPUT

struct GamePadPacketData
{
	unsigned int unButtonState;
	float fLeftAnalogX;
	float fLeftAnalogY;
	float fRightAnalogX;
	float fRightAnalogY;
};


class CVitaInput
{
public:
	static enum TouchStatus { None, Down, Up, Move, Canceled };
	static enum GamePadButtons 
	{ 
		DPadLeft	= 1 << 0,  DPadUp	 = 1 << 1, DPadRight = 1 << 2,  DPadDown	= 1 << 3, 
		Square		= 1 << 4,  Triangle  = 1 << 5, Circle    = 1 << 6,  Cross		= 1 << 7, 
		Start		= 1 << 8,  Select	 = 1 << 9, L		 = 1 << 10, R			= 1 << 11, 
		//Enter		= 1 << 16, Back		 = 1 << 17,
		TotalButtons = 12 // 14 if uncommented
	};
private:

	GamePadPacketData m_CurState;
	GamePadPacketData m_PrevState;

	INPUT m_KeyEvent;

	void SendKeyPress(unsigned int vkKey);
	void SendKeyRelease(unsigned int vkKey);

public:

	CVitaInput(void);
	~CVitaInput(void);

	void UpdateButtonState(GamePadPacketData &data);
	bool ButtonDown(GamePadButtons btn);
	bool ButtonPressed(GamePadButtons btn);
	bool ButtonReleased(GamePadButtons btn);
};

