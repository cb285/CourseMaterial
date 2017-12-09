using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Server
{
    class Card
    {
        private char symbol;
        private char suit;
        private bool hasValue;

        public Card()
        {
            symbol = '0';
            suit = '0';
            hasValue = false;
        }

        public Card(char initSymbol, char initSuite)
        {
            symbol = initSymbol;

            if (initSuite == 'H' || initSuite == 'C' || initSuite == 'S' || initSuite == 'D')
                suit = initSuite;
            else
                throw new Exception("suite must be either 'H', 'C', 'S', or 'D'");

            hasValue = true;
        }

        public char GetSymbol()
        {
            if (hasValue)
                return symbol;
            else
                throw new Exception("cannot get symbol of a card that has not been assigned a value yet");
        }

        public char GetSuit()
        {
            if (hasValue)
                return suit;
            else
                throw new Exception("cannot get suite of a card that has not been assigned a value yet");
        }
    }

    class Deck
    {
        private List<Card> cards;

        public Deck()
        {
            cards = new List<Card>();

            // for each of the 4 suits
            foreach (char suit in new List<char>(new char[] { 'H', 'C', 'S', 'D' }))
            {
                // for each card in 1 - 9
                for (int i = 0; i < 9; i++)
                {
                    cards.Add(new Card((char)('1'+i), suit));
                }

                // for J, Q, K, A
                cards.Add(new Card('J', suit));
                cards.Add(new Card('Q', suit));
                cards.Add(new Card('K', suit));
                cards.Add(new Card('A', suit));
            }
        }

        public void Shuffle()
        {
            Random rnd = new Random();

            int n = cards.Count;

            while (n > 1)
            {
                n--;
                int k = rnd.Next(n + 1);
                Card aCard = cards[k];
                cards[k] = cards[n];
                cards[n] = aCard;
            }
        }

        public Card DrawCard()
        {
            Card toDraw = cards.First();
            cards.Remove(toDraw);

            return toDraw;
        }
    }
}
