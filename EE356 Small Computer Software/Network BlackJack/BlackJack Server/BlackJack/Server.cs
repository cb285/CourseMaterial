using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

public class Server
{
    private int numSupported;
    private int port;
    private bool running;
    private IPAddress ipAddr;
    private List<Client> clients;

    private List<int> availableClientNumbers;
    private List<int> usedClientNumbers;

    private BackgroundWorker ConnectionHandler;
    private BackgroundWorker DisconnectionHandler;

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

    public void RemoveClient(Client aClient)
    {
        if (clients.Contains(aClient))
            clients.Remove(aClient);

        availableClientNumbers.Add(aClient.GetClientNumber());
        usedClientNumbers.Remove(aClient.GetClientNumber());
    }

    public Server(int maxClients)
    {
        numSupported = maxClients;
        clients = new List<Client>();

        availableClientNumbers = new List<int>();
        usedClientNumbers = new List<int>();

        for (int i = 0; i < maxClients; i++)
            availableClientNumbers.Add(i);

        ConnectionHandler = new BackgroundWorker();
        DisconnectionHandler = new BackgroundWorker();
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

    public void SendAll(string message)
    {
        foreach (Client serverClient in clients)
            serverClient.Send(message);
    }

    public void SendClientNumber(int clientNum, string message)
    {
        foreach (Client serverClient in clients)
            if (serverClient.GetClientNumber() == clientNum)
            {
                serverClient.Send(message);
                return;
            }
    }

    public void SendClient(Client aClient, string message)
    {
        if (clients.Contains(aClient))
            aClient.Send(message);
    }

    public int GetNumAvail()
    {
        return numSupported - clients.Count;
    }

    public Client GetClient(int clientNum)
    {
        foreach (Client serverClient in clients)
            if (serverClient.GetClientNumber() == clientNum)
            {
                return serverClient;
            }

        return new Client();
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
            //newClient.Start();

            if (clients.Count == numSupported)
            {
                newClient.Send("Sorry, the server is currently at maximum capacity.");
                newClient.Send("disconnect");
                newClient.Stop();
            }
            else
            {
                newClient.RecievedMessage += new EventHandler<MyEventArgs>(Client_RecievedMessage);

                // add client to list
                clients.Add(newClient);

                // assign a client number
                newClient.AssignClientNumber(availableClientNumbers.First());

                usedClientNumbers.Add(availableClientNumbers.First());
                availableClientNumbers.Remove(newClient.GetClientNumber());

                // send the client number
                newClient.Send(newClient.GetClientNumber().ToString());

                OnClientConnected(new MyEventArgs("" + newClient.GetClientNumber()));
            }
        }
    }

    private void Client_RecievedMessage(object sender, MyEventArgs e)
    {
        string message = e.MyEventString;
        Client thisClient = (Client)sender;

        if (!thisClient.IsRunning())
        {
            clients.Remove(thisClient);
            
            usedClientNumbers.Remove(thisClient.GetClientNumber());
            availableClientNumbers.Add(thisClient.GetClientNumber());

            OnClientDisconnected(new MyEventArgs("-" + GetNumAvail()));
            return;
        }

        OnReceivedMessage(new MyEventArgs(message));

        if (message == "disconnect")
        {
            thisClient.Stop();
            clients.Remove((Client)sender);
            usedClientNumbers.Remove(thisClient.GetClientNumber());
            availableClientNumbers.Add(thisClient.GetClientNumber());
            OnClientDisconnected(new MyEventArgs("" + GetNumAvail()));
            return;
        }
        else
            SendAll(message);
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

    int clientNumber;

    bool running;

    public event EventHandler<MyEventArgs> RecievedMessage;

    public Client()
    {
        running = false;
    }

    public Client(Socket clientSocket)
    {
        ns = new NetworkStream(clientSocket);
        sr = new StreamReader(ns);
        sw = new StreamWriter(ns);
        clientbkw = new BackgroundWorker();
        clientbkw.WorkerSupportsCancellation = true;
        clientbkw.DoWork += new DoWorkEventHandler(client_DoWork);

        clientNumber = new int();

        running = false;
    }

    public void AssignClientNumber(int num)
    {
        clientNumber = num;
    }

    public int GetClientNumber()
    {
        return clientNumber;
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

class Player
{
    CardStack playerCardStack;
    int money;
    int bet;

    public bool isPlaying;

    bool busted;

    int playerNum;

    Client playerClient;

    public Player(int initMoney, Client newClient)
    {
        money = initMoney;
        bet = 0;

        busted = false;

        playerCardStack = new CardStack();

        playerClient = newClient;

        playerNum = newClient.GetClientNumber();

        isPlaying = true;
    }

    public void Deal(Card aCard)
    {
        playerCardStack.Add(aCard);
    }

    public void ClearCards()
    {
        playerClient.Send("clearcards");
    }

    public void AskForBet()
    {
        playerClient.Send("getbet");
    }

    public void AskHitOrStand()
    {
        playerClient.Send("hitorstand");
    }

    public string Read()
    {
        return  playerClient.Read();
    }

    public int GetPoints()
    {
        return playerCardStack.GetPoints();
    }

    public void Bet(int betDollars)
    {
        money -= betDollars;
        bet = betDollars;
    }

    public int GetBet()
    {
        return bet;
    }

    public void Bust()
    {
        busted = true;
    }

    public bool IsBusted()
    {
        return busted;
    }

    public List<Card> GetCards()
    {
        return playerCardStack.GetCards();
    }

    public int GetPlayerNum()
    {
        return playerNum;
    }

    public void Clear()
    {
        bet = 0;
        busted = false;
        playerCardStack.Clear();
    }

    public void SetMoney(int mon)
    {
        money = mon;
    }

    public int GetMoney()
    {
        return money;
    }

    public Client GetClient()
    {
        return playerClient;
    }

    public void OutOfMoney()
    {
        playerClient.Send("outofmoney");
        playerClient.Send("disconnect");
    }
}