#include "Packet.h"
#include <assert.h>

// init static variables
unsigned int CPacket::m_unHeaderSize = sizeof(PacketHeader);

CPacket::CPacket(void)
{
}


CPacket::~CPacket(void)
{
}


char* CPacket::RequestPacket(unsigned int sizeRequested, unsigned short packetID)
{
	// request the packet
	char* pPacket = (char*)malloc(sizeRequested + m_unHeaderSize);
	assert(pPacket != NULL && "Out of memory - Malloc Failed!");

	// set the header data
	PacketHeader& header = (PacketHeader&)pPacket[0];
	header.id = packetID;
	header.label[0] = 'R'; header.label[1] = 'Z';
	header.size = sizeRequested + m_unHeaderSize;

	// return the packet address
	return pPacket;
}

