using System;
using System.Collections.Generic;
using Sce.PlayStation.Core;
using Sce.PlayStation.Core.Imaging;
using Sce.PlayStation.Core.Environment;
using Sce.PlayStation.HighLevel.UI;

namespace VitaRemoteClient
{
    public partial class MainUI : Scene
    {
        public MainUI()
        {
            InitializeWidget();
        }
		
		public void UpdateFPSLabel(string str)
		{
			Label_1.Text = str;
		}
    }
}
