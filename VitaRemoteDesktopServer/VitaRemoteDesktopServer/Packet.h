#pragma once

//server packet header 8 bytes
//        RZ                  0000                      00                  000000000000000000000000000
//head start 2bytes    packet size 4 bytes      packet id 2 bytes           packet data

//client packet header 6 bytes
//        RZ                  00                      00                  000000000000000000000000000
//head start 2bytes    packet size 2 bytes      packet id 2 bytes           packet data

#include <stdlib.h>

struct ImageInfo
{
	int imageWidth;
	int imageHeight;
};

struct PacketHeader
{
	char label[2]; // "RZ"
	short id;
	int size; // including this header
};

class CPacket
{
private:
	static unsigned int m_unHeaderSize;

	// no instances
	CPacket(void);
	~CPacket(void);
	CPacket(CPacket&) {}
	void operator=(CPacket&){}

public:

	// NOTE: automatically adds sizeof(PacketHeader)
	// in: number of bytes 
	// ret: pointer to data
	static char* RequestPacket(unsigned int sizeRequested, unsigned short packetID);

	static inline unsigned int GetHeaderSize() { return m_unHeaderSize;}
};

