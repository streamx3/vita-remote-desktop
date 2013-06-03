#pragma once
#include <WinSock.h>
#include <queue>

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

