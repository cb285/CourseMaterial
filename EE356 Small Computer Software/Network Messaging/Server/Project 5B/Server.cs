using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Server
{
    public class Server
    {
        private int numSupported;
        private int port;
        private bool running;
        private IPAddress ipAddr;
        private List<Client> clients;
        private BackgroundWorker ConnectionHandler;

        public event EventHandler<MyEventArgs> RecievedMessage;
        public event EventHandler<MyEventArgs> ClientConnected;
        public event EventHandler<MyEventArgs> ClientDisconnected;

        // Invoke the Changed event; called whenever list changes
        protected virtual void OnReceivedMessage(MyEventArgs e)
        {
            RecievedMessage?.Invoke(this, e);
        }

        protected virtual void OnClientConnected(MyEventArgs e)
        {
            ClientConnected?.Invoke(this, e);
        }

        protected virtual void OnClientDisconnected(MyEventArgs e)
        {
            ClientDisconnected?.Invoke(this, e);
        }

        public Server(int maxClients)
        {
            numSupported = maxClients;
            List<Client> clients = new List<Client>();
            ConnectionHandler = new BackgroundWorker();
            running = false;
        }

        public void Start(int serverPort)
        {
            port = serverPort;
            ipAddr = GetLocalIPAddress();

            clients = new List<Client>();

            ConnectionHandler.DoWork += new DoWorkEventHandler(ConnectionHandler_DoWork);
            ConnectionHandler.WorkerSupportsCancellation = true;
            ConnectionHandler.RunWorkerAsync(port);
            running = true;
        }

        public void Stop()
        {
            foreach (Client serverClient in clients)
            {
                serverClient.Send("disconnect");
                serverClient.Stop();
            }

            clients.Clear();
            ConnectionHandler.CancelAsync();
        }

        public bool IsRunning()
        {
            return running;
        }

        public void Send(string message)
        {
            foreach (Client serverClient in clients)
                serverClient.Send(message);
        }

        public int GetNumAvail()
        {
            return numSupported - clients.Count;
        }

        // Function: serverBkw_DoWork
        // runs when server is started
        // sets up server for connecting to clients
        private void ConnectionHandler_DoWork(object sender, DoWorkEventArgs e)
        {
            int port = (int)e.Argument;

            // create a TCP Listener
            TcpListener newsocket = new TcpListener(IPAddress.Any, port);

            // start the TCP Listener
            newsocket.Start();

            // accept connections while server hasn't reached max client number
            while (true)
            {
                if (!System.Net.NetworkInformation.NetworkInterface.GetIsNetworkAvailable())
                {
                    Stop();
                    throw new Exception("Network connection failed, stopping server");
                }

                if (ConnectionHandler.CancellationPending)
                    return;

                // check for pending connections or pending cancelation
                while (!newsocket.Pending() && !ConnectionHandler.CancellationPending) ;

                // if server is being canceled, then stop the background worker
                if (ConnectionHandler.CancellationPending)
                {
                    newsocket.Stop();
                    return;
                }

                // Accept new connection
                Client newClient = new Client(newsocket.AcceptSocket());
                newClient.Start();

                if (clients.Count == numSupported)
                {
                    newClient.Send("Sorry, the server is currently at maximum capacity.");
                    newClient.Send("disconnect");
                    newClient.Stop();
                }
                else
                {
                    newClient.Send("Welcome!");

                    newClient.RecievedMessage += new EventHandler<MyEventArgs>(Client_RecievedMessage);

                    // add client to list
                    clients.Add(newClient);

                    OnClientConnected(new MyEventArgs("" + GetNumAvail()));
                }
            }
        }

        private void Client_RecievedMessage(object sender, MyEventArgs e)
        {
            string message = e.MyEventString;
            Client thisClient = (Client)sender;

            if (!thisClient.IsRunning())
            {
                clients.Remove((Client)sender);
                OnClientDisconnected(new MyEventArgs("" + GetNumAvail()));
                return;
            }

            OnReceivedMessage(new MyEventArgs(message));

            if (message == "disconnect")
            {
                thisClient.Stop();
                clients.Remove((Client)sender);
                OnClientDisconnected(new MyEventArgs("" + GetNumAvail()));
                return;
            }
            else
                Send(message);
        }

        public int GetPortNumber()
        {
            return port;
        }

        public IPAddress GetIPAddress()
        {
            return ipAddr;
        }

        private IPAddress GetLocalIPAddress()
        {
            var host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (var ip in host.AddressList)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    return ip;
                }
            }
            throw new Exception("Local IP Address Not Found!");
        }
    }

    public class Client
    {
        NetworkStream ns;
        StreamReader sr;
        StreamWriter sw;
        BackgroundWorker clientbkw;

        bool running;

        string userName;

        public event EventHandler<MyEventArgs> RecievedMessage;

        public Client(Socket clientSocket)
        {
            ns = new NetworkStream(clientSocket);
            sr = new StreamReader(ns);
            sw = new StreamWriter(ns);
            clientbkw = new BackgroundWorker();
            clientbkw.WorkerSupportsCancellation = true;
            clientbkw.DoWork += new DoWorkEventHandler(client_DoWork);

            running = false;
        }

        public void Start()
        {
            clientbkw.RunWorkerAsync();
            running = true;
        }

        public void Stop()
        {
            sw.Close();
            sr.Close();
            ns.Close();
            clientbkw.CancelAsync();

            running = false;
        }

        public bool IsRunning()
        {
            return running;
        }

        private void client_DoWork(object sender, DoWorkEventArgs e)
        {
            while (!clientbkw.CancellationPending)
            {
                try
                {
                    string message = Read();

                    if (message == "disconnect")
                        Stop();

                    OnReceivedMessage(new MyEventArgs(message));
                }
                catch
                {
                    Stop();
                }
            }
        }

        // Invoke the Changed event; called whenever message recieved
        protected virtual void OnReceivedMessage(MyEventArgs e)
        {
            RecievedMessage?.Invoke(this, e);
        }

        public void Send(string message)
        {
            sw.WriteLine(message);
            sw.Flush();
        }

        public bool MessagePending()
        {
            return (sr.Peek() != -1);
        }

        public string Read()
        {
            return sr.ReadLine();
        }
    }
}
