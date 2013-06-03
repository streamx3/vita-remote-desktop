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

