using System.IO.Ports;
using System.Text;
using System.Timers;
using System.Windows;

namespace Sound
{
    public partial class MainWindow : Window
    {
        System.Timers.Timer timer;
        SerialPort port;
        
        private delegate void updateDelegate(string txt);
        public MainWindow()
        {
            InitializeComponent();
            
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            bool ArduinoPortFound = false;
            try
            {
                string[] ports = SerialPort.GetPortNames();
                foreach (string p in ports)
                {

                    port = new SerialPort(p, 9600);

                    if (ArduinoDetected())
                    {
                        ArduinoPortFound = true;
                        break;
                    }
                    else
                    {
                        ArduinoPortFound = false;
                    }
                }
            }
            catch { }

            if (ArduinoPortFound)
            {
                System.Threading.Thread.Sleep(1000);

                port.BaudRate = 9600;
                port.DtrEnable = true;
                port.ReadTimeout = 1000;

                try { port.Open(); }
                catch { }

                timer = new System.Timers.Timer();
                timer.Elapsed += OnTimedEvent;
                timer.AutoReset = true;
                timer.Enabled = true;
            }
        }

        private void OnTimedEvent(object? sender, ElapsedEventArgs e)
        {
            if (!port.IsOpen) { return; }

            try 
            { 
                port.DiscardInBuffer();
                port.Encoding = Encoding.UTF8;
                string mes = port.ReadLine();
                lbl.Dispatcher.BeginInvoke(new updateDelegate(updateTextBox), mes);
            } 
            catch { }
        }

        private void updateTextBox(string txt)
        {
            lbl.Content = txt;
        }

        private bool ArduinoDetected()
        {
            try
            {
                port.Open();
                System.Threading.Thread.Sleep(1000);

                string message = port.ReadLine();
                port.Close();

                if (message.Contains("Arduino")) { return true; }

                return false;
            }
            catch { return false; }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            timer.Enabled = false;
            port.Close();
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!port.IsOpen) return;

                if (CmbBx.SelectedIndex == 0)
                {
                    port.Write("1");
                }
                else if (CmbBx.SelectedIndex == 1)
                {
                    port.Write("2");
                }
                else if (CmbBx.SelectedIndex == 2)
                {
                    port.Write("3");
                }
}
            catch {}
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            if (!port.IsOpen) return;
            port.Write("s");
        }
    }
}