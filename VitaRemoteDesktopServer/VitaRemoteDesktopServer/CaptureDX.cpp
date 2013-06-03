#include "CaptureDX.h"
#include <d3dx9.h>
#include <d3dx9tex.h>

#include "Packet.h"
#include "Server.h"

#define BITSPERPIXEL 32
#define SAFE_FREE(x) if(x != NULL){free(x); x = NULL;}

#include <fstream>

CCaptureDX::CCaptureDX(void)
{
}


CCaptureDX::~CCaptureDX(void)
{
}

// TODO: Make this run on its own thread
void CCaptureDX::CaptureScreen(bool bScreenShot)
{

    HRESULT err = m_pDevice->GetFrontBufferData(0, m_pBufferSurface);
	//HRESULT err = m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBufferSurface);
	//D3DXSaveSurfaceToFile("C:/remoteserver/Desktop_BB.bmp", D3DXIFF_BMP, m_pFinalSurface, NULL, NULL); 
	if(err != D3D_OK)
	{
		printf("(%d) - Error on GetBackBuffer()\n", err);
	}
	
#if AS_BITMAP
	
	err = D3DXLoadSurfaceFromSurface(m_pFinalSurface, NULL, NULL, 
		m_pBufferSurface, NULL, &m_pSettings.m_CaptureRect, D3DX_FILTER_LINEAR, 0);
	if(err != D3D_OK)
	{
		int invc = D3DERR_INVALIDCALL;
		int invd = D3DXERR_INVALIDDATA;
		printf("(%d) - Error on D3DXLoadSurfaceFromSurface()\n", err);
	}

	// create an image from the surface
	// NOTE: PSM only accepts BMP, JPG, and PNG.
	// BMP makes a large packet, and PNG takes too long to create 
	LPD3DXBUFFER DXBuffer = NULL;
	err = D3DXSaveSurfaceToFileInMemory(&DXBuffer, D3DXIFF_JPG, m_pFinalSurface, NULL, &m_pSettings.m_CaptureRect);
	if(err != MB_OK)
	{
		printf("(%d) - Error on D3DXSaveSurfaceToFileInMemory()\n", err);
	}

	// the image information
	int infoSize = sizeof(ImageInfo);
	ImageInfo info;
	info.imageHeight = m_pSettings.m_unDestinationWidth;
	info.imageWidth = m_pSettings.m_unDestinationHeight;

	// copy the packet over
	DWORD DXImgSize = DXBuffer->GetBufferSize();
	m_FinalImageSize = DXImgSize;
	void* imgBuffer = DXBuffer->GetBufferPointer();

	// send normally or save as a screenshot for testing
	if(bScreenShot == false)
	{
		if(m_Packet = NULL)
		{
			char* packet = CPacket::RequestPacket(DXImgSize + infoSize, 103);
			memcpy((char*)packet + CPacket::GetHeaderSize(), &info, infoSize);
			memcpy((char*)packet + CPacket::GetHeaderSize() + infoSize, imgBuffer, m_FinalImageSize);
			m_Packet = packet;
		}
	}
	else
	{
		// build the file naem
		char buffer[128] = {0};
		static int frameNumber = 0;
		sprintf_s(buffer, 128, "C:/remoteserver/clean/Desktop_RAW_%d.jpg", frameNumber++);

		// write it out
		std::fstream out;
		out.open(buffer, std::ios_base::binary | std::ios_base::out);
		out.write((char*)imgBuffer, m_FinalImageSize);
		out.clear();
		out.close();
	}
	
#else
	// NOTE: This area would need updated if decided to use
	RECT tmpRect;
	tmpRect.left = m_pSettings.m_unScreenOffsetX;
	tmpRect.right = m_pSettings.m_unScreenOffsetX + m_pSettings.m_unCropWidth;
	tmpRect.top = m_pSettings.m_unScreenOffsetY;
	tmpRect.bottom = m_pSettings.m_unScreenOffsetY + m_pSettings.m_unCropHeight;

	// lock the surface
	D3DLOCKED_RECT lockedRect;
	m_pFinalSurface->LockRect(&lockedRect, &tmpRect, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);

	// copy the pixel information over
	UINT rowSize = m_pSettings.m_unCropWidth * BITSPERPIXEL / 8;
	UINT pitchSize = lockedRect.Pitch;
	UINT halfRow = rowSize / 2;
	UINT halfPitch = lockedRect.Pitch / 2;
	for( unsigned int byteIndex = 0; byteIndex < m_pSettings.m_unCropHeight; ++byteIndex)
	{
#if SPLIT_IMAGE
		// copy the left image
		BYTE* dstAddr = (BYTE*)m_pPixelDataLeft + (byteIndex * halfRow);
		BYTE* srcAddr = (BYTE*)lockedRect.pBits + (byteIndex * halfPitch);
		memcpy(dstAddr, srcAddr, halfRow);

		// copy the right image
		dstAddr = (BYTE*)m_pPixelDataRight + (byteIndex * halfRow);
		srcAddr += halfPitch;
		memcpy(dstAddr, srcAddr, halfRow);
#else
		// copy the full image
		BYTE* dstAddr = (BYTE*)m_pPixelData + (byteIndex * rowSize);
		BYTE* srcAddr = (BYTE*)lockedRect.pBits + (byteIndex * pitchSize);
		memcpy(dstAddr, srcAddr, rowSize);

#endif
	}

	// unlock the surface
	m_pFinalSurface->UnlockRect();
	//m_pFinalSurface->Release();
#endif
}

int CCaptureDX::Init(HWND windowHandle)
{
	// create the interface
	m_pInterface = Direct3DCreate9(D3D_SDK_VERSION);

	if(m_pInterface == NULL)
	{
		// couldnt create interface
		return 1;
	}

	// create the present params
	D3DPRESENT_PARAMETERS presentParams;
	memset(&presentParams, 0, sizeof(D3DPRESENT_PARAMETERS));
	presentParams.Windowed = true;
	presentParams.SwapEffect = D3DSWAPEFFECT_COPY;

	// create the device
	UINT adapter = D3DADAPTER_DEFAULT;
	D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
	DWORD behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	m_pWindowHandle = windowHandle;

	HRESULT error = m_pInterface->CreateDevice(adapter, deviceType, m_pWindowHandle, behaviorFlags, &presentParams, &m_pDevice);
	if(FAILED(error))
	{
		/*
		UINT a = D3DERR_DEVICELOST;
		UINT b = D3DERR_INVALIDCALL;
		UINT c = D3DERR_NOTAVAILABLE;
		UINT d = D3DERR_OUTOFVIDEOMEMORY;
		*/
		// error creating the device
		return error;
	}

	// create the surface to draw into
	m_pDevice->CreateOffscreenPlainSurface(m_pSettings.m_unScreenWidth, m_pSettings.m_unScreenHeight, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &m_pBufferSurface, NULL);
	m_pDevice->CreateOffscreenPlainSurface(m_pSettings.m_unCropWidth, m_pSettings.m_unCropHeight, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &m_pFinalSurface, NULL);

#if SPLIT_IMAGE
	// allocate the buffer for the cropped image(source size and position)
	m_unPixelDataBufferSize = (m_pSettings.m_unScreenWidth / 2) * m_pSettings.m_unScreenHeight * (BITSPERPIXEL / 8);
	m_pPixelDataLeft = malloc(m_unPixelDataBufferSize);
	m_pPixelDataRight = malloc(m_unPixelDataBufferSize);

	// allocate the area for the scaled image(destination size)
	m_unScaledPixelDataBufferSize = (m_pSettings.m_unCropWidth / 2) * m_pSettings.m_unCropHeight * (BITSPERPIXEL / 8);
	m_pScaledPixelDataLeft = malloc(m_unScaledPixelDataBufferSize);
	m_pScaledPixelDataRight = malloc(m_unScaledPixelDataBufferSize);
#else
	
#if !AS_BITMAP
	// allocate the buffer for the cropped image(source size and position)
	m_unPixelDataBufferSize = (m_pSettings.m_unScreenWidth) * m_pSettings.m_unScreenHeight * (BITSPERPIXEL / 8);
	m_pPixelData = malloc(m_unPixelDataBufferSize);

	// allocate the area for the scaled image(destination size)
	m_unScaledPixelDataBufferSize = (m_pSettings.m_unCropWidth) * m_pSettings.m_unCropHeight * (BITSPERPIXEL / 8);
	m_pScaledPixelData = malloc(m_unScaledPixelDataBufferSize);

#endif

#endif


	return 0;
}

void CCaptureDX::Shutdown()
{
#if SPLIT_IMAGE
	// free our buffers
	SAFE_FREE(m_pPixelDataLeft);
	SAFE_FREE(m_pPixelDataRight);
	SAFE_FREE(m_pScaledPixelDataLeft);
	SAFE_FREE(m_pScaledPixelDataRight);
#else
#if !AS_BITMAP
	// free our buffers
	//SAFE_FREE(m_Packet);
	SAFE_FREE(m_pPixelData);
	SAFE_FREE(m_pScaledPixelData);
#endif

#endif
	// shutdown DirectX stuff
	if(m_pBufferSurface != NULL)
	{
		m_pBufferSurface->Release();
		m_pBufferSurface = NULL;
	}
	if(m_pFinalSurface != NULL)
	{
		m_pFinalSurface->Release();
		m_pFinalSurface = NULL;
	}
	if(m_pDevice != NULL)
	{
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	if(m_pInterface != NULL)
	{
		m_pInterface->Release();
		m_pInterface = NULL;
	}
}

void CCaptureDX::Run(bool bScreenShot)
{
	CaptureScreen(bScreenShot);
	//ScaleImage();
}

#if SPLIT_IMAGE
void CCaptureDX::SendScreenPackets(CServer* server)
{
	char* leftPacket = CPacket::RequestPacket(m_unPixelDataBufferSize, 101);
	char* RightPacket = CPacket::RequestPacket(m_unPixelDataBufferSize, 102);

	memcpy((char*)leftPacket + CPacket::GetHeaderSize(), m_pPixelDataLeft, m_unPixelDataBufferSize);
	memcpy((char*)RightPacket + CPacket::GetHeaderSize(), m_pPixelDataRight, m_unPixelDataBufferSize);

	server->QueuePacket(leftPacket);
	server->QueuePacket(RightPacket);
}
#else

void CCaptureDX::SendScreenPackets(CServer* server)
{
	/*
	// the image information
	int infoSize = sizeof(ImageInfo);
	ImageInfo info;
	info.imageHeight = m_pSettings.m_unCropHeight;
	info.imageWidth = m_pSettings.m_unCropWidth;

#if AS_BITMAP
	// copy the packet over
	char* packet = CPacket::RequestPacket(m_FinalImageSize + infoSize, 103);
	memcpy((char*)packet + CPacket::GetHeaderSize(), &info, infoSize);
	memcpy((char*)packet + CPacket::GetHeaderSize() + infoSize, m_pPixelData, m_FinalImageSize);
#else
	// copy the packet over
	char* packet = CPacket::RequestPacket(m_unPixelDataBufferSize + infoSize, 103);
	memcpy((char*)packet + CPacket::GetHeaderSize(), &info, infoSize);
	memcpy((char*)packet + CPacket::GetHeaderSize() + infoSize, m_pPixelData, m_unPixelDataBufferSize);
#endif
	*/
	// Queue it
	//server->QueuePacket(packet);
	if(m_Packet != NULL)
	{
		server->QueuePacket(m_Packet);
	}

	// client not ready
	server->ClientNotReady();
}

#endif
