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
#include <WinSock.h>
#include <queue>

#include "VitaInput.h"

class CServer
{
private:
	enum Server_State { SVR_Nothing, SVR_Recieve, SVR_Parse, SVR_Send};
	Server_State m_curState;

	// winsock variables
	WORD m_winsockVersion;
	WSADATA m_wsaData;

	SOCKET m_clientSocket;
	SOCKET m_listenSocket;
	SOCKADDR_IN m_serverInfo;

	int m_nRet;
	unsigned short m_Port;
	unsigned int m_connectionCount;
	///////////////////////////////////////////////////////////////////////

	// server related
	std::queue<char*> m_PacketQueue;
	char* m_pRecvPacket;
	volatile bool m_bClientConnected;
	volatile bool m_bClientReady;

	void ReportError(int errorCode, const char *whichFunc);
	void ConnectionLost();

	
	// thread safe
	CRITICAL_SECTION CS_PacketQueue;
	CRITICAL_SECTION CS_ClientSocket;
	void Lock(CRITICAL_SECTION& CS);
	void UnLock(CRITICAL_SECTION& CS);

	// recv from vita
	CVitaInput m_Input;

public:
	CServer(void);
	~CServer(void);

	bool Init(unsigned short port, unsigned int connectionCount);
	void Shutdown();
	
	// states
	bool WaitForClient();
	bool Recieve();
	void Parse();
	void Send();

	void QueuePacket(char* pPacket);
	inline bool ClientConnected() { return m_bClientConnected;}
	inline bool ClientReady() { return m_bClientReady;}
	inline void ClientNotReady() { m_bClientReady = false;}
};

