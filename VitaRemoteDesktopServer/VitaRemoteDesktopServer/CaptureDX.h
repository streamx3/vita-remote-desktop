#pragma once
// This class is used to Init DirextX to capture the screen
#define LEAN_AND_MEAN
#define SPLIT_IMAGE 0
#define AS_BITMAP 1

#include <Windows.h>

#include "ServerSettings.h"

struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DSurface9;
class CServer;


class CCaptureDX
{
private:
	// server settings
	CServerSettings m_pSettings;

	// to init DirectX
	IDirect3D9* m_pInterface;
	IDirect3DDevice9* m_pDevice;
	HWND m_pWindowHandle;

	// for capturing the screen;
	IDirect3DSurface9* m_pFinalSurface;
	IDirect3DSurface9* m_pBufferSurface;
	
	// to send
	char* m_Packet;


#if SPLIT_IMAGE
	void* m_pPixelDataLeft;
	void* m_pPixelDataRight;
	void* m_pScaledPixelDataLeft;
	void* m_pScaledPixelDataRight;
#else
#if !AS_BITMAP
	void* m_pPixelData;
	void* m_pScaledPixelData;
#endif
#endif

#if AS_BITMAP
	unsigned int m_FinalImageSize;
#else
	unsigned int m_unPixelDataBufferSize;
	unsigned int m_unScaledPixelDataBufferSize;
#endif
	
	// for resizing
	struct PixelColor
	{
		BYTE A, R, G, B;
	};

	// functions
	void CaptureScreen(bool bScreenShot = false);
	void ScaleImage();
public:
	CCaptureDX(void);
	~CCaptureDX(void);

	int Init(HWND windowHandle);
	void Shutdown();
	void Run(bool bScreenShot = false);

	void SendScreenPackets(CServer* server);
};

