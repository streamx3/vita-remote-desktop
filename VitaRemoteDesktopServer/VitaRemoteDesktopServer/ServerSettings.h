#pragma once
#define LEAN_AND_MEAN
#include <Windows.h>

class CCaptureDX;

class CServerSettings
{
	friend class CCaptureDX;
private:
	unsigned int m_unScreenWidth;
	unsigned int m_unScreenHeight;
	unsigned int m_unScreenOffsetX;
	unsigned int m_unScreenOffsetY;
	unsigned int m_unCropWidth;
	unsigned int m_unCropHeight;
	unsigned int m_unDestinationWidth;
	unsigned int m_unDestinationHeight;

	
	RECT m_ScreenRect;
	RECT m_CaptureRect;
	RECT m_DestinationRect; // the vita screen size of 960x544
	
	// hide constructors to make singleton
	CServerSettings(void);
	CServerSettings(CServerSettings&);
	CServerSettings& operator=(CServerSettings&);
public:
	~CServerSettings(void);
	static CServerSettings* GetInstance();
};

