using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

    public int GetValue()
    {
        if (!hasValue)
            throw new Exception("cannot get value of card that hasn't been initialized");

        int value = 0;

        if (int.TryParse(symbol + "", out value))
            return value;

        if (symbol == 'J' || symbol == 'Q' || symbol == 'K')
            return 10;
        else
            return 11;
    }
}

class CardStack
{
    private List<Card> cards;

    int numDecks;

    public CardStack()
    {
        numDecks = 0;
        cards = new List<Card>();
    }

    public CardStack(int initDecks)
    {
        numDecks = initDecks;

        cards = new List<Card>();

        // for each of the decks
        for (int j = 0; j < numDecks; j++)
        {
            // for each of the 4 suits
            foreach (char suit in new List<char>(new char[] { 'H', 'C', 'S', 'D' }))
            {
                // for each card in 1 - 9
                for (int i = 1; i < 9; i++)
                {
                    cards.Add(new Card((char)('1' + i), suit));
                }

                // for J, Q, K, A
                cards.Add(new Card('J', suit));
                cards.Add(new Card('Q', suit));
                cards.Add(new Card('K', suit));
                cards.Add(new Card('A', suit));
            }
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

    public void Add(Card aCard)
    {
        cards.Add(aCard);
    }

    public Card DrawCard()
    {
        Card toDraw = cards.First();
        cards.Remove(toDraw);

        return toDraw;
    }

    public void Reset()
    {
        CardStack newStack;

        if (numDecks == 0)
            cards = new List<Card>();
        else
            newStack = new CardStack(numDecks);
    }

    public int Count()
    {
        return cards.Count();
    }

    public int GetPoints()
    {
        int total = 0;
        int numAces = 0;

        foreach (Card aCard in cards)
        {
            // add card value
            total += aCard.GetValue();

            if (aCard.GetSymbol() == 'A')
                numAces++;
        }

        while (numAces > 0 && total > 21)
        {
            total -= 10;
            numAces--;
        }

        return total;
    }

    public void Clear()
    {
        cards.Clear();
    }

    public List<Card> GetCards()
    {
        return cards;
    }
}
