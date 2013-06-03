/*
Copyright (c) 2013, Erick<RetroZelda>Folckemer
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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