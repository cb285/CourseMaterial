using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        NetworkStream ns;
        StreamReader sr;
        StreamWriter sw;
        delegate void SetTextCallback(String text);
        delegate void EnableButtonCallback();
        BackgroundWorker backgroundWorker = new BackgroundWorker();

        string userName;

        public MainWindow()
        {
            InitializeComponent();
            button_Send.IsEnabled = false;
            textBox_Message.IsEnabled = false;
            button_Disconnect.IsEnabled = false;

            backgroundWorker.DoWork += new DoWorkEventHandler(backgroundWorker_DoWork);
            backgroundWorker.WorkerSupportsCancellation = true;
        }

        // Function: button_Connect_Click
        // runs when user clicks connect button
        // connects to the server
        private void button_Connect_Click(object sender, RoutedEventArgs e)
        {
            // if didn't enter a name
            if (textBox_Name.Text == "" || textBox_Name.Text.ToLower() == "server")
            {
                // tell user to enter a name
                MessageBox.Show("Enter a valid name first!", "Error", MessageBoxButton.OK);
                return;
            }

            IPAddress ipaddr;

            if (!IPAddress.TryParse(textBox_IPAddress.Text, out ipaddr))
            {
                // tell user to enter a valid IP
                MessageBox.Show("Enter a valid IP address!", "Error", MessageBoxButton.OK);
                return;
            }

            int port;

            if (!int.TryParse(textBox_Port.Text, out port))
            {
                // tell user to enter a name
                MessageBox.Show("Enter a name first!", "Error", MessageBoxButton.OK);
                return;
            }

            // get user's name
            userName = textBox_Name.Text;

            // disable the name textbox and connect button
            textBox_Name.IsEnabled = false;
            button_Connect.IsEnabled = false;

            // create a TCP client
            TcpClient newcon = new TcpClient();
            // try to connect to the server
            try
            {
                newcon.Connect(ipaddr, port);
            }
            catch
            {
                // if couldn't connect, then display error message
                MessageBox.Show("Could not connect to server. Check that server is running and accessible.", "Connection Error", MessageBoxButton.OK);
                textBox_Name.IsEnabled = true;
                button_Connect.IsEnabled = true;
                return;
            }
            // get the network stream, stream reader, and stream writer
            ns = newcon.GetStream();
            sr = new StreamReader(ns);  //Stream Reader and Writer take away some of the overhead of keeping track of Message size.  By Default WriteLine and ReadLine use Line Feed to delimit the messages
            sw = new StreamWriter(ns);

            // start the background worker
            backgroundWorker.RunWorkerAsync();

            // enable the send button
            button_Send.IsEnabled = true;

            textBox_Message.IsEnabled = true;
            button_Disconnect.IsEnabled = true;
        }

        // Function: button_Send_Click
        // runs when user clicks send button
        // sends the message to the server
        private void button_Send_Click(object sender, RoutedEventArgs e)
        {
            SendHandler();
        }

        private void SendHandler()
        {
            if (textBox_Message.Text == "")
                return;

            // check if user entered "disconnect"
            if (textBox_Message.Text == "disconnect")
            {
                // close the connection
                Disconnect();
            }
            // otherwise, just send the message
            else
            {
                string message = userName + " > " + textBox_Message.Text;
                // send text to server
                sw.WriteLine(message);
                sw.Flush();
            }

            // clear message text box
            textBox_Message.Text = "";
        }

        // Function: backgroundWorker_DoWork
        // runs while client is connected to server
        // gets data from server 
        private void backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            // loop while connected
            while (!backgroundWorker.CancellationPending)
            {
                try
                {
                    // get message from the server (blocking function)
                    string inputStream = sr.ReadLine();  //Note Read only reads into a byte array.  Also Note that Read is a "Blocking Function"

                    // check if the message was "disconnect"
                    if (inputStream == "disconnect")
                    {
                        // send disconnect message
                        sw.WriteLine("disconnect");
                        sw.Flush();
                        Disconnect();
                        return;
                    }
                    else
                        // add the message to the listBox
                        addText(inputStream);
                }
                catch
                {
                    if (backgroundWorker.CancellationPending)
                        return;

                    // if no longer connected, then disconnect
                    Disconnect();
                }
            }
        }

        // Function: addText
        // adds text to the client's listBox, doesn't send it.
        // input: a string to add
        private void addText(string text)
        {
            // check if have access to write to the listBox
            if (listBox.Dispatcher.CheckAccess())
            {
                listBox.Items.Add(text);
            }
            else
            {
                // get access
                listBox.Dispatcher.BeginInvoke(new SetTextCallback(addText), text);
            }
        }

        private void Disconnect()
        {
            if (button_Connect.Dispatcher.CheckAccess())
            {
                button_Disconnect.IsEnabled = false;
                button_Connect.IsEnabled = true;
                textBox_Name.IsEnabled = true;
                button_Send.IsEnabled = false;
                textBox_Message.IsEnabled = false;

                backgroundWorker.CancelAsync();

                sr.Close();
                sw.Close();
                ns.Close();
                addText("disconnected from server.");
            }
            else
                button_Connect.Dispatcher.BeginInvoke(new EnableButtonCallback(Disconnect));
        }

        // Function: Window_Closing
        // runs when user closes window
        // properly disconnects client from server
        private void Window_Closing(object sender, CancelEventArgs e)
        {
            if (button_Disconnect.IsEnabled)
            {
                // send disconnect message
                sw.WriteLine("disconnect");
                sw.Flush();
                Disconnect();
            }
        }

        private void textBox_Message_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
                SendHandler();
        }

        private void button_Disconnect_Click(object sender, RoutedEventArgs e)
        {
            // send disconnect message
            sw.WriteLine("disconnect");
            sw.Flush();
            Disconnect();
        }
    }
}
