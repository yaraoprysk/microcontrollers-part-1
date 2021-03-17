using System;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.IO.Ports;
using System.Drawing;
using System.Threading;

namespace WindowsFormsApplication1
{   
    public partial class Form1 : Form
    {
        private int algorithmCount = 0;
        private int algorithmNumber;

        public Form1()
        {
            InitializeComponent();
        }
        
        private void comboBox1_Click(object sender, EventArgs e)
        {
            int num;
            comboBox1.Items.Clear();
            string[] ports = SerialPort.GetPortNames().OrderBy(a => a.Length > 3 && int.TryParse(a.Substring(3), out num) ? num : 0).ToArray();
            comboBox1.Items.AddRange(ports);
        }

        private void buttonOpenPort_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
                try
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.Open();
                    buttonOpenPort.Text = "Close";
                    comboBox1.Enabled = false;
                    button1.Visible = true;
                    button2.Visible = true;
                }
                catch
                {
                    MessageBox.Show("Port " + comboBox1.Text + " is invalid!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            else
            {
                serialPort1.Close();
                buttonOpenPort.Text = "Open";
                comboBox1.Enabled = true;
                button1.Visible = false;
                button2.Visible = false;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            serialPort1.Write("1");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort1.Write("2");
        }

        private void clearAllLed()
        {
            panel1.BackColor = Color.SkyBlue;
            panel2.BackColor = Color.SkyBlue;
            panel3.BackColor = Color.SkyBlue;
            panel4.BackColor = Color.SkyBlue;
            panel5.BackColor = Color.SkyBlue;
            panel6.BackColor = Color.SkyBlue;
            panel7.BackColor = Color.SkyBlue;
            panel8.BackColor = Color.SkyBlue;
        }

        private void Algorithms_1()
        {
            clearAllLed();
            panel8.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel8.BackColor = Color.SkyBlue;

            panel1.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel1.BackColor = Color.SkyBlue;

            panel7.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel7.BackColor = Color.SkyBlue;

            panel2.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel2.BackColor = Color.SkyBlue;

            panel6.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel6.BackColor = Color.SkyBlue;

            panel3.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel3.BackColor = Color.SkyBlue;

            panel5.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel5.BackColor = Color.SkyBlue;

            panel4.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel4.BackColor = Color.SkyBlue;
        }

        private void Algorithms_2()
        {
            clearAllLed();
            panel8.BackColor = Color.DodgerBlue;
            panel1.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel8.BackColor = Color.SkyBlue;
            panel1.BackColor = Color.SkyBlue;

            panel7.BackColor = Color.DodgerBlue;
            panel2.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel7.BackColor = Color.SkyBlue;
            panel2.BackColor = Color.SkyBlue;

            panel6.BackColor = Color.DodgerBlue;
            panel3.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel6.BackColor = Color.SkyBlue;
            panel3.BackColor = Color.SkyBlue;

            panel5.BackColor = Color.DodgerBlue;
            panel4.BackColor = Color.DodgerBlue;
            Thread.Sleep(1150);
            panel5.BackColor = Color.SkyBlue;
            panel4.BackColor = Color.SkyBlue;
        }

        private void startTimer()
        {
            timer1.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            clearAllLed();
            algorithmCount++;
            if (algorithmCount < 6)
                if (algorithmCount % 2 == 1)
                {
                    panel1.BackColor = Color.Red;
                    panel2.BackColor = Color.Red;
                    panel3.BackColor = Color.Red;
                }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            char commandFromArduino = (char)serialPort1.ReadChar();
            if (commandFromArduino == '1')
            {
                Algorithms_1();
            }
            else if (commandFromArduino == '2')
            {
                Algorithms_2();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void panel3_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel5_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel4_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
