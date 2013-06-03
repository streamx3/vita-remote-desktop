#include "ServerSettings.h"


CServerSettings::CServerSettings(void)
{
	// the screen size
	m_unScreenWidth = 1680;
	m_unScreenHeight = 1050;

	// the RECT that represents the screen
	m_ScreenRect.left = 0;
	m_ScreenRect.right = m_unCropWidth;
	m_ScreenRect.top = 0;
	m_ScreenRect.bottom = m_unCropHeight;

	// the "crop" dimentions
	m_unScreenOffsetX = 0;
	m_unScreenOffsetY = 0;
	m_unCropWidth = 1024;
	m_unCropHeight = 768;

	// the RECT that represents the "crop" location
	m_CaptureRect.left = m_unScreenOffsetX;
	m_CaptureRect.right = m_unScreenOffsetX + m_unCropWidth;
	m_CaptureRect.top = m_unScreenOffsetY;
	m_CaptureRect.bottom = m_unScreenOffsetY + m_unCropHeight;

	// the vita screen size
	m_unDestinationWidth = 960;
	m_unDestinationHeight = 544;

	// the RECT that represents the vita's screen
	m_DestinationRect.left = 0;
	m_DestinationRect.right = m_unDestinationWidth;
	m_DestinationRect.top = 0;
	m_DestinationRect.bottom = m_unDestinationHeight;

}

CServerSettings::~CServerSettings(void)
{
}

CServerSettings::CServerSettings(CServerSettings&)
{
}

CServerSettings& CServerSettings::operator=(CServerSettings& other)
{
	return other;
}

CServerSettings* CServerSettings::GetInstance()
{
	static CServerSettings pInstance;
	return &pInstance;
}