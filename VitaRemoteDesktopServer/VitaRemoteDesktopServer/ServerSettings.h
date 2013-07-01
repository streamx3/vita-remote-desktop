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

#pragma once
#define LEAN_AND_MEAN
#include <Windows.h>

#include <map>
#include "VitaInput.h"

class CCaptureDX;

class CServerSettings
{
	friend class CCaptureDX;
	friend class CVitaInput;
private:
	unsigned int m_unScreenWidth;
	unsigned int m_unScreenHeight;
	unsigned int m_unScreenOffsetX;
	unsigned int m_unScreenOffsetY;
	unsigned int m_unCropWidth;
	unsigned int m_unCropHeight;
	unsigned int m_unDestinationWidth;
	unsigned int m_unDestinationHeight;

	std::map<CVitaInput::GamePadButtons, char> m_KeyBinding;

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

