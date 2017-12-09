using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;

namespace BlackJackServer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class ServerWindow : Window
    {
        // create a server with a max of 5 clients
        Server theServer = new Server(5);

        BackgroundWorker bkw = new BackgroundWorker();

        // create list for storing current players
        List<Player> playerList = new List<Player>();

        // create shoe of cards
        CardStack shoe;

        // create delegates for updating GUI
        delegate void PlayerStatCallBack(int clientNum, string stat);
        delegate void AddCardToListCallBack(ListBox aListbox, Card aCard);
        delegate void CardsLeftCallBack(int cardsLeft);
        delegate void PlayerMoneyCallBack(Player aPlayer, int money);
        delegate void SetLabelCallback(Label aLabel, string content);
        delegate void ClearListBoxCallBack(ListBox aListBox);

        // start window
        public ServerWindow()
        {
            InitializeComponent();

            // add new client handler function
            theServer.ClientConnected += new EventHandler<MyEventArgs>(NewClient);

            // setup background worker
            bkw.DoWork += new DoWorkEventHandler(bkw_DoWork);
        }

        // Function: button_Start_Click
        // event handler for clicking the start button
        // starts the server on port 9090
        private void button_Start_Click(object sender, RoutedEventArgs e)
        {
            // disable the start button
            button_Start.IsEnabled = false;

            // start the server
            theServer.Start(9090);
            
            // display a messagebox with IP and port
            MessageBox.Show("Server started on " + theServer.GetIPAddress().ToString() + " port " + theServer.GetPortNumber(), "Server started.", MessageBoxButton.OK);
        }

        // Function: NewClient
        // event handler for a new client connection
        // creates a "player" and adds the player to the list
        // starts the blackjack game if it is the first client
        private void NewClient(object sender, MyEventArgs e)
        {
            // get the client number
            int clientNum;
            int.TryParse(e.MyEventString, out clientNum);

            // get the client from the server
            Client newClient = theServer.GetClient(clientNum);

            // create a new player with $1000
            Player newPlayer = new Player(1000, newClient);

            // add the player to the list
            playerList.Add(newPlayer);

            // update the server GUI with player money amount
            UpdatePlayerMoney(newPlayer, 1000);

            // update GUI player status
            SetPlayerStatus(clientNum, "Connected");

            // if first player, then start the blackjack game
            if (playerList.Count == 1)
                bkw.RunWorkerAsync();
        }

        // Function: UpdatePlayerMoney
        // sets the given player's money amount to the money value given, sends the new amount to the clients, then updates the server GUI
        private void UpdatePlayerMoney(Player aPlayer, int money)
        {
            // update player's server money
            aPlayer.SetMoney(money);

            // send to clients
            theServer.SendAll(aPlayer.GetPlayerNum() + ":money:" + aPlayer.GetMoney());

            // set the proper label's content
            switch (aPlayer.GetPlayerNum())
            {
                case 0:
                    SetLabelContent(label_P1Money, "$" + money);
                    break;
                case 1:
                    SetLabelContent(label_P2Money, "$" + money);
                    break;
                case 2:
                    SetLabelContent(label_P3Money, "$" + money);
                    break;
                case 3:
                    SetLabelContent(label_P4Money, "$" + money);
                    break;
                case 4:
                    SetLabelContent(label_P5Money, "$" + money);
                    break;
            }
        }

        // Function: SetPlayerStatus
        // given a player number and a string, set's that player's status label content to the given string
        private void SetPlayerStatus(int playerNum, string stat)
        {
            switch (playerNum)
            {
                case 0:
                    SetLabelContent(label_P1Stat, stat);
                    break;
                case 1:
                    SetLabelContent(label_P2Stat, stat);
                    break;
                case 2:
                    SetLabelContent(label_P3Stat, stat);
                    break;
                case 3:
                    SetLabelContent(label_P4Stat, stat);
                    break;
                case 4:
                    SetLabelContent(label_P5Stat, stat);
                    break;
            }
        }

        // Function: bkw_DoWork
        // Runs the BlackJack game
        private void bkw_DoWork(object sender, DoWorkEventArgs e)
        {
            // create a shoe of cards containing 7 decks
            shoe = new CardStack(7);

            // shuffle the shoe
            shoe.Shuffle();

            // create dealer's hand
            CardStack dealerCards = new CardStack();

            // loop until cancelled
            while (!bkw.CancellationPending)
            {
                // if shoe contains 52 cards or less, then reset the shoe to 7 decks
                if (shoe.Count() <= 52)
                {
                    // reset (refill with cards)
                    shoe.Reset();
                    // shuffle cards
                    shoe.Shuffle();
                }

                // update server GUI
                UpdateCardsLeft(shoe.Count());

                // create copy of current player list so that it doesn't change during the round
                List<Player> thisRoundPlayers = new List<Player>(playerList);

                foreach (Player serverPlayer in thisRoundPlayers)
                    if (!serverPlayer.isPlaying)
                        playerList.Remove(serverPlayer);

                // if no players, then stop the game
                if (playerList.Count == 0)
                    return;

                // create copy of current player list so that it doesn't change during the round
                thisRoundPlayers = new List<Player>(playerList);

                // get each player's bet
                foreach (Player serverPlayer in thisRoundPlayers)
                {
                    if (serverPlayer.GetMoney() <= 0)
                    {
                        serverPlayer.OutOfMoney();
                        DisconnectPlayer(serverPlayer);
                        continue;
                    }

                    // ask for player's bet
                    serverPlayer.AskForBet();

                    string[] playerMessage;

                    try
                    {
                        // split the message to parse it
                        playerMessage = serverPlayer.Read().Split(":".ToCharArray());
                    }
                    catch
                    {
                        // remove them from the lists
                        if (playerList.Contains(serverPlayer))
                            playerList.Remove(serverPlayer);

                        // mark player as not playing
                        serverPlayer.isPlaying = false;

                        // update status to disconnected
                        SetPlayerStatus(serverPlayer.GetPlayerNum(), "Disconnected");

                        // go to next player
                        continue;
                    }

                    // if disconnected
                    if (playerMessage[0] == "disconnect")
                    {
                        // disconnected them
                        DisconnectPlayer(serverPlayer);

                        // go to next player
                        continue;
                    }

                    // get bet from the player
                    if (playerMessage[0] != "bet")
                        throw new Exception("expected a bet.");

                    int bet;

                    // parse the second part of message for bet amount
                    if (!int.TryParse(playerMessage[1], out bet))
                        throw new Exception("expected a number for the bet.");

                    // clear their hand
                    serverPlayer.ClearCards();

                    try
                    {
                        // place the bet
                        PlayerBet(serverPlayer, bet);
                    }
                    catch
                    {
                        // disconnect them
                        DisconnectPlayer(serverPlayer);

                        // go to next player
                        continue;
                    }
                }

                // give each player first card
                foreach (Player serverPlayer in thisRoundPlayers)
                {
                    // give them the card
                    GivePlayerCard(serverPlayer, shoe.DrawCard());

                    // update server GUI
                    UpdateCardsLeft(shoe.Count());
                }

                // give each player 2nd card and, if they hit, other cards
                foreach (Player serverPlayer in thisRoundPlayers)
                {
                    string playerMessage = "";

                    do
                    {
                        // give them a card
                        GivePlayerCard(serverPlayer, shoe.DrawCard());
                        
                        // update cards left server GUI
                        UpdateCardsLeft(shoe.Count());

                        // stop if busted
                        if (serverPlayer.GetPoints() >= 21)
                            break;

                        // ask for hit or stand
                        serverPlayer.AskHitOrStand();

                        try
                        {
                            // get their response
                            playerMessage = serverPlayer.Read();
                        }
                        catch
                        {
                            // disconnected player
                            DisconnectPlayer(serverPlayer);
                            // go to next player
                            continue;
                        }

                        // while they hit, haven't busted, and have less than 5 cards: keep dealing them cards
                    } while (playerMessage == "hit" && serverPlayer.GetPoints() <= 21 && serverPlayer.GetCards().Count() < 5);

                    // check if they busted
                    if (serverPlayer.GetPoints() > 21)
                        serverPlayer.Bust();

                    // if disconnect message
                    else if (playerMessage == "disconnect")
                        // disconnect them
                        DisconnectPlayer(serverPlayer);

                    // check if they gave a bad response
                    else if (playerMessage != "stand" && playerMessage != "hit" && playerMessage != "" && playerMessage != null)
                        throw new Exception("expected hit, stand, or disconnect response");
                }

                // check if all players busted
                bool allBusted = true;

                foreach (Player serverPlayer in thisRoundPlayers)
                {
                    allBusted = allBusted && serverPlayer.IsBusted();
                }

                // If all players busted, the hand ends automatically without the dealer having to play their hand
                if (allBusted)
                {
                    foreach (Player serverPlayer in thisRoundPlayers)
                        theServer.SendAll(serverPlayer.GetPlayerNum() + ":lost");

                    // clear each player's hand
                    foreach (Player serverPlayer in thisRoundPlayers)
                    {
                        serverPlayer.Clear();
                        ClearCardList(serverPlayer.GetPlayerNum());
                    }

                    // clear dealer's hand
                    dealerCards.Clear();
                    ClearCardList(-100);

                    // end this round
                    theServer.SendAll("roundover");
                    continue;
                }

                // if not all players busted:

                // give dealer first card
                Card drawnCard = shoe.DrawCard();
                dealerCards.Add(drawnCard);
                // update cards left label
                UpdateCardsLeft(shoe.Count());

                // send card to clients
                theServer.SendAll("dealer:card:" + drawnCard.GetSymbol() + ":" + drawnCard.GetSuit());

                // give dealer second card, and hit while points < 17
                do
                {
                    // draw card from the shoe
                    drawnCard = shoe.DrawCard();

                    // add the card to dealer's hand
                    dealerCards.Add(drawnCard);

                    // send the card to clients
                    theServer.SendAll("dealer:card:" + drawnCard.GetSymbol() + ":" + drawnCard.GetSuit());

                    // update server GUI
                    AddCardToList(listBox_Dealer, drawnCard);
                    UpdateCardsLeft(shoe.Count());

                } while (dealerCards.GetPoints() < 17);

                // check if dealer busted
                bool dealerBusted = !(dealerCards.GetPoints() <= 21);

                // check for winners
                foreach(Player serverPlayer in thisRoundPlayers)
                {
                    // if player got blackjack and dealer did not, then 3 to 2 bet return
                    if ((serverPlayer.GetCards().Count == 2) && (serverPlayer.GetPoints() == 21) && ((dealerCards.GetPoints() != 21) || dealerBusted))
                    {
                        // calculate new amount of money
                        int newMoney = serverPlayer.GetMoney() + serverPlayer.GetBet() + (3 / 2) * serverPlayer.GetBet();

                        // set player's bet to 0
                        PlayerBet(serverPlayer, 0);

                        // update gui and send money to clients
                        UpdatePlayerMoney(serverPlayer, newMoney);

                        // send clients message that this player won with a "blackjack"
                        theServer.SendAll(serverPlayer.GetPlayerNum() + ":blackjack");
                    }
                    // if dealer busted and player did not, or neither busted but player got more points, then 1 to 1 bet return
                    else if ((dealerBusted || (serverPlayer.GetPoints() > dealerCards.GetPoints())) && !serverPlayer.IsBusted())
                    {
                        // calculate new amount of money
                        int newMoney = serverPlayer.GetMoney() + 2 * serverPlayer.GetBet();

                        // set player's bet to 0
                        PlayerBet(serverPlayer, 0);

                        // update gui and send money to clients
                        UpdatePlayerMoney(serverPlayer, newMoney);

                        // send clients message that this player won
                        theServer.SendAll(serverPlayer.GetPlayerNum() + ":won");
                    }
                    // if tied (push), then just return bet
                    else if ((dealerCards.GetPoints() <= 21) && (serverPlayer.GetPoints() == dealerCards.GetPoints()))
                    {
                        // calculate new amount of money
                        int newMoney = serverPlayer.GetMoney() + serverPlayer.GetBet();

                        // set player's bet to 0
                        PlayerBet(serverPlayer, 0);

                        // update gui and send money to clients
                        UpdatePlayerMoney(serverPlayer, newMoney);

                        // send clients message that this player pushed
                        theServer.SendAll(serverPlayer.GetPlayerNum() + ":push");
                    }
                    // otherwise, player lost their bet
                    else
                    {
                        // set player's bet to 0
                        PlayerBet(serverPlayer, 0);

                        // send clients message that this player lost
                        theServer.SendAll(serverPlayer.GetPlayerNum() + ":lost");
                    }
                }

                // clear each player's hand
                foreach (Player serverPlayer in thisRoundPlayers)
                {
                    serverPlayer.Clear();
                    ClearCardList(serverPlayer.GetPlayerNum());
                }

                // clear dealer's hand
                dealerCards.Clear();
                ClearCardList(-100);

                // send round over message
                theServer.SendAll("roundover");
            }
        }

        // Function: DisconnectPlayer
        // marks player to be removed later
        // resets player status, money, and bet
        private void DisconnectPlayer(Player serverPlayer)
        {
            // remove them from the lists
            if (playerList.Contains(serverPlayer))
                playerList.Remove(serverPlayer);

            // mark as not playing
            serverPlayer.isPlaying = false;

            // set status to not connected
            SetPlayerStatus(serverPlayer.GetPlayerNum(), "Not Connected");

            // set their bet to 0
            switch (serverPlayer.GetPlayerNum())
            {
                case 0:
                    SetLabelContent(label_Bet1, "Bet: $" + 0);
                    break;
                case 1:
                    SetLabelContent(label_Bet2, "Bet: $" + 0);
                    break;
                case 2:
                    SetLabelContent(label_Bet3, "Bet: $" + 0);
                    break;
                case 3:
                    SetLabelContent(label_Bet4, "Bet: $" + 0);
                    break;
                case 4:
                    SetLabelContent(label_Bet5, "Bet: $" + 0);
                    break;
            }

            // set their money to 0
            switch (serverPlayer.GetPlayerNum())
            {
                case 0:
                    SetLabelContent(label_P1Money, "$" + 0);
                    break;
                case 1:
                    SetLabelContent(label_P2Money, "$" + 0);
                    break;
                case 2:
                    SetLabelContent(label_P3Money, "$" + 0);
                    break;
                case 3:
                    SetLabelContent(label_P4Money, "$" + 0);
                    break;
                case 4:
                    SetLabelContent(label_P5Money, "$" + 0);
                    break;
            }

            // clear their cards
            ClearCardList(serverPlayer.GetPlayerNum());

            // remove from the server
            theServer.RemoveClient(serverPlayer.GetClient());
        }

        // Function: ClearCardList
        // given a playernum, clears their card listBox
        private void ClearCardList(int playerNum)
        {
            switch (playerNum)
            {
                case 0:
                    ClearListBox(listBox_P1);
                    break;
                case 1:
                    ClearListBox(listBox_P2);
                    break;
                case 2:
                    ClearListBox(listBox_P3);
                    break;
                case 3:
                    ClearListBox(listBox_P4);
                    break;
                case 4:
                    ClearListBox(listBox_P5);
                    break;
                default:
                    ClearListBox(listBox_Dealer);
                    break;
            }
        }

        // Function: ClearListBox
        // given a listbox, gets access and clears its contents
        private void ClearListBox(ListBox aListBox)
        {
            if (!aListBox.Dispatcher.CheckAccess())
                aListBox.Dispatcher.BeginInvoke(new ClearListBoxCallBack(ClearListBox), aListBox);
            else
                aListBox.Items.Clear();
        }

        // Function: UpdateCardsLeft
        // given a number of cards left, gets access to the cardsleft label then sets the content of the label to the num of cards left
        private void UpdateCardsLeft(int cardsLeft)
        {
            // check for access
            if (!label_CardsLeft.Dispatcher.CheckAccess())
                // if no, then get access with delegate
                label_CardsLeft.Dispatcher.BeginInvoke(new CardsLeftCallBack(UpdateCardsLeft), cardsLeft);
            else
                // otherwise, set the label content
                label_CardsLeft.Content = cardsLeft + " Cards in stack";
        }

        // Function: GivePlayerCard
        // given a player and a card, sends the card to the player and updates player's card listbox
        private void GivePlayerCard(Player serverPlayer, Card card)
        {
            // give the card to the serverplayer
            serverPlayer.Deal(card);

            // send the clients the card
            theServer.SendAll(serverPlayer.GetPlayerNum() + ":card:" + card.GetSymbol() + ":" + card.GetSuit());

            // add the card to the player's card listbox
            switch (serverPlayer.GetPlayerNum())
            {
                case 0:
                    AddCardToList(listBox_P1, card);
                    break;
                case 1:
                    AddCardToList(listBox_P2, card);
                    break;
                case 2:
                    AddCardToList(listBox_P3, card);
                    break;
                case 3:
                    AddCardToList(listBox_P4, card);
                    break;
                case 4:
                    AddCardToList(listBox_P5, card);
                    break;
            }
        }

        // Function: AddCardToList
        // given a listbox and a card, gets access to the listbox, then adds the card to the listbox
        void AddCardToList(ListBox alistbox, Card aCard)
        {
            // check for access
            if (!alistbox.Dispatcher.CheckAccess())
                // if no, then get access with delegate
                alistbox.Dispatcher.BeginInvoke(new AddCardToListCallBack(AddCardToList), alistbox, aCard);
            else
                // otherwise, add the card
                alistbox.Items.Insert(alistbox.Items.Count, aCard.GetSymbol() + " " + aCard.GetSuit());
        }

        // Function: PlayerBet
        // saves the player's bet, sends the bet amount to all clients, and updates the server GUI
        private void PlayerBet(Player serverPlayer, int bet)
        {
            // get the player number
            int playerNum = serverPlayer.GetPlayerNum();

            // save the player's bet
            serverPlayer.Bet(bet);

            // send the bet to clients
            theServer.SendAll(playerNum + ":bet:" + bet);
            
            // update this player's money amount
            UpdatePlayerMoney(serverPlayer, serverPlayer.GetMoney());

            // set the proper label content to the bet
            switch (playerNum)
            {
                case 0:
                    SetLabelContent(label_Bet1, "Bet: $" + bet);
                    break;
                case 1:
                    SetLabelContent(label_Bet2, "Bet: $" + bet);
                    break;
                case 2:
                    SetLabelContent(label_Bet3, "Bet: $" + bet);
                    break;
                case 3:
                    SetLabelContent(label_Bet4, "Bet: $" + bet);
                    break;
                case 4:
                    SetLabelContent(label_Bet5, "Bet: $" + bet);
                    break;
            }
        }

        // Function: SetLabelContent
        // given a label and a string, gets access to that label and sets its content to the given string
        private void SetLabelContent(Label aLabel, string content)
        {
            // check for access
            if (!aLabel.Dispatcher.CheckAccess())
            {
                // if no, then get access with a delegate
                aLabel.Dispatcher.BeginInvoke(new SetLabelCallback(SetLabelContent), aLabel, content);
                return;
            }

            // otherwise, set the content to the string
            aLabel.Content = content;
        }
    }
}
