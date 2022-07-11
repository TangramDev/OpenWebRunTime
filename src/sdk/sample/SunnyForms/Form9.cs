﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Universe;

namespace SunnyForms
{
    public partial class Form9 : Form
    {
        Xobj hostXobj = null;
        public Form9()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void Form9_Load(object sender, EventArgs e)
        {
            hostXobj = WebRT.GetXobjFromControl(this);
            if (hostXobj != null)
                hostXobj.OnCloudMessageReceived += HostXobj_OnCloudMessageReceived;
        }

        private void HostXobj_OnCloudMessageReceived(Wormhole cloudSession)
        {
        }
    }
}
