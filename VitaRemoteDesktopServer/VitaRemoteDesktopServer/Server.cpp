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

#include "Server.h"
#include "Packet.h"
#include <stdio.h>

#define SAFE_FREE(x) if(x != NULL){free(x); x = NULL;}

CServer::CServer(void)
{
	m_curState = SVR_Nothing;
	m_pRecvPacket = NULL;
	m_bClientConnected = false;
	m_bClientReady = false;
}


CServer::~CServer(void)
{
}


bool CServer::Init(unsigned short port, unsigned int connectionCount)
{
	// save variables
	m_Port = port;
	m_connectionCount = connectionCount;

	// winsock version 1.1
	m_winsockVersion = MAKEWORD(1, 1);

	// init
	WSAStartup(m_winsockVersion, &m_wsaData);

	// create the listening socket
	printf("Creating server socket...\n");
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_listenSocket == INVALID_SOCKET)
	{
		m_nRet = WSAGetLastError();
		ReportError(m_nRet, "socket()");
		WSACleanup();
		return false;
	}

	// non binding
	u_long iMode=1;
	ioctlsocket(m_listenSocket, FIONBIO, &iMode);
	
	// fill out hte server information
	m_serverInfo.sin_family = AF_INET;
	m_serverInfo.sin_addr.s_addr = INADDR_ANY; // we are listening for all addresses
	m_serverInfo.sin_port = htons(m_Port);

	// bind hte socket
	printf("Binding server socket...\n");
	m_nRet = bind(m_listenSocket, (LPSOCKADDR)&m_serverInfo, sizeof(SOCKADDR_IN));
	if(m_nRet == SOCKET_ERROR)
	{
		m_nRet = WSAGetLastError();
		ReportError(m_nRet, "bind()");
		WSACleanup();
		return false;
	}

	// start listening
	printf("Starting to listen...\n");
	m_nRet = listen(m_listenSocket, m_connectionCount);
	if(m_nRet == SOCKET_ERROR)
	{
		m_nRet = WSAGetLastError();
		ReportError(m_nRet, "listen()");
		WSACleanup();
		return false;
	}
	
	InitializeCriticalSection(&CS_PacketQueue);
	InitializeCriticalSection(&CS_ClientSocket);

	printf("Waiting for a client...\n");

	return true;
}
bool CServer::WaitForClient()
{
	// wait for a client to connect
	Lock(CS_ClientSocket);
		m_clientSocket = accept(m_listenSocket, NULL, NULL);
	
		if(m_clientSocket == SOCKET_ERROR)
		{
	UnLock(CS_ClientSocket);
			m_nRet = WSAGetLastError();
			if(m_nRet != WSAEWOULDBLOCK)
			{
				ReportError(m_nRet, "accept()");
				WSACleanup();
			}
			return false;
		}
	UnLock(CS_ClientSocket);

	m_bClientConnected = true;
	printf("Client Connected...\n");
	return true;
}

void CServer::Shutdown()
{
	Lock(CS_PacketQueue);
		while(m_PacketQueue.size() > 0)
		{
			free(m_PacketQueue.front());
			m_PacketQueue.pop();
		}
	UnLock(CS_PacketQueue);

	DeleteCriticalSection(&CS_PacketQueue);
	DeleteCriticalSection(&CS_ClientSocket);

	closesocket(m_clientSocket);
	closesocket(m_listenSocket);
	WSACleanup();
}

void CServer::QueuePacket(char* pPacket)
{
	Lock(CS_PacketQueue);
		m_PacketQueue.push(pPacket);
	UnLock(CS_PacketQueue);
}

bool CServer::Recieve()
{
	// dont bother if we dont have a client
	if(m_bClientConnected == false)
	{
		return false;
	}

	// only read the header
	PacketHeader tempHeader;
	Lock(CS_ClientSocket);
		m_nRet = recv(m_clientSocket, (char*)&tempHeader, (int)CPacket::GetHeaderSize(), 0);//MSG_PEEK);
	UnLock(CS_ClientSocket);

	if(m_nRet == SOCKET_ERROR)
	{
		m_nRet = WSAGetLastError();
		if(m_nRet == WSAEWOULDBLOCK)
		{
			return false;
		}
		if(m_nRet == WSAECONNRESET)
		{
			ConnectionLost();
			return false;
		}
		ReportError(m_nRet, "recv() - header");
	}
	else
	{
		// allocate a buffer for this packet
		int bytesRecv = m_nRet;
		m_pRecvPacket = (char*)malloc(tempHeader.size);
		char* pData = m_pRecvPacket + bytesRecv;

		// move the header over
		memcpy(m_pRecvPacket, (char*)&tempHeader, bytesRecv); // NOTE: wont need if MSG_PEEK is used

		// recv the rest
		do
		{
			Lock(CS_ClientSocket);
				//m_nRet = recv(m_clientSocket, (char*)&pData + bytesRecv, (int)tempHeader.size - CPacket::GetHeaderSize(), 0);
				m_nRet = recv(m_clientSocket, (char*)&m_pRecvPacket + bytesRecv, (int)tempHeader.size - CPacket::GetHeaderSize(), 0);
			UnLock(CS_ClientSocket);

			if(m_nRet == SOCKET_ERROR)
			{
				m_nRet = WSAGetLastError();
				ReportError(m_nRet, "recv() - data");
			}
			else
			{
				bytesRecv += m_nRet;
				pData += m_nRet;
				//printf("Recv: %d/%d\n", m_nRet, tempHeader.size);
			}
		} while(bytesRecv < tempHeader.size);
	}
	return true;
	
}
/*
 * Out packet IDs
 * NOTE: 501 -> 507 send intX and intY
 *     : 
 * 501 - // sendTap
 * 502 - // sendDoubleTap
 * 503 - // sendDrag1
 * 504 - // sendDrag2
 * 505 - // sendLongPress
 * 506 - // sendDragStart
 * 507 - // sendDragEnd
 * 401 - // sendReady
 * 301 - // sendGamePadInput
 * 302 - // sendSensorData
 * */
void CServer::Parse()
{
	if(m_pRecvPacket == NULL)
	{
		return;
	}
	PacketHeader& header = (PacketHeader&)*m_pRecvPacket;
	printf("Recv id #%d\n", header.id);

	switch(header.id)
	{
		case 501: // sendTap
			{
			} break;
		case 502: // sendDoubleTap
			{
			} break;
		case 503: // sendDrag1
			{
			} break;
		case 504: // sendDrag2
			{
			} break;
		case 505: // sendLongPress
			{
			} break;
		case 506: // sendDragStart
			{
			} break;
		case 507: // sendDragEnd
			{
			} break;
		case 401: // sendReady
			{
				//printf("Client is ready to start recieving data\n");
				m_bClientReady = true;
			} break;
		case 301: // sendGamePadInput
			{
			} break;
		case 302: // sendSensorData
			{
			} break;
	}
	SAFE_FREE(m_pRecvPacket);
	m_pRecvPacket = NULL;
}

void CServer::Send()
{
	Lock(CS_PacketQueue);
		int size = m_PacketQueue.size();
	UnLock(CS_PacketQueue);
	while(size > 0)
	{
		// get the next packet
		Lock(CS_PacketQueue);
			char* pPacket = m_PacketQueue.front();
			m_PacketQueue.pop();
			size = m_PacketQueue.size();
		UnLock(CS_PacketQueue);

		// send it
		unsigned int packetSize = ((PacketHeader*)pPacket)->size;
		Lock(CS_ClientSocket);
			m_nRet = send(m_clientSocket, pPacket, packetSize, 0);
		UnLock(CS_ClientSocket);

		// free the packet
		SAFE_FREE(pPacket);
		pPacket = NULL;

		// check for errors
		if(m_nRet == SOCKET_ERROR)
		{
			m_nRet = WSAGetLastError();
			if(m_nRet == WSAEWOULDBLOCK)
			{
				continue;
			}
			if(m_nRet == WSAECONNRESET)
			{
				ConnectionLost();
				continue;
			}
			ReportError(m_nRet, "send()");
		}
		else
		{
			printf("Sent: %d/%d\n", packetSize, m_nRet);
		}
	}
}

void CServer::ReportError(int errorCode, const char *whichFunc)
{
	printf("Call to %s returned error %d!\n", (char *)whichFunc, errorCode);
}
void CServer::ConnectionLost()
{
	if(m_bClientConnected == true)
	{
		m_bClientConnected = false;
		m_bClientReady = false;
		printf("Client Disconnected...\n");
	}
}

void CServer::Lock(CRITICAL_SECTION& CS)
{
	EnterCriticalSection(&CS);
}

void CServer::UnLock(CRITICAL_SECTION& CS)
{
	LeaveCriticalSection(&CS);
}