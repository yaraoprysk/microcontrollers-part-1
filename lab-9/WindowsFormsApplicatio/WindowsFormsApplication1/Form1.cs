using System;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.IO.Ports;
using System.Drawing;
using System.Threading;
using System.Text.RegularExpressions;
using System.Collections.Generic;
using System.Text;

namespace WindowsFormsApplication1 {
    public partial class Form1 : Form {
        const byte SLAVE1_ADDRESS = 0xE9;
        const byte SLAVE2_ADDRESS = 0xA7;

        public Form1() {
            InitializeComponent();
        }
        
        private void comboBox1_Click(object sender, EventArgs e) {
            int num;
            comboBox1.Items.Clear();
            string[] ports = SerialPort.GetPortNames().OrderBy(a => a.Length > 3 && int.TryParse(a.Substring(3), out num) ? num : 0).ToArray();
            comboBox1.Items.AddRange(ports);
        }

        private void buttonOpenPort_Click(object sender, EventArgs e) {
            if (!serialPort1.IsOpen) {
                try {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.Open();
                    buttonOpenPort.Text = "Close";
                    comboBox1.Enabled = false;
                    buttonFromSlave1.Visible = true;
                    buttonFromSlave2.Visible = true;
                } catch {
                    MessageBox.Show("Port " + comboBox1.Text + " is invalid!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            } else {
                serialPort1.Close();
                buttonOpenPort.Text = "Open";
                comboBox1.Enabled = true;
                buttonFromSlave1.Visible = false;
                buttonFromSlave2.Visible = false;
            }
        }

        private void buttonFromSlave1_Click(object sender, EventArgs e) {
            byte[] b1 = new byte[3];
            b1[0] = SLAVE1_ADDRESS;
            b1[1] = 0xB1;
            serialPort1.Write(b1, 0, 2);
            TextBox[] myArr = { textBox1, textBox3, textBox4, textBox5, textBox6 };
            for (int k = 0; k < 5; k++)
            {
                List<string> nameList = new List<string>();
                List<byte> byteList = new List<byte>();
                List<byte> reflectedByteList = new List<byte>();
                List<byte> checkSumList = new List<byte>();
                for (int i = 0; i < 24; i++)
                {
                    byte info = (byte)serialPort1.ReadByte();
                    Console.WriteLine(info);
                    byteList.Add(info);
                    switch (info)
                    {
                        case 0x01:
                            nameList.Add("Y");
                            break;
                        case 0x02:
                            nameList.Add("A");
                            break;
                        case 0x03:
                            nameList.Add("R");
                            break;
                        case 0x04:
                            nameList.Add("N");
                            break;
                        case 0x05:
                            nameList.Add("O");
                            break;
                        case 0x06:
                            nameList.Add("P");
                            break;
                        case 0x07:
                            nameList.Add("S");
                            break;
                      
                        case 0x08:
                            nameList.Add("K");
                            break;
                        case 0x09:
                            nameList.Add("L");
                            break;
                        case 0x0A:
                            nameList.Add("E");
                            break;
                        case 0x0B:
                            nameList.Add("G");
                            break;
                        case 0x0C:
                            nameList.Add("I");
                            break;
                        case 0x0D:
                            nameList.Add("V");
                            break;
                  
                        case 0x00:
                            nameList.Add(" ");
                            break;
                        default:
                            nameList.Add("");
                            break;
                    };

                    if (i == 22)
                    {
                        byteList.Remove(info);
                        checkSumList.Add(info);
                    }
                    if (i == 23)
                    {
                        byteList.Remove(info);
                        checkSumList.Add(info);
                    }

                }
                StringBuilder builder = new StringBuilder();
                foreach (string name in nameList)
                {
                    builder.Append(name);
                }

                byte[] bytes = byteList.ToArray();
                byte[] checkSumbytes = checkSumList.ToArray();

                ushort checkSumValue = (ushort)CombineBytes(checkSumbytes[0], checkSumbytes[1]);

                foreach (byte b in bytes)
                {
                    byte reflectedByte = ReverseByte(b);
                    reflectedByteList.Add(reflectedByte);
                }
                byte[] reflectedBytes = reflectedByteList.ToArray();

                ushort myUshortResult = Compute_CRC16(reflectedBytes);

                string result = builder.ToString();
                if (checkSumValue == myUshortResult)
                {
                    myArr[k].Text = result.ToString();

                }
                else
                {
                    myArr[k].Text = "Дані спотворені! " + checkSumValue.ToString() + " vs " + myUshortResult.ToString();
                }
            }
        }

        private void buttonFromSlave2_Click(object sender, EventArgs e) {
            byte[] b1 = new byte[3];
            b1[0] = SLAVE2_ADDRESS;
            b1[1] = 0xB1;
            serialPort1.Write(b1, 0, 2);
            TextBox[] myArr = { textBox2, textBox7, textBox8, textBox9, textBox10 };
            for (int k = 0; k < 5; k++)
            {
                List<string> numberList = new List<string>();
                List<byte> byteList = new List<byte>();
                List<byte> reflectedByteList = new List<byte>();
                List<byte> checkSumList = new List<byte>();

                for (int i = 0; i < 12; i++)
                {
                    byte info = (byte)serialPort1.ReadByte();
                    Console.WriteLine(info);
                    byteList.Add(info);

                    switch (info)
                    {
                        case 0x00:
                            numberList.Add("0");
                            break;

                        case 0x02:
                            numberList.Add("2");
                            break;

                        case 0x05:
                            numberList.Add("5");
                            break;

                        case 0x07:
                            numberList.Add("7");
                            break;


                        case 0xFF:
                            numberList.Add(".");
                            break;
                        default:
                            numberList.Add("");
                            break;
                    };

                    if (i == 10)
                    {
                        byteList.Remove(info);
                        checkSumList.Add(info);
                    }
                    if (i == 11)
                    {
                        byteList.Remove(info);
                        checkSumList.Add(info);
                    }

                }
                StringBuilder builder = new StringBuilder();
                foreach (string date in numberList)
                {
                    builder.Append(date);
                }

                Console.WriteLine(builder);
                byte[] bytes = byteList.ToArray();
                byte[] checkSumbytes = checkSumList.ToArray();

                ushort checkSumValue = (ushort)CombineBytes(checkSumbytes[0], checkSumbytes[1]);

                foreach (byte b in bytes)
                {
                    byte reflectedByte = ReverseByte(b);
                    reflectedByteList.Add(reflectedByte);
                }

                byte[] reflectedBytes = reflectedByteList.ToArray();
                ushort myUshortResult = Compute_CRC16(reflectedBytes);

                string result = builder.ToString();
                if (checkSumValue == myUshortResult)
                {
                    myArr[k].Text = result.ToString();

                }
                else
                {

                    myArr[k].Text = "Дані спотворені! " + checkSumValue.ToString() + " vs " + myUshortResult.ToString();
                }
            }
            
        }

        public static ushort Compute_CRC16(byte[] bytes) {
            const ushort generator = 0x1021;
            ushort crc = 0x1D0F;

            foreach (byte b in bytes) {
                crc ^= (ushort) (b << 8);

                for (int i = 0; i < 5; i++) {
                    if ((crc & 0x8000) != 0) {
                        crc = (ushort) ((crc << 1) ^ generator);
                    } else {
                        crc <<= 1;
                    }
                }
            }
            ushort myNewResult = Reflect16(crc);
            return myNewResult;
        }

        public static ushort Reflect16(ushort val) {
            ushort resVal = 0;

            for (int i = 0; i < 16; i++) {
                if ((val & (1 << i)) != 0) {
                    resVal |= (ushort)(1 << (15 - i));
                }
            }

            return resVal;
        }

        public static byte ReverseByte(byte b) {
            int a = 0;
            for (int i = 0; i < 8; i++) {
                if ((b & (1 << i)) != 0) {
                    a |= 1 << (7 - i);
                }
            }
            return (byte) a;
        }

        public int CombineBytes(byte b1, byte b2) {
            int combined = b1 << 8 | b2;
            return combined;
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}
