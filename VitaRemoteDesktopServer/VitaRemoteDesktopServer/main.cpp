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
	return 0;
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
	bRunning = server.Init(8081, 10);

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