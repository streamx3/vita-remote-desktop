using System;
using System.Collections.Generic;
using Sce.PlayStation.Core;
using Sce.PlayStation.Core.Imaging;
using Sce.PlayStation.Core.Environment;
using Sce.PlayStation.HighLevel.UI;

namespace VitaRemoteClient 
{
	partial class MainUI  
	{
		Label lblOutput;
		ImageBox imgDesktop;
		ImageBox imgDesktop2;
		ImageBox dummy;
		
        Label FPS_Label;
		
		//FlickGestureDetector flick_gd;
		TapGestureDetector tap_gd;
		DoubleTapGestureDetector dtap_gd;
		DragGestureDetector drag_gd;
		LongPressGestureDetector longPress_gd;
		
		private LayoutOrientation _currentLayoutOrientation;	
		private void InitializeWidget()
        {
            InitializeWidget(LayoutOrientation.Horizontal);
        }
		
		 private void InitializeWidget(LayoutOrientation orientation)
        {
			
			tap_gd = new TapGestureDetector();
			dtap_gd = new DoubleTapGestureDetector();
			drag_gd = new DragGestureDetector();
			longPress_gd = new LongPressGestureDetector();
			FPS_Label = new Label();
            FPS_Label.Name = "FPS Label";

            // FPS_Label
            FPS_Label.TextColor = new UIColor(0f / 255f, 0f / 255f, 0f / 255f, 255f / 255f);
            FPS_Label.Font = new UIFont(FontAlias.System, 25, FontStyle.Regular);
            FPS_Label.LineBreak = LineBreak.Character;
			
			tap_gd.TapDetected +=  Tap;
			dtap_gd.DoubleTapDetected += DoubleTap;
			drag_gd.DragDetected +=  Drag;
			drag_gd.DragStartDetected +=  dragStart;
			drag_gd.DragEndDetected +=  dragEnd;
			longPress_gd.LongPressDetected +=  LongPress;
			
			lblOutput = new Label();
			
			imgDesktop = new ImageBox();
			imgDesktop2 = new ImageBox();
			dummy = new ImageBox();
			
			dummy.AddGestureDetector(tap_gd);
			dummy.AddGestureDetector(dtap_gd);
			dummy.AddGestureDetector(drag_gd);
			dummy.AddGestureDetector(longPress_gd);
			
			imgDesktop.ImageScaleType = ImageScaleType.Stretch;
			
			this.RootWidget.AddChildLast(imgDesktop);
			this.RootWidget.AddChildLast(imgDesktop2);
			this.RootWidget.AddChildLast(dummy);
			this.RootWidget.AddChildLast(lblOutput);
            this.RootWidget.AddChildLast(FPS_Label);
			
		
			SetWidgetLayout(orientation);	
		}

		 void  flick (object sender, FlickEventArgs e)
		 {
		 	Console.WriteLine("flicked");
		 }

		public void SetWidgetLayout(LayoutOrientation orientation)
		{
			switch (orientation)
			{
				case LayoutOrientation.Vertical:
					this.DesignWidth = 544;
                	this.DesignHeight = 960;
				
                	imgDesktop.SetPosition(471, 116);
                	imgDesktop.SetSize(200, 200);
                	imgDesktop.Anchors = Anchors.Top | Anchors.Height | Anchors.Left | Anchors.Width;
                	imgDesktop.Visible = false;
				
                    FPS_Label.SetPosition(734, 498);
                    FPS_Label.SetSize(214, 36);
                    FPS_Label.Anchors = Anchors.None;
                    FPS_Label.Visible = true;

                	break;

            	default:
                	this.DesignWidth = 960;
                	this.DesignHeight = 544;
			
                	imgDesktop.SetPosition(0, 0);
                	imgDesktop.SetSize(470, 544);
					imgDesktop2.SetPosition(470, 0);
                	imgDesktop2.SetSize(470, 544);
					dummy.SetSize(940,544);
					dummy.SetPosition(0,0);
					dummy.Visible = true;
                	imgDesktop.Anchors = Anchors.Top | Anchors.Height | Anchors.Left | Anchors.Width;
                	imgDesktop.Visible = true;
					imgDesktop2.Visible = true;
				
                    FPS_Label.SetPosition(734, 498);
                    FPS_Label.SetSize(214, 36);
                    FPS_Label.Anchors = Anchors.None;
                    FPS_Label.Visible = true;

					break;
			}
			_currentLayoutOrientation = orientation;
		}
		
		private void onShowing(object sender, EventArgs e)
        {
            switch (_currentLayoutOrientation)
            {
                case LayoutOrientation.Vertical:
                {
                }
                break;

                default:
                {
                }
                break;
            }
        }
        private void onShown(object sender, EventArgs e)
        {
            switch (_currentLayoutOrientation)
            {
                case LayoutOrientation.Vertical:
                {
                }
                break;

                default:
                {
                }
                break;
            }
        }
	}
}
	