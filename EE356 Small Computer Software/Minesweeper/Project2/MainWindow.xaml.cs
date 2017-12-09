// Clayton Brutus
// Project 2 - Minesweeper
// EE356
// 9/19/16

using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Project2
{
    /// Interaction logic for MainWindow.xaml
    public partial class MainWindow : Window
    {
        // Create game timer
        System.Windows.Threading.DispatcherTimer gameTimer;
        // Create 9 x 9 minesweeper grid
        SweeperGrid theGrid = new SweeperGrid();

        public MainWindow()
        {
            // Initialize window
            InitializeComponent();
            // Create 9 x 9 array of buttons
            CreateButtons(9, 9);
            // Set up game timer
            gameTimer = new System.Windows.Threading.DispatcherTimer();
            gameTimer.Interval = new System.TimeSpan(0, 0, 0, 1, 0); //1 sec
            gameTimer.Tick += GameTimer_Tick;

            // Initialize score and time to 0
            txtBox_Time.Text = "0";
            txtBox_Score.Text = "0";
        }

        // Function: GameTimer_Tick
        // Runs every second when timer is running
        private void GameTimer_Tick(object sender, EventArgs e)
        {
            // get current time
            int curr = 0;
            int.TryParse(txtBox_Time.Text, out curr);
            // add 1 to current time and display it
            txtBox_Time.Text = (curr + 1).ToString();
        }

        // Function: CreateButtons
        // input: dimensions of button array to create
        // creates button array
        private void CreateButtons(int numCols, int numRows)
        {
            // clear the grid
            grd1.Children.Clear();
            grd1.RowDefinitions.Clear();
            grd1.ColumnDefinitions.Clear();

            // create list to store buttons
            List<Button> btnList = new List<Button>();

            // set dimensions of grid
            grd1.Height = (numRows + 1) * 20;
            grd1.Width = (numCols + 1) * 20;

            // resize window to fit grid
            this.Width = (5 + numCols) * 20;
            this.Height = 70 + (4 + numRows) * 20;

            // create the buttons
            for (int r = 0; r < numRows; r++)
                for (int c = 0; c < numCols; c++)
                {
                    Button btn = new Button();
                    btn.VerticalAlignment = VerticalAlignment.Top;
                    btn.HorizontalAlignment = HorizontalAlignment.Left;
                    btn.Content = "";
                    btn.Width = 20;
                    btn.Height = 20;
                    btn.Margin = new Thickness(10 + 20 * c, 20 * r, 0, 0);
                    btn.Name = "btn" + c.ToString() + "x" + r.ToString();
                    btn.Background = Brushes.DarkGray; 
                    btn.Click += new RoutedEventHandler(button_Click);
                    btn.MouseRightButtonDown += new System.Windows.Input.MouseButtonEventHandler(button_RightClick);
                    btnList.Add(btn);
                    grd1.Children.Add(btn);
                }
            // put the list of buttons into the minesweeper grid
            theGrid.btnLst = btnList;
        }
        // Function: button_RightClick
        // runs when a minesweeper cell is right clicked
        private void button_RightClick(object sender, RoutedEventArgs e)
        {
            // get the button name
            object a = e.Source;
            Button btn = (Button)a;

            // extract the button location from the name
            string[] location = btn.Name.Replace("btn", "").Split('x');
            int c, r;
            int.TryParse(location[0], out c);
            int.TryParse(location[1], out r);

            // mark the cell
            theGrid.Mark(c, r);

            // update the GUI
            UpdateGui();
        }
        // Function: button_Click
        // runs when a minesweeper cell is left clicked
        private void button_Click(object sender, RoutedEventArgs e)
        {
            // check that game isn't already ended
            if (!theGrid.GameOver())
            {
                // get button name
                object a = e.Source;
                Button btn = (Button)a;

                // extract button location from name
                string[] location = btn.Name.Replace("btn", "").Split('x');
                int c, r;
                int.TryParse(location[0], out c);
                int.TryParse(location[1], out r);

                // start the timer if it's the first click
                if (theGrid.GetScore() == 0)
                    gameTimer.Start();

                // uncover the cell and check if uncovered a bomb
                if (theGrid.Uncover(c, r))
                {
                    // stop the timer
                    gameTimer.Stop();
                    // uncover all cells
                    theGrid.UncoverAll();
                    // update GUI
                    UpdateGui();
                    // display gameover message
                    MessageBox.Show("Game Over! Click the Restart button to play again.");
                }
                // check if didn't uncover bomb
                else
                {
                    // update the GUI
                    UpdateGui();
                    // display message if they won
                    if (theGrid.GameOver())
                        MessageBox.Show("Congratulations, you won!! Click the Restart button to play again.");
                }
            }
            // if game is already over, then display message saying so
            else
                MessageBox.Show("This game is over! Click the Restart button to play again.");
        }
        
        // Function: UpdateGui
        // updates the color and content of the buttons to match the internal minesweeper grid
        private void UpdateGui()
        {
            for (int r = 0; r < theGrid.numRows; r++)
                for (int c = 0; c < theGrid.numCols; c++)
                {
                    // becomes red if contains an uncovered bomb
                    if (theGrid.GetRep(c, r) == '*')
                        theGrid.btnLst[r * theGrid.numCols + c].Background = Brushes.Red;
                    // becomes light gray if is uncovered empty cell
                    else if (!theGrid.grid[c, r].IsCovered())
                        theGrid.btnLst[r * theGrid.numCols + c].Background = Brushes.LightGray;
                    // becomes yellow if marked
                    else if (theGrid.grid[c, r].IsMarked())
                        theGrid.btnLst[r * theGrid.numCols + c].Background = Brushes.LightYellow;
                    // becomes dark gray otherwise
                    else
                        theGrid.btnLst[r * theGrid.numCols + c].Background = Brushes.DarkGray;
                    // set the content of the button
                    theGrid.btnLst[r * theGrid.numCols + c].Content = theGrid.GetRep(c, r);
                }

            txtBox_Score.Text = theGrid.GetScore().ToString();
        }

        // Function: but_Restart_Click
        // runs when the restart button is clicked
        private void but_Restart_Click(object sender, RoutedEventArgs e)
        {
            // stop the timer
            gameTimer.Stop();

            // if intermediate is checked, then make a 16 x 16 grid
            if (rdo_Interm.IsChecked ?? false)
            {
                theGrid = new SweeperGrid(16, 16);
                CreateButtons(16, 16);
            }
            // if hard is checked, then make a 20 x 24 grid
            else if (rdo_Hard.IsChecked ?? false)
            {
                theGrid = new SweeperGrid(20, 24);
                CreateButtons(20, 24);
            }
            // otherwise, make a 9 x 9 grid
            else
            {
                theGrid = new SweeperGrid();
                CreateButtons(9, 9);
            }
            // reset the timer
            txtBox_Time.Text = "0";
            // update the Gui
            UpdateGui();
        }

        private void but_About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Title: Minesweeper\nAuthor: Clayton Brutus\nClass: EE356\nDate: 9/18/2016");
        }

        private void but_Help_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Some randomly selected squares contain mines.\nThe game is played by clicking cells to uncover them.\nThe number that appears in cells indicates the number of adjacent cells that contain bombs.\nYou lose the game if you uncover a mine, and win if you uncover all cells that don't contain a mine.\n\nScore represents the number of cells you have uncovered, and Time is the time in seconds since you started the game.");
        }
    }

    // Class: SweeperGrid
    // internal representation of Minesweeper game
    class SweeperGrid
    {
        public List<Button> btnLst;
        public int numRows, numCols;
        public int numBombs;
        private int score;
        private int currBombs;
        private bool gameOver;
        public Cell[,] grid;

        // Function: SweeeperGrid
        // constructor with default values
        public SweeperGrid(int initCols = 9, int initRows = 9, int initBombs = 10)
        {
            // set internal variables
            numRows = initRows;
            numCols = initCols;
            currBombs = 0;
            score = 0;
            gameOver = false;

            if (initBombs != 10)
                numBombs = initBombs;
            else if (numCols == 9)
                numBombs = 10;
            else if (numCols == 16)
                numBombs = 40;
            else if (numCols == 20)
                numBombs = 99;
            else
                numBombs = 10;

            // create array of Cells
            grid = new Cell[numCols, numRows];

            // initialize each Cell
            for (int r = 0; r < numRows; r++)
                for (int c = 0; c < numCols; c++)
                    grid[c, r] = new Cell();
        }

        // Function: GameOver
        // returns bool indicating if game is over or not
        public bool GameOver()
        {
            return gameOver;
        }

        // Function: GetRep
        // input: location of cell
        // returns the character represention of the cell
        public char GetRep(int c, int r)
        {
            return grid[c, r].GetRep();
        }

        // Function: GetScore
        // returns the number of user uncovered cells
        public int GetScore()
        {
            return score;
        }

        // Function: Mark
        // input: location of cell
        // toggles mark/unmarking of cell
        public void Mark(int c, int r)
        {
            if (grid[c, r].IsMarked())
                grid[c, r].UnMark();
            else
                grid[c, r].Mark();
        }

        // Function: Uncover
        // input: location of cell
        // recursively opens the cell and neighbors
        // returns true if contains bomb, false otherwise
        public bool Uncover(int c, int r)
        {
            // if already uncovered, then do nothing
            if (!grid[c, r].IsCovered())
                return false;

            // if first click, then place the bombs first
            if (currBombs == 0)
            {
                FirstClick(c, r);
                RecursiveUncover(c, r);
            }
            // if doesn't have bomb, then recursively uncover neighbors
            else if (!grid[c, r].HasBomb())
                RecursiveUncover(c, r);
            // check if game has been won
            if (score == numCols * numRows - numBombs)
                gameOver = true;

            // return true for bomb, false for no bomb
            return grid[c, r].HasBomb();
        }

        // Function: UncoverAll
        // Uncovers all cells without altering score
        public void UncoverAll()
        {
            // end the game
            gameOver = true;

            for (int r = 0; r < numRows; r++)
                for (int c = 0; c < numCols; c++)
                    grid[c, r].Uncover();
        }

        // Function: RecursiveUncover
        // input: location of a cell
        // recursively uncovers the cells in the area until it gets to cells that have adjacent bombs
        private void RecursiveUncover(int c, int r)
        {
            // add 1 to the score
            score++;

            // uncover the current cell
            grid[c, r].Uncover();

            // stop if the current cell has adjacent bombs
            if (grid[c, r].GetNumAdj() > 0)
                return;

            // recurse:
            // up
            if(InBounds(c,r-1))
                if(grid[c, r-1].IsCovered())
                    RecursiveUncover(c, r - 1);
            // top right
            if (InBounds(c + 1, r - 1))
                if (grid[c + 1, r - 1].IsCovered())
                    RecursiveUncover(c + 1, r - 1);
            // right
            if (InBounds(c + 1, r))
                if (grid[c + 1, r].IsCovered())
                    RecursiveUncover(c + 1, r);
            // bot right
            if (InBounds(c + 1, r + 1))
                if (grid[c + 1, r + 1].IsCovered())
                    RecursiveUncover(c + 1, r + 1);
            // down
            if (InBounds(c, r + 1))
                if (grid[c, r + 1].IsCovered())
                    RecursiveUncover(c, r + 1);
            // bot left
            if (InBounds(c - 1, r + 1))
                if (grid[c - 1, r + 1].IsCovered())
                    RecursiveUncover(c - 1, r + 1);
            // left
            if (InBounds(c - 1, r))
                if (grid[c - 1, r].IsCovered())
                    RecursiveUncover(c - 1, r);
            // top left
            if (InBounds(c - 1, r - 1))
                if (grid[c - 1, r - 1].IsCovered())
                    RecursiveUncover(c - 1, r - 1); 
        }

        // Function: InBounds
        // input: location of cell
        // returns true if it is a valid location
        private bool InBounds(int c, int r)
        {
            if (c < 0 || c >= numCols || r < 0 || r >= numRows)
                return false;
            return true;
        }

        // Function: FirstClick
        // input: location of cell that has been clicked
        // randomly places bombs everywhere exept the cliced and adjacent cells
        private void FirstClick(int c, int r)
        {
            // set up random number generator
            Random rand = new Random();
            int randR, randC;

            // loop while it hasn't placed enough bombs
            while (currBombs < numBombs)
            {
                // generate a random location on the grid
                randR = rand.Next(0, numRows);
                randC = rand.Next(0, numCols);

                // check that location is not in area of clicked cell
                if (randC != c || randR != r)           // center
                    if (randC != c || randR != r - 1)       // up
                        if (randC != c || randR != r + 1)       // down
                            if (randC != c - 1 || randR != r)       // left
                                if (randC != c + 1 || randR != r)       // right
                                    if (randC != c - 1 || randR != r - 1)   // top left
                                        if (randC != c + 1 || randR != r - 1)   // top right
                                            if (randC != c - 1 || randR != r + 1)   // bot left
                                                if (randC != c + 1 || randR != r + 1)   // bot right
                                                    if (!grid[randC, randR].HasBomb())
                                                    {
                                                        // place a bomb
                                                        grid[randC, randR].PlaceBomb();
                                                        // add 1 to curr bombs
                                                        currBombs++;
                                                        
                                                        // increment the number of adjacent bombs for the adjacent cells
                                                        // up
                                                        if (InBounds(0, randR - 1))
                                                            grid[randC, randR - 1].IncNumAdj();
                                                        // down
                                                        if (InBounds(0, randR + 1))
                                                            grid[randC, randR + 1].IncNumAdj();
                                                        // right
                                                        if (InBounds(randC + 1, 0))
                                                            grid[randC + 1, randR].IncNumAdj();
                                                        // left
                                                        if (InBounds(randC - 1, 0))
                                                            grid[randC - 1, randR].IncNumAdj();
                                                        // top left
                                                        if (InBounds(randC - 1, randR - 1))
                                                            grid[randC - 1, randR - 1].IncNumAdj();
                                                        // top right
                                                        if (InBounds(randC + 1, randR -1))
                                                            grid[randC + 1, randR - 1].IncNumAdj();
                                                        // bot left
                                                        if (InBounds(randC - 1, randR + 1))
                                                            grid[randC - 1, randR + 1].IncNumAdj();
                                                        // bot right
                                                        if (InBounds(randC + 1, randR + 1))
                                                            grid[randC + 1, randR + 1].IncNumAdj();
                                                    }
            }
        }
    }
    // Class: Cell
    // represents 1 cell of the minesweeper grid
    class Cell
    {
        private bool covered;
        private bool bomb;
        private bool marked;
        private char rep;
        private int numAdj;

        // default constructor
        public Cell()
        {
            // make a cell that's covered and empty
            covered = true;
            bomb = false;
            marked = false;
            numAdj = 0;
            rep = ' ';
        }

        // Function: HasBomb
        // returns true if cell contains a bomb
        public bool HasBomb()
        {
            return bomb;
        }

        // Function: IsCovered
        // returns true if cell is covered
        public bool IsCovered()
        {
            return covered;
        }

        // Function: IsMarked
        // returns true if cell is marked
        public bool IsMarked()
        {
            return marked;
        }

        // Function: GetNumAdj
        // returns the number of adjacent cells that contain bombs
        public int GetNumAdj()
        {
            return numAdj;
        }

        // Function: IncNumAdj
        // adds 1 to the number of adjacent cells that contain bombs
        public void IncNumAdj()
        {
            numAdj++;
            if (!bomb && !covered)
                rep = (char)('0' + numAdj);
        }

        // Function: PlaceBomb
        // places a bomb in the cell
        public void PlaceBomb()
        {
            bomb = true;
            if (!covered)
                rep = '*';
        }

        // Function: Uncover
        // uncovers the cell and returns true if it contains a bomb
        public bool Uncover()
        {
            covered = false;
            if (bomb)
                rep = '*';
            else
                rep = (char)('0' + numAdj);

            return bomb;
        }

        // Function: Mark
        // marks the cell
        public void Mark()
        {
            marked = true;
            if (covered)  // covered, so show the mark
                rep = '!';
        }

        // Function: UnMark
        // unmarks the cell
        public void UnMark()
        {
            marked = false;
            if (covered)  // covered, so remove the mark
                rep = ' ';
        }

        // Function: GetRep
        // returns the character represention of the cell
        public char GetRep()
        {
            if (rep == '0')
                return ' ';
            return rep;
        }
    }
}