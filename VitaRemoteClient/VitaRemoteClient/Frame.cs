using System;
using System.Collections.Generic;
using System.Threading;
using Sce.PlayStation.Core.Graphics;

namespace VitaRemoteClient
{
	public static class Frame
	{
		private static Texture2D screenTexture;
		
		private static bool _frameReady = false;
		private static int _width = 0;
		private static int _height = 0;
		private static int tc = 0;
		
		private static bool bRelativeDragOne = false;
		public static bool RelativeDragOne
		{
			get { return bRelativeDragOne;}
			set { bRelativeDragOne = value;}
		}
		
		private static bool bRelativeDragTwo = false;
		public static bool RelativeDragTwo
		{
			get { return bRelativeDragTwo;}
			set { bRelativeDragTwo = value;}
		}
		
		
		public static int TC
		{
			get{ return tc;}
			set{ tc = value;}
		}
		
		public static int Width
		{
			get{return _width;}
			set{_width = value;
			}
		}
		
		public static int Height
		{
			get{return _height;}
			set{_height = value;
			}
		}
		
		
		public static bool FrameReady
		{
			get{return _frameReady;}
			set { _frameReady = value;}
		}
		
		public static Texture2D ScreenTexture
		{
			get { return screenTexture;}
			set { screenTexture = value;}
		}
		
		/*
		public static byte[] FrameBuffer1
		{
			get{
				return _FrameBuffer1;
			}
			set{
				_FrameBuffer1 = value;
			}
		}
		
		
		public static byte[] FrameBuffer2
		{
			get{
				_frameReady = false;
				return _FrameBuffer2;
			}
			set{
				_frameReady = true;
				_FrameBuffer2 = value;
			}
		}	
		*/
	}
}

