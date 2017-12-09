using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;

namespace BlackJackClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // create network streams
        NetworkStream ns;
        StreamReader sr;
        StreamWriter sw;
        
        // create background worker
        BackgroundWorker backgroundWorker = new BackgroundWorker();

        // create deleagates for updating GUI
        delegate void SetTextCallback(String text);
        delegate void EnableButtonCallback();
        delegate void DealCardCallback(Card aCard, int playerNum);
        delegate void ButtonChangerCallBack(Button theButton, string func);
        delegate void SetLabelCallback(Label aLabel, string content);
        delegate void BetBoxChangerCallBack(string func);
        delegate void ClearImagesCallBack();

        // create vars for saving player number and money amount
        int myPlayerNum;
        int myMoney = 0;

        // create bools for player's status
        bool ready = true;
        bool connected = false;

        // create var for game over message
        string overMessage = "";

        // create storage for card animation parameters
        double dealerXMove, dealerYMove;
        List<Image> dealerCards = new List<Image>();
        
        // create list of Players
        List<Player> playerList = new List<Player>();

        // custom class: Player
        class Player
        {
            List<Image> playerCards;
            double xMove;
            double yMove;
            Label playerNumLabel;
            Label playerBetLabel;
            Label playerMoneyLabel;

            public Player()
            {
                playerCards = new List<Image>();
                xMove = 0;
                yMove = 0;
            }

            public void SetXMove(double x)
            {
                xMove = x;
            }

            public void SetYMove(double y)
            {
                yMove = y;
            }

            public void SetPlayerNumLabel(Label aLabel)
            {
                playerNumLabel = aLabel;
            }

            public void SetPlayerBetLabel(Label aLabel)
            {
                playerBetLabel = aLabel;
            }

            public void SetPlayerMoneyLabel(Label aLabel)
            {
                playerMoneyLabel = aLabel;
            }

            public double GetXMove()
            {
                return xMove + playerCards.Count * 15;
            }

            public double GetYMove()
            {
                return yMove;
            }

            public void AddCard(Image cardImage)
            {
                playerCards.Add(cardImage);
            }

            public void ClearCards()
            {
                playerCards.Clear();
            }

            public List<Image> GetCards()
            {
                return playerCards;
            }

            public Label GetNumLabel()
            {
                return playerNumLabel;
            }

            public Label GetBetLabel()
            {
                return playerBetLabel;
            }

            public Label GetMoneyLabel()
            {
                return playerMoneyLabel;
            }
        }

        // open window
        public MainWindow()
        {
            InitializeComponent();

            // setup background worker
            backgroundWorker.DoWork += new DoWorkEventHandler(backgroundWorker_DoWork);
            backgroundWorker.WorkerSupportsCancellation = true;

            // disable the buttons
            button_Bet.IsEnabled = false;
            button_Hit.IsEnabled = false;
            button_Stand.IsEnabled = false;
        }

        // Function: button_Connect_Click
        // runs when user clicks connect button
        // connects to the server
        private void button_Connect_Click(object sender, RoutedEventArgs e)
        {
            IPAddress ipaddr;

            if (!IPAddress.TryParse(textBox_IP.Text, out ipaddr))
            {
                // tell user to enter a valid IP
                MessageBox.Show("Enter a valid IP address!", "Error", MessageBoxButton.OK);
                return;
            }

            int port;

            if (!int.TryParse(textBox_Port.Text, out port))
            {
                // tell user to enter a name
                MessageBox.Show("Enter a valid port number!", "Error", MessageBoxButton.OK);
                return;
            }

            // disable the connect button
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
                button_Connect.IsEnabled = true;
                return;
            }

            // get the network stream, stream reader, and stream writer
            ns = newcon.GetStream();
            sr = new StreamReader(ns);  //Stream Reader and Writer take away some of the overhead of keeping track of Message size.  By Default WriteLine and ReadLine use Line Feed to delimit the messages
            sw = new StreamWriter(ns);

            // get the player number from the server
            if (!int.TryParse(sr.ReadLine(), out myPlayerNum))
                throw new Exception("invalid player number from server");

            connected = true;

            SetupTable(myPlayerNum);

            // start the background worker
            backgroundWorker.RunWorkerAsync();
        }

        // Function: SetupTable
        // given this player's number, sets up player labels and animation paramters
        private void SetupTable(int thisPlayerNum)
        {
            // add players to list
            for (int i = 0; i < 5; i++)
                playerList.Add(new Player());

            // get locations to deal cards
            double XLoc = label_Dealer.Margin.Left;
            double YLoc = label_Dealer.Margin.Top - image_CardStack.Height; ;

            dealerXMove = XLoc - image_CardStack.Margin.Left;
            dealerYMove = YLoc - image_CardStack.Margin.Top;

            // label_Bot:
            // calculate move distances for dealing cards
            playerList[thisPlayerNum].SetPlayerNumLabel(label_Bot);
            playerList[thisPlayerNum].SetPlayerMoneyLabel(label_BotMoney);
            playerList[thisPlayerNum].SetPlayerBetLabel(label_BotBet);

            XLoc = label_Bot.Margin.Left;
            YLoc = label_Bot.Margin.Top - image_CardStack.Height;

            playerList[thisPlayerNum].SetXMove(XLoc - image_CardStack.Margin.Left);
            playerList[thisPlayerNum].SetYMove(YLoc - image_CardStack.Margin.Top);

            // set label content to "player #"
            SetLabelContent(label_Bot, "Player " + (thisPlayerNum + 1));

            thisPlayerNum++;

            if (thisPlayerNum == 5)
                thisPlayerNum = 0;

            // label_BotRight:
            // calculate move distances for dealing cards
            playerList[thisPlayerNum].SetPlayerNumLabel(label_BotRight);
            playerList[thisPlayerNum].SetPlayerMoneyLabel(label_BotRightMoney);
            playerList[thisPlayerNum].SetPlayerBetLabel(label_BotRightBet);

            XLoc = label_BotRight.Margin.Left;
            YLoc = label_BotRight.Margin.Top - image_CardStack.Height;

            playerList[thisPlayerNum].SetXMove(XLoc - image_CardStack.Margin.Left);
            playerList[thisPlayerNum].SetYMove(YLoc - image_CardStack.Margin.Top);

            // set label content to "player #"
            SetLabelContent(label_BotRight, "Player " + (thisPlayerNum + 1));

            thisPlayerNum++;

            if (thisPlayerNum == 5)
                thisPlayerNum = 0;

            // label_Right:
            // calculate move distances for dealing cards
            playerList[thisPlayerNum].SetPlayerNumLabel(label_Right);
            playerList[thisPlayerNum].SetPlayerMoneyLabel(label_RightMoney);
            playerList[thisPlayerNum].SetPlayerBetLabel(label_RightBet);

            XLoc = label_Right.Margin.Left;
            YLoc = label_Right.Margin.Top - image_CardStack.Height;

            playerList[thisPlayerNum].SetXMove(XLoc - image_CardStack.Margin.Left);
            playerList[thisPlayerNum].SetYMove(YLoc - image_CardStack.Margin.Top);

            // set label content to "player #"
            SetLabelContent(label_Right, "Player " + (thisPlayerNum + 1));

            thisPlayerNum++;

            if (thisPlayerNum == 5)
                thisPlayerNum = 0;

            // label_Left:
            // calculate move distances for dealing cards
            playerList[thisPlayerNum].SetPlayerNumLabel(label_Left);
            playerList[thisPlayerNum].SetPlayerMoneyLabel(label_LeftMoney);
            playerList[thisPlayerNum].SetPlayerBetLabel(label_LeftBet);

            XLoc = label_Left.Margin.Left;
            YLoc = label_Left.Margin.Top - image_CardStack.Height;

            playerList[thisPlayerNum].SetXMove(XLoc - image_CardStack.Margin.Left);
            playerList[thisPlayerNum].SetYMove(YLoc - image_CardStack.Margin.Top);

            // set label content to "player #"
            SetLabelContent(label_Left, "Player " + (thisPlayerNum + 1));

            thisPlayerNum++;

            if (thisPlayerNum == 5)
                thisPlayerNum = 0;

            // label_BotLeft:
            // label_Right
            // calculate move distances for dealing cards
            playerList[thisPlayerNum].SetPlayerNumLabel(label_BotLeft);
            playerList[thisPlayerNum].SetPlayerMoneyLabel(label_BotLeftMoney);
            playerList[thisPlayerNum].SetPlayerBetLabel(label_BotLeftBet);

            XLoc = label_BotLeft.Margin.Left;
            YLoc = label_BotLeft.Margin.Top - image_CardStack.Height;

            playerList[thisPlayerNum].SetXMove(XLoc - image_CardStack.Margin.Left);
            playerList[thisPlayerNum].SetYMove(YLoc - image_CardStack.Margin.Top);

            // set label content to "player #"
            SetLabelContent(label_BotLeft, "Player " + (thisPlayerNum + 1));
        }

        // Function: DealCard
        // given a card and a player number, shows the card dealing animation
        private void DealCard (Card aCard, int playerNum)
        {
            // get access to canvas
            if (!MyCanvas.Dispatcher.CheckAccess())
            {
                MyCanvas.Dispatcher.BeginInvoke(new DealCardCallback(DealCard), aCard, playerNum);
                return;
            }

            // create bitmap of back of card
            BitmapImage src = new BitmapImage();
            src.BeginInit();
            src.UriSource = new Uri(@"pack://application:,,,/CardImages/back.png", UriKind.Absolute);
            src.EndInit();

            // create animation card image
            Image animationCard = new Image();

            // set dimensions
            animationCard.Height = 100;
            animationCard.Width = 70;

            // set location
            animationCard.SetValue(Canvas.TopProperty, 20.0);
            animationCard.SetValue(Canvas.LeftProperty, 200.0);

            // set to card back image
            animationCard.Source = src;

            // add to canvas (display it)
            MyCanvas.Children.Add(animationCard);
            
            // create animations
            RotateTransform rotateTrans = new RotateTransform();
            DoubleAnimation rotateAnimation = new DoubleAnimation(720, TimeSpan.FromSeconds(1));

            // create animation completed function
            rotateAnimation.Completed += new EventHandler((sender1, e1) => {
                BitmapImage src1 = new BitmapImage();
                src1.BeginInit();
                src1.UriSource = GetCardURI(aCard);
                src1.EndInit();
                animationCard.Source = src1;

                // if for dealer
                if (playerNum < 0)
                    dealerCards.Add(animationCard);
                // if for player
                else
                    playerList[playerNum].AddCard(animationCard);

                ready = true;
            });//Lamda function

            // calculate card's center
            rotateTrans.CenterX = animationCard.Width / 2;
            rotateTrans.CenterY = animationCard.Height / 2;

            // create transforms
            TranslateTransform xTranslateTransform = new TranslateTransform();
            TranslateTransform yTranslateTransform = new TranslateTransform();

            // create animations
            DoubleAnimation xTranslateAnimation = new DoubleAnimation();
            DoubleAnimation yTranslateAnimation = new DoubleAnimation();

            // if for dealer
            if (playerNum < 0)
            {
                xTranslateAnimation = new DoubleAnimation(dealerXMove + dealerCards.Count * 15, TimeSpan.FromSeconds(1));
                yTranslateAnimation = new DoubleAnimation(dealerYMove, TimeSpan.FromSeconds(1));
            }
            // if for player
            else
            {
                xTranslateAnimation = new DoubleAnimation(playerList[playerNum].GetXMove(), TimeSpan.FromSeconds(1));
                yTranslateAnimation = new DoubleAnimation(playerList[playerNum].GetYMove(), TimeSpan.FromSeconds(1));
            }

            // create transform group
            TransformGroup transGroup = new TransformGroup();
            transGroup.Children.Add(rotateTrans);
            transGroup.Children.Add(xTranslateTransform);
            transGroup.Children.Add(yTranslateTransform);

            // add transform to animation card
            animationCard.RenderTransform = transGroup;

            // do the animation
            rotateTrans.BeginAnimation(RotateTransform.AngleProperty, rotateAnimation);
            xTranslateTransform.BeginAnimation(TranslateTransform.XProperty, xTranslateAnimation);
            yTranslateTransform.BeginAnimation(TranslateTransform.YProperty, yTranslateAnimation);
        }

        // Function: SetPlayerMoney
        // given a player number and a amount of money, sets that player's money label
        private void SetPlayerMoney(int playerNum, int money)
        {
            SetLabelContent(playerList[playerNum].GetMoneyLabel(), "$" + money);
        }

        // Function: SetPlayerBet
        // given a player number and a bet amount, sets that player's money label
        private void SetPlayerBet (int playerNum, int bet)
        {
           SetLabelContent(playerList[playerNum].GetBetLabel(), "Bet: $" + bet);
        }

        // Function: GetCardURI
        // given a card, returns a uri to the image of that card
        private Uri GetCardURI(Card aCard)
        {
            string symbolString = "";
            string suitString = "";

            int cardValue;

            // get symbol
            if (int.TryParse(aCard.GetSymbol() + "", out cardValue))
                symbolString = cardValue.ToString();
            else
            {
                switch (aCard.GetSymbol())
                {
                    case 'A':
                        symbolString = "ace";
                        break;
                    case 'J':
                        symbolString = "jack";
                        break;
                    case 'Q':
                        symbolString = "queen";
                        break;
                    case 'K':
                        symbolString = "king";
                        break;
                }
            }

            // get suit
            switch (aCard.GetSuit())
            {
                case 'H':
                    suitString = "hearts";
                    break;
                case 'C':
                    suitString = "clubs";
                    break;
                case 'S':
                    suitString = "spades";
                    break;
                case 'D':
                    suitString = "diamonds";
                    break;
            }

            // return uri
            return new Uri(@"pack://application:,,,/CardImages/" + symbolString + "_of_" + suitString + ".png", UriKind.Absolute);
        }

        // Function: SetLabelContent
        // given a label and a string, sets that labels content to the string
        private void SetLabelContent(Label aLabel, string content)
        {
            // get access to label
            if (!aLabel.Dispatcher.CheckAccess())
            {
                aLabel.Dispatcher.BeginInvoke(new SetLabelCallback(SetLabelContent), aLabel, content);
                return;
            }

            // set content
            aLabel.Content = content;
        }

        // Function: backgroundWorker_DoWork
        // runs while client is connected to server
        // gets data from server 
        private void backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            // loop while connected
            while (!backgroundWorker.CancellationPending)
            {
                string inputStream;

                try
                {
                    // wait for last animation to finish
                    while (!ready) ;

                    // get message from the server (blocking function)
                    inputStream = sr.ReadLine();  //Note Read only reads into a byte array.  Also Note that Read is a "Blocking Function"

                    // check if the message was "disconnect"
                    if (inputStream == "disconnect")
                    {
                        // send disconnect message
                        sw.WriteLine("disconnect");
                        sw.Flush();
                        Disconnect();
                        return;
                    }
                }
                catch
                {
                    if (backgroundWorker.CancellationPending)
                        return;

                    // if no longer connected, then disconnect
                    Disconnect();
                    return;
                }

                // server asked for player's bet
                if (inputStream == "getbet")
                {
                    // enable the bet button
                    ButtonChanger(button_Bet, "enable");
                    continue;
                }
                // server asked for player to hit or stand
                else if (inputStream == "hitorstand")
                {
                    // enable the hit and stand buttons
                    ButtonChanger(button_Hit, "enable");
                    ButtonChanger(button_Stand, "enable");
                    continue;
                }
                else if (inputStream == "roundover")
                {
                    // show messagebox about this player's winning or losing
                    MessageBox.Show(overMessage, "Round Over", MessageBoxButton.OK);
                    continue;
                }

                else if (inputStream == "clearcards")
                {
                    // clear cards
                    ClearCards();

                    continue;
                }

                else if (inputStream == "outofmoney")
                {
                    MessageBox.Show("You're out of money!", "You lost.", MessageBoxButton.OK);
                    Disconnect();
                    
                    return;
                }

                // otherwise, split the message
                string[] message = inputStream.Split(':');

                // get the player's number
                int playerNum;

                if (message[0] == "dealer")
                    playerNum = -100;
                else if (!int.TryParse(message[0], out playerNum))
                    throw new Exception("expected a player number (or dealer) at the begining of message");
                
                // player placed a bet
                if (message[1] == "bet")
                {
                    // set player's bet
                    SetPlayerBet(playerNum, int.Parse(message[2]));
                }
                // player recived a card
                else if (message[1] == "card")
                {
                    ready = false;
                    // deal the card to the player
                    DealCard(new Card(message[2].ToCharArray()[0], message[3].ToCharArray()[0]), playerNum);
                }
                // recieved player's amount of money
                else if (message[1] == "money")
                {
                    // give them money
                    SetPlayerMoney(playerNum, int.Parse(message[2]));

                    // if it's this client's number, then save the money
                    if (playerNum == myPlayerNum)
                    {
                        myMoney = int.Parse(message[2]);
                    }
                }
                // if a player got a blackjack
                else if (message[1] == "blackjack")
                {
                    if (playerNum == myPlayerNum)
                        overMessage = "You won with a BlackJack!";
                }
                else if (message[1] == "won")
                {
                    if (playerNum == myPlayerNum)
                        overMessage = "You beat the dealer!";
                }
                else if (message[1] == "push")
                {
                    if (playerNum == myPlayerNum)
                        overMessage = "You pushed!";
                }
                else if (message[1] == "lost")
                {
                    if (playerNum == myPlayerNum)
                        overMessage = "You lost!";
                }
                // otherwise, recieved unhandled message
                else
                    throw new Exception("unhandled message recieved");
            }
        }

        // Function: ClearCards
        // removes all cards from player's hands
        private void ClearCards()
        {
            // check for access
            if (!MyCanvas.Dispatcher.CheckAccess())
            {
                MyCanvas.Dispatcher.BeginInvoke(new ClearImagesCallBack(ClearCards));
                return;
            }

            // remove dealer cards
            foreach (Image card in dealerCards)
                MyCanvas.Children.Remove(card);

            dealerCards.Clear();

            // remove player cards
            foreach (Player aPlayer in playerList)
                foreach (Image card in aPlayer.GetCards())
                    MyCanvas.Children.Remove(card);

            foreach (Player aPlayer in playerList)
            {
                aPlayer.ClearCards();
            }
        }

        // Function: ButtonChanger
        // given a button and a function string, enables or disables the button
        private void ButtonChanger(Button theButton, string func)
        {
            // get access to the button
            if (!theButton.Dispatcher.CheckAccess())
            {
                theButton.Dispatcher.BeginInvoke(new ButtonChangerCallBack(ButtonChanger), theButton, func);
                return;
            }

            if (func == "enable")
                theButton.IsEnabled = true;
            else if (func == "disable")
                theButton.IsEnabled = false;
        }

        // Function: BetBoxChanger
        // enables or disables the bet textbox
        private void BetBoxChanger(string func)
        {
            if (!textBox_Bet.Dispatcher.CheckAccess())
            {
                textBox_Bet.Dispatcher.BeginInvoke(new BetBoxChangerCallBack(BetBoxChanger), func);
                return;
            }

            if (func == "enable")
                textBox_Bet.IsEnabled = true;
            else
                textBox_Bet.IsEnabled = false;
        }

        // Function: Disconnect
        // disconnects from the server
        private void Disconnect()
        {
            // get access to connect button
            if (button_Connect.Dispatcher.CheckAccess())
            {
                button_Connect.IsEnabled = true;

                backgroundWorker.CancelAsync();

                sr.Close();
                sw.Close();
                ns.Close();

                connected = false;

                MessageBox.Show("Disconnected from server.", "Disconnected", MessageBoxButton.OK);
            }
            else
                button_Connect.Dispatcher.BeginInvoke(new EnableButtonCallback(Disconnect));
        }

        // Function: Window_Closing
        // runs when user closes window
        // properly disconnects client from server
        private void Window_Closing(object sender, CancelEventArgs e)
        {
            if (connected)
            {
                sw.WriteLine("disconnect");
                sw.Flush();

                Disconnect();
            }
        }

        // Function: button_Bet_Click
        // handles click event of bet button
        // places the bet, if it is valid
        private void button_Bet_Click(object sender, RoutedEventArgs e)
        {
            int bet;

            ButtonChanger(button_Bet, "disable");

            if (!int.TryParse(textBox_Bet.Text, out bet))
            {
                MessageBox.Show("Invalid bet! Try again.", "Invalid Bet", MessageBoxButton.OK);
                ButtonChanger(button_Bet, "enable");
                return;
            }

            if (bet < 5)
            {
                MessageBox.Show("Invalid bet! Must be at least 5.", "Invalid Bet", MessageBoxButton.OK);
                ButtonChanger(button_Bet, "enable");
                return;
            }

            if (bet > myMoney)
            {
                MessageBox.Show("Invalid bet! Must be less than your current money.", "Invalid Bet", MessageBoxButton.OK);
                ButtonChanger(button_Bet, "enable");
                return;
            }   

            sw.WriteLine("bet:" + textBox_Bet.Text);
            sw.Flush();
        }

        // Function: button_Hit_Click
        // handles click event of hit button
        // sends hit message to the server
        private void button_Hit_Click(object sender, RoutedEventArgs e)
        {
            ButtonChanger(button_Hit, "disable");
            ButtonChanger(button_Stand, "disable");
            sw.WriteLine("hit");
            sw.Flush();
        }

        // Function: button_Stand_Click
        // handles click event for stand button
        // sends stand message to the server
        private void button_Stand_Click(object sender, RoutedEventArgs e)
        {
            ButtonChanger(button_Hit, "disable");
            ButtonChanger(button_Stand, "disable");
            sw.WriteLine("stand");
            sw.Flush();
        }
    }
}