#include <Windows.h>
#include <stdio.h>
#include <vld.h>
#include "ServerSettings.h"
#include "CaptureDX.h"
#include "Server.h"

#ifdef _DEBUG
#define return system("pause"); return
#endif

const char* szWindowTitle = "Remote Desktop Server";

struct ThreadData
{
	CServer* server;
	bool *bRunning;
	int* netFPS;
};

DWORD WINAPI CaptureThread(LPVOID d)
{
	ThreadData &data = *(ThreadData*)d;
	
	printf("Initializing the Capture Device...\n");

	
	HWND consoleHandle;
	CCaptureDX captureObj;
	HRESULT error = 0;
	CServerSettings* settings = CServerSettings::GetInstance();

	//consoleHandle = FindWindow(0, "Minecraft");
	consoleHandle = GetDesktopWindow();
	error = captureObj.Init(consoleHandle);

	if(error != 0)
	{
		printf("Error #%d has occured.... Now exiting\n", error);
		return error;
	}
	printf("Initialization Complete!\n");

	bool bClientReady = false;
	while(*data.bRunning)
	{
		bClientReady = data.server->ClientConnected() && data.server->ClientReady();
		if(bClientReady)
		{
			(*data.netFPS)++;
			captureObj.Run();
			captureObj.SendScreenPackets(data.server);
			data.server->Send();
		}
	}
	
	printf("Shutting down the Capture Device...\n");
	captureObj.Shutdown();
}

int main(int argc, char* argv[])
{
	SetConsoleTitle(szWindowTitle);
	printf("Server Starting...\n");

	// variables...
	char titleBuffer[255];
	bool bRunning = true;
	int frameCount = 0;
	int netFPS = 0;
	DWORD curTime = 0;
	DWORD prevTime = 0;
	CServer server;
		
	printf("Starting socket initializations...\n");
	bRunning = server.Init(8080, 10);

	if(bRunning == false)
	{
		printf("Problem Initializing Server... Shutting down\n");
		return -1;
	}

	ThreadData data;
	data.server = &server;
	data.bRunning = &bRunning;
	data.netFPS = &netFPS;
	HANDLE threadHandle = CreateThread(NULL, 0, CaptureThread, (void*)&data, 0, NULL);


	bool recvData = false;
	bool bStart = true;
	bool bHaveClient = false;
	prevTime = GetTickCount();
	do
	{
		// keep track of the fps
		frameCount++;
		curTime = GetTickCount();
		if(curTime - prevTime > 1000)
		{
			memset(&titleBuffer, 0, 255);
			sprintf_s(titleBuffer, 255, "%s - fps: %d - Network %d", szWindowTitle, frameCount, netFPS);
			netFPS = 0;
			SetConsoleTitle(titleBuffer);
			prevTime = curTime;
			frameCount = 0;
		}
		///////////////////////////////////////////////////////////////////////
		
		if(bHaveClient == false)
		{
			bHaveClient = server.WaitForClient();
		}
		else
		{


			// recieve packets from vita
			recvData = server.Recieve();
			if(recvData == true)
			{
				// process packets
				server.Parse();
			}


			/*
			// ensure the client is ready to recieve
			if(server.ClientReady() == true)
			{
				// get the screenshots
				//captureObj.Run();

				// count the sent packets per sec
				//netCount++;

				// Send the Screenshots
				//captureObj.SendScreenPackets(&server);
				//server.Send();
			}
			else
			{
			}
			*/

		}
		// check for an exit
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			bRunning = false;
		}

	} while(bRunning == true);

	// closing the thread
	WaitForSingleObject(threadHandle, INFINITE);

	printf("Shutting down the server...\n");
	server.Shutdown();
	
	return 0;
}