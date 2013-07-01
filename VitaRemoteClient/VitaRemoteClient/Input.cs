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
using System.Runtime.InteropServices;
using Sce.PlayStation.Core.Input;
using Sce.PlayStation.Core;

namespace VitaRemoteClient
{
	//[StructLayout(LayoutKind.Explicit)]
	public class GamepadPacketData
	{
		private uint m_unButtonState;
		private float m_fLeftAnalogX;
		private float m_fLeftAnalogY;
		private float m_fRightAnalogX;
		private float m_fRightAnalogY;
		private byte[] m_Raw = new byte[20];
		
		public uint ButtonState
		{
			get { return m_unButtonState;}
			set 
			{
				m_unButtonState = value;
				Buffer.BlockCopy(BitConverter.GetBytes(m_unButtonState), 0, m_Raw, 0, 4);
			}
		}
		
		public float LeftAnalogX
		{
			get{ return m_fLeftAnalogX; }
			set
			{
				m_fLeftAnalogX = value;
				Buffer.BlockCopy(BitConverter.GetBytes(m_fLeftAnalogX), 0, m_Raw, 4, 4);
			}
		}
		public float LeftAnalogY
		{
			get{ return m_fLeftAnalogY; }
			set
			{
				m_fLeftAnalogY = value;
				Buffer.BlockCopy(BitConverter.GetBytes(m_fLeftAnalogY), 0, m_Raw, 8, 4);
			}
		}
		public float RightAnalogX
		{
			get{ return m_fRightAnalogX; }
			set
			{
				m_fRightAnalogX = value;
				Buffer.BlockCopy(BitConverter.GetBytes(m_fRightAnalogX), 0, m_Raw, 12, 4);
			}
		}
		public float RightAnalogY
		{
			get{ return m_fRightAnalogY; }
			set
			{
				m_fRightAnalogY = value;
				Buffer.BlockCopy(BitConverter.GetBytes(m_fRightAnalogY), 0, m_Raw, 16, 4);
			}
		}
		
		public byte[] Raw
		{
			get { return m_Raw;}
			set 
			{
				m_Raw = value;
				Buffer.BlockCopy(m_Raw, 0, BitConverter.GetBytes(m_unButtonState), 0, 4);
				Buffer.BlockCopy(m_Raw, 4, BitConverter.GetBytes(m_fLeftAnalogX), 0, 4);
				Buffer.BlockCopy(m_Raw, 8, BitConverter.GetBytes(m_fLeftAnalogY), 0, 4);
				Buffer.BlockCopy(m_Raw, 12, BitConverter.GetBytes(m_fRightAnalogX), 0, 4);
				Buffer.BlockCopy(m_Raw, 16, BitConverter.GetBytes(m_fRightAnalogY), 0, 4);
			}
		}		
	};
	
	public static class Input
	{
//		private static int PSV_BTN_UP;
//		private static int PSV_BTN_DOWN;
//		private static int PSV_BTN_LEFT;
//		private static int PSV_BTN_RIGHT;
//		private static int PSV_BTN_CROSS;
//		private static int PSV_BTN_SQUARE;
//		private static int PSV_BTN_CIRCLE;
//		private static int PSV_BTN_TRIANGLE;
//		private static int PSV_BTN_LTRIGGER;
//		private static int PSV_BTN_RTRIGGER;
//		private static int PSV_BTN_SELECT;
//		private static int PSV_BTN_START;
		
		private static byte[] _prevData;
		private static int inputType = 0;  // 0 = keyboardmouse 1 = gamepad;
		private static int motionSensitivity = 10;
		
		private static GamepadPacketData prevGamepadData = new GamepadPacketData();
		
		public static void UpdateGamepadState()
		{
			var gamepadData = GamePad.GetData(0);
			
			GamepadPacketData padData = new GamepadPacketData();
			
			// turn on the bool bits
			uint unButtonState = 0;
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Left);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Right);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Up);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Down);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Square);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Cross);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Circle);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Triangle);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.L);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.R);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Select);
			unButtonState |= (uint)(gamepadData.Buttons & GamePadButtons.Start);
			padData.ButtonState = unButtonState;
			
			// copy the analog values
			padData.LeftAnalogX = gamepadData.AnalogLeftX;
			padData.LeftAnalogY = gamepadData.AnalogLeftY;
			padData.RightAnalogX = gamepadData.AnalogRightX;
			padData.RightAnalogY = gamepadData.AnalogRightY;
			
			// send
			bool compResult = padData.Raw.CompareBytes(prevGamepadData.Raw);
			if(compResult == true)
			{
			   SendPacket.sendGamePadInput(padData.Raw);
			}
			prevGamepadData.Raw = padData.Raw;

			/*
			byte[] data = new byte[16];
			
			data[0] = (byte)(((gamepadData.Buttons & GamePadButtons.Left) != 0) 		? 1:0);
			data[1] = (byte)(((gamepadData.Buttons & GamePadButtons.Right) != 0) 		? 1:0);
			data[2] = (byte)(((gamepadData.Buttons & GamePadButtons.Up) != 0) 			? 1:0);
			data[3] = (byte)(((gamepadData.Buttons & GamePadButtons.Down) != 0) 		? 1:0);
			data[4] = (byte)(((gamepadData.Buttons & GamePadButtons.Square) != 0) 		? 1:0);
			data[5] = (byte)(((gamepadData.Buttons & GamePadButtons.Cross) != 0) 		? 1:0);
			data[6] = (byte)(((gamepadData.Buttons & GamePadButtons.Circle) != 0) 		? 1:0);
			data[7] = (byte)(((gamepadData.Buttons & GamePadButtons.Triangle) != 0) 	? 1:0);
			data[8] = (byte)(((gamepadData.Buttons & GamePadButtons.L) != 0) 			? 1:0);
			data[9] = (byte)(((gamepadData.Buttons & GamePadButtons.R) != 0) 			? 1:0);
			data[10] = (byte)(((gamepadData.Buttons & GamePadButtons.Select) != 0) 		? 1:0);
			data[11] = (byte)(((gamepadData.Buttons & GamePadButtons.Start) != 0)		? 1:0);
			
			data[12] = (byte)(gamepadData.AnalogLeftX);
			data[13] = (byte)(gamepadData.AnalogLeftY);
			data[14] = (byte)(gamepadData.AnalogRightX);
			data[15] = (byte)(gamepadData.AnalogRightY);
			
			if(inputType == 0)
			{
			//value 1 is positive value 2 is negative
				
				if(gamepadData.AnalogLeftX > 0.5f)
					data[12] = 1;
				
				if(gamepadData.AnalogLeftX < -0.5f)
					data[12] = 2; 
				
				if(gamepadData.AnalogLeftX < 0.5f && gamepadData.AnalogLeftX > -0.5f)
					data[12] = 0;
				
				if(gamepadData.AnalogLeftY > 0.5f)
					data[13] = 1;
				
				if(gamepadData.AnalogLeftY < -0.5f)
					data[13] = 2;
				
				if(gamepadData.AnalogLeftY < 0.5f && gamepadData.AnalogLeftY > -0.5f)
					data[13] = 0;
				
				if(gamepadData.AnalogRightX > 0.5f)
					data[14] = 1;
				
				if(gamepadData.AnalogRightX < -0.5f)
					data[14] = 2; 
				
				if(gamepadData.AnalogRightX < 0.5f && gamepadData.AnalogRightX > -0.5f)
					data[14] = 0;
				
				if(gamepadData.AnalogRightY > 0.5f)
					data[15] = 1;
				
				if(gamepadData.AnalogRightY < -0.5f)
					data[15] = 2;
				
				if(gamepadData.AnalogRightY < 0.5f && gamepadData.AnalogRightY > -0.5f)
					data[15] = 0;
			}
			else
			{
				//0 to 127 positive value  128 to 254 negative value
				if((127 * gamepadData.AnalogLeftX) >= 0)
					data[12] = (byte)(127 * gamepadData.AnalogLeftX);
				else
					data[12] = (byte)(((127 * gamepadData.AnalogLeftX) *-1f) + 127);
					
			}
			*/
			/*
			if((gamepadData.Buttons & GamePadButtons.L) != 0 && false)
			{
				var motionData = Motion.GetData(0);
				Vector3 vel = motionData.AngularVelocity;
				Vector3 acc = motionData.Acceleration;
				
				int avX,avY,avZ,aX,aY,aZ;
				
				avX = (int)(motionSensitivity * vel.X);
				avY = (int)(motionSensitivity * vel.Y);
				avZ = (int)(motionSensitivity * vel.Z);
				aX = (int)(motionSensitivity * acc.X);
				aY = (int)(motionSensitivity * acc.Y);
				aZ = (int)(motionSensitivity * acc.Z);
				
				SendPacket.sendSensorData(avX,avY,avZ,aX,aY,aZ);
			}
			 */
			//data[12] = (byte)(gamepadData.AnalogLeftX);
			//data[13] = (byte)(gamepadData.AnalogLeftY);
			//data[14] = (byte)(gamepadData.AnalogRightX);
			//data[15] = (byte)(gamepadData.AnalogRightY);
			
			/*
			if(_prevData != null)
			{
				if(data.CompareBytes(_prevData))
				   SendPacket.sendGamePadInput(data);
			}
			else
			{
				SendPacket.sendGamePadInput(data);
			}
			
			_prevData = data;
			*/
//			int left = ((gamepadData.Buttons & GamePadButtons.Left) != 0) 			? 1:0;
//			int right = ((gamepadData.Buttons & GamePadButtons.Right) != 0) 		? 1:0;
//			int up = ((gamepadData.Buttons & GamePadButtons.Up) != 0) 				? 1:0;
//			int down = ((gamepadData.Buttons & GamePadButtons.Down) != 0) 			? 1:0;
//			int square = ((gamepadData.Buttons & GamePadButtons.Square) != 0) 		? 1:0;
//			int cross = ((gamepadData.Buttons & GamePadButtons.Cross) != 0) 		? 1:0;
//			int circle = ((gamepadData.Buttons & GamePadButtons.Circle) != 0) 		? 1:0;
//			int triangle = ((gamepadData.Buttons & GamePadButtons.Triangle) != 0) 	? 1:0;
//			int ltrigger = ((gamepadData.Buttons & GamePadButtons.L) != 0) 			? 1:0;
//			int rtrigger = ((gamepadData.Buttons & GamePadButtons.R) != 0) 			? 1:0;
//			int iselect = ((gamepadData.Buttons & GamePadButtons.Select) != 0) 		? 1:0;
//			int start = ((gamepadData.Buttons & GamePadButtons.Start) != 0)			? 1:0;
//			
//			bool change = GamePadStateChange(left,
//			                                 right,
//			                                 up,
//			                                 down,
//			                                 square,
//			                                 cross,
//			                                 circle,
//			                                 triangle,
//			                                 ltrigger,
//			                                 rtrigger);
//			
//			if(change)
//			{
//				byte[] data = new byte[10];
//				data[0] = (byte)PSV_BTN_LEFT;
//				data[1] = (byte)PSV_BTN_RIGHT;
//				data[2] = (byte)PSV_BTN_UP;
//				data[3] = (byte)PSV_BTN_DOWN;
//				data[4] = (byte)PSV_BTN_SQUARE;
//				data[5] = (byte)PSV_BTN_CROSS;
//				data[6] = (byte)PSV_BTN_CIRCLE;
//				data[7] = (byte)PSV_BTN_TRIANGLE;
//				data[8] = (byte)PSV_BTN_LTRIGGER;
//				data[9] = (byte)PSV_BTN_RTRIGGER;
//				data[10] = (byte)PSV_BTN_RTRIGGER;
//				data[11] = (byte)PSV_BTN_RTRIGGER;
//				
//				
//				SendPacket.sendGamePadInput(data);
//			}
//				//send keystate data only if keystate change
//			Console.WriteLine(change);
//			//Console.WriteLine(gamepadData.Buttons);
//			//Console.WriteLine(PSV_BTN_LEFT);
//			//joyState.digital[0] = (ctrl.Buttons & PSP_CTRL_TRIANGLE) ? 1 : 0;
		}
		
		
		
//		private static bool GamePadStateChange(int left,
//		                                int right,
//		                                int up,
//		                                int down,
//		                                int square,
//		                                int cross,
//		                                int circle,
//		                                int triangle,
//		                                int ltrigger,
//			                            int rtrigger)
//		{
//			bool retval = false;
//			
//			if(PSV_BTN_LEFT != left) retval = true; PSV_BTN_LEFT = left;
//			if(PSV_BTN_RIGHT != right) retval = true; PSV_BTN_RIGHT = right;
//			if(PSV_BTN_UP != up) retval = true; PSV_BTN_UP = up;
//			if(PSV_BTN_DOWN != down) retval = true; PSV_BTN_DOWN = down;
//			if(PSV_BTN_SQUARE != square) retval = true; PSV_BTN_SQUARE = square;
//			if(PSV_BTN_CROSS != cross) retval = true; PSV_BTN_CROSS = cross;
//			if(PSV_BTN_CIRCLE != circle) retval = true; PSV_BTN_CIRCLE = circle;
//			if(PSV_BTN_TRIANGLE != triangle) retval = true; PSV_BTN_TRIANGLE = triangle;
//			if(PSV_BTN_LTRIGGER != ltrigger) retval = true; PSV_BTN_LTRIGGER = ltrigger;
//			if(PSV_BTN_RTRIGGER != rtrigger) retval = true; PSV_BTN_RTRIGGER = rtrigger;
//			   
//			return retval;
//		}
	}
	
}

