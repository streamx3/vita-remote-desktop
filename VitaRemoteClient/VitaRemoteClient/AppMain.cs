using System;
using System.Collections.Generic;
using System.Threading;

using Sce.PlayStation.Core;
using Sce.PlayStation.Core.Environment;
using Sce.PlayStation.Core.Graphics;
using Sce.PlayStation.Core.Imaging;
using Sce.PlayStation.Core.Input;
using System.IO;
using System.Reflection;
using Sce.PlayStation.HighLevel.UI;
using System.Diagnostics;
//using Sample

namespace VitaRemoteClient
{
	public class AppMain
	{
		private static GraphicsContext graphics;
		public static MainUI sceneMain;
		public static TCPConnection client;
		
		private static dialog.DLG_CONNECTION dgl_connection;
		
		private static Stopwatch stopWatch;
		
		private static Texture2D texture1;
		private static Texture2D texture2;
		
		private static double       mFps;
		private static long        mFrame = 0;
		private static long        mT = 0;
		private static long        mT0 = 0;
		
		private static bool loop = true;
		
		public static bool Loop
		{
			get { return loop;}
			set { loop = value;}
		}
		
		public static void Main (string[] args)
		{
			Initialize ();
			stopWatch.Start();
			while (loop) {
				//Thread.Sleep(10);
				
				SystemEvents.CheckEvents ();
				Update ();
				Render ();
			}
			
			term();
		}
		
		public static void Initialize ()
		{
			// Set up the graphics system
			graphics = new GraphicsContext();
			sceneMain = new MainUI();
			
			Draw.init(graphics);
			dgl_connection = new dialog.DLG_CONNECTION();
		
			UISystem.Initialize(graphics);
			UISystem.SetScene(sceneMain);
			
			stopWatch = new Stopwatch();
			texture1 = new Texture2D(940, 544, false,PixelFormat.Rgba);
			texture2 = new Texture2D(940, 544, false,PixelFormat.Rgba);
			
			// Set up Client
			client = new TCPConnection("127.0.0.1",8080);
			SendPacket.Init(client);
			
			// set settings
			graphics.SetClearColor (0.0f, 0.0f, 0.0f, 0.0f);
			
			
		}
		
		public static void Update ()
		{
			// Query gamepad for current state
			//var gamePadData = GamePad.GetData (0);
			
			
			switch(client.StatusType)
			{
			case Status.kNone:
					if(dgl_connection.Visible != true)
						dgl_connection.Show();
				break;
			case Status.kDisconnected:
				if(dgl_connection.Visible != true)
						dgl_connection.Show();
				break;
			default:
				break;
			}
			
			List<TouchData> touchDataList = Touch.GetData (0);
			Frame.TC = touchDataList.Count;
				
			UISystem.Update(touchDataList);

		}
		
		public static void Render ()
		{			
			// Clear the screen
			graphics.Clear ();
			
			if(client.isConnected == true)
			{
				Input.UpdateGamepadState();
				client.updateImage();
				
				drawDesktop();
			}
			else
			{
				Draw.renderTest();
			}
			sceneMain.UpdateFPSLabel("FPS: " + mFps.ToString());
			
			UISystem.Render();

			// Present the screen
			graphics.SwapBuffers ();
			//SampleDraw.ClearSprite();
		}
		
		private static void term()
		{
			graphics.Dispose();
			UISystem.Terminate();
			client.Disconnect();
		}
		
		private static void drawDesktop()
		{
			
			try
			{
				/*
				if(Frame.FrameReady)
				{
					if(texture1.Width != Frame.Width || texture1.Height != Frame.Height)
					{
						texture1 = new Texture2D(Frame.Width ,Frame.Height,false,PixelFormat.Rgba);
						texture2 = new Texture2D(Frame.Width ,Frame.Height,false,PixelFormat.Rgba);
					}
					
					texture1.SetPixels(0,Frame.FrameBuffer1);
					texture2.SetPixels(0,Frame.FrameBuffer2);
					
					sceneMain.updateImage(texture1,texture2);
										
					//SendPacket.sendReady();
					//GC.Collect();
					
				}
				Draw.render(texture1, texture2);
				*/
					Draw.render(Frame.ScreenTexture);
				if(Frame.FrameReady == true)
				{
					SendPacket.sendReady();
					Frame.FrameReady = false;
				}
			}
			catch(Exception e)
			{
				Console.WriteLine(e.ToString());
			}
		}
	}
}
