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

using System;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace VitaRemoteClient
{
	struct ImageInfo
	{
		public int imageWidth;
		public int imageHeight;
	};
	
	struct PacketHeader
	{
		public char[] label;// = new char[] {'R', 'Z'};// "RZ"
		public short id;
		public int size; // including this header
	};
	
	public class Packet
	{
        private static int headerSize = 8;
		private PacketHeader header;
        //private byte[] headerStart = new byte[] {82, 68};
		//private int _ID;
		//private int _Len;
		private byte[] _Data;
		
		public Packet ()
		{
			header = new PacketHeader();
			header.label = new char[] {'R', 'Z'};
		}
		
		public byte[] Data 
		{
			get{return _Data;}
			set
			{
				this._Data = value;
				header.size = Data.Length + headerSize;
			}
		}
		
		public short ID 
		{
			get{ return header.id;}
			set{ header.id = value;}
		}
		
		public int Size 
		{
			get{return header.size;}
		}
		
		public static int HeaderSize
		{
			get { return headerSize;}
		}
		
		public byte[] toArray()
        {
            byte[] retVal = new byte[Size];			
            if (_Data != null)
            {
				// copy the header information over
				Buffer.BlockCopy(BitConverter.GetBytes(header.label[0]), 0, retVal, 0, 1);
				Buffer.BlockCopy(BitConverter.GetBytes(header.label[1]), 0, retVal, 1, 1);
				Buffer.BlockCopy(BitConverter.GetBytes(header.id), 0, retVal, 2, 2);
				Buffer.BlockCopy(BitConverter.GetBytes(header.size), 0, retVal, 4, 2);
				
				// copy the data
				Buffer.BlockCopy(_Data, 0, retVal, headerSize, header.size - headerSize);
			}
            return retVal;
        }
		
		public void FromArray(byte[] data)
		{
			MemoryStream stream = new MemoryStream(data);
			BinaryReader read = new BinaryReader(stream);
			
			// get the header filled out
			header.label = read.ReadChars(2);
			header.id = read.ReadInt16();
			header.size = read.ReadInt32();			
			
			// get the packet data
			_Data = new byte[header.size - headerSize];
			read.Read(_Data, 0, header.size - headerSize);
		}
		
	}
	
}

