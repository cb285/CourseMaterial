using System;
using System.Collections;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Controls;
using Microsoft.SPOT.Presentation.Media;
using Microsoft.SPOT.Presentation.Shapes;
using Microsoft.SPOT.Touch;
using Gadgeteer.Networking;
using GT = Gadgeteer;
using GTM = Gadgeteer.Modules;
using Gadgeteer.Modules.GHIElectronics;

namespace BreakOut
{
    public partial class Program
    {
        // constants for easily changing settings
        const int PADDLE_HEIGHT = 3;
        const int PADDLE_WIDTH = 25;
        const int PADDLE_STEP = 5;
        const int BALL_DIAM = 8;
        const int HEADER_HEIGHT = 12;
        const int WAIT_TIME = 33;
        const int START_SPEED = 4;
        const int SPEEDUP = 3;

        // ball bitmap
        Bitmap Ball = new Bitmap(BALL_DIAM + 2, BALL_DIAM + 2);

        // clear ball bitmap
        Bitmap ClearBall = new Bitmap(BALL_DIAM + 2, BALL_DIAM + 2);

        // paddle bitmap
        Bitmap Paddle = new Bitmap(PADDLE_WIDTH, PADDLE_HEIGHT);

        void ProgramStarted()
        {
            while (true)
            {
                // erase header
                displayN18.Draw(new Bitmap(displayN18.Width, HEADER_HEIGHT), 0, 0);
                
                // erase paddle
                displayN18.Draw(new Bitmap(displayN18.Width, PADDLE_HEIGHT), 0, displayN18.Height - PADDLE_HEIGHT);

                // display spash screen
                Bitmap splash = new Bitmap(displayN18.Width - 10, displayN18.Height / 4);
               // splash.DrawRectangle(Color.White, 1, 0, 0, displayN18.Width / 2, displayN18.Height / 3, 0, 0, Color.White, 0, 0, Color.White, displayN18.Width / 2, displayN18.Height / 3, 0xFF);
                splash.DrawText("BreakOut", Resources.GetFont(Resources.FontResources.harlowsolid), Colors.Blue, 8, 0);

                displayN18.Draw(splash, 10, displayN18.Height / 3);

                // wait 1 second
                Thread.Sleep(1000);

                // wait for user to press joystick
                while (!joystick.IsPressed) ;

                // erase splash screen
                displayN18.Draw(new Bitmap(displayN18.Width - 10, displayN18.Height / 5), 10, displayN18.Height / 3);

                // display P1 message
                Bitmap message = new Bitmap(displayN18.Width / 2, displayN18.Height / 3);
                message.DrawText("P1's turn!", Resources.GetFont(Resources.FontResources.small), Color.White, 0, 0);

                displayN18.Draw(message, displayN18.Width / 3, displayN18.Height / 3);

                // wait 1 second
                Thread.Sleep(1000);

                // wait for user to press joystick
                while (!joystick.IsPressed) ;

                // erase message
                displayN18.Draw(new Bitmap(displayN18.Width / 2, displayN18.Height / 3), displayN18.Width / 3, displayN18.Height / 3);

                // Player 1 game
                int P1Score = PlayGame();

                // wait .5 second
                Thread.Sleep(500);

                // display P2 message
                message = new Bitmap(displayN18.Width / 2, displayN18.Height / 3);
                message.DrawText("P2's turn!", Resources.GetFont(Resources.FontResources.small), Color.White, 0, 0);
                displayN18.Draw(message, displayN18.Width / 3, displayN18.Height / 3);

                // wait 1 second
                Thread.Sleep(1000);

                // wait for user to press joystick
                while (!joystick.IsPressed) ;

                // erase message
                displayN18.Draw(new Bitmap(displayN18.Width / 2, displayN18.Height / 3), displayN18.Width / 3, displayN18.Height / 3);

                // Player 2:
                int P2Score = PlayGame();

                // display scores
                message = new Bitmap(displayN18.Width / 2, displayN18.Height / 3);

                string str = "P1: " + P1Score + " P2: " + P2Score;

                message.DrawText(str, Resources.GetFont(Resources.FontResources.small), Color.White, 0, 0);
                displayN18.Draw(message, displayN18.Width / 3, displayN18.Height / 3);

                // wait 2 seconds
                Thread.Sleep(2000);

                // announce winner
                message = new Bitmap(displayN18.Width / 2, displayN18.Height / 3);

                if (P1Score > P2Score)
                    str = "P1 Won!";
                else if (P2Score > P1Score)
                    str = "P2 Won!";
                else
                    str = "Tie!";

                message.DrawText(str, Resources.GetFont(Resources.FontResources.small), Color.White, 0, 0);
                displayN18.Draw(message, displayN18.Width / 3, displayN18.Height / 3);

                // wait 2 seconds
                Thread.Sleep(2000);
            }
        }

        // Function: PlayGame
        // plays 1 game (3 lives) of breakout and returns the score when game is over
        private int PlayGame()
        {
             // current position for ball
            int BallPositionX = 0;
            int BallPositionY = 0;

            // current position for paddle
            int PaddlePosition = 0;
            // current paddle width
            int PaddleWidth = PADDLE_WIDTH;

            // start with 3 lives
            int lives = 3;

            // draw header
            DrawHeader(0, lives);

            // draw ball bitmap
            Ball.DrawEllipse(Colors.White, BALL_DIAM / 2, BALL_DIAM / 2, BALL_DIAM / 2, BALL_DIAM / 2);

            // draw paddle bitmap
            Paddle.DrawRectangle(Colors.Blue, 5, 0, 0, PADDLE_WIDTH, PADDLE_HEIGHT, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            
            // start ball at middle of screen
            BallPositionY =  (displayN18.Height / 2);
            BallPositionX = (displayN18.Width / 2);

            // start paddle at middle of screen
            PaddlePosition = (displayN18.Width / 2) - (PADDLE_WIDTH / 2);

            // draw ball
            displayN18.Draw(Ball, BallPositionX - (BALL_DIAM / 2), BallPositionY - (BALL_DIAM / 2));

            // draw paddle
            DrawPaddle(PaddlePosition);

            // score starts at 0
            int score = 0;

            // round score starts at 0
            int roundScore = 0;

            // start ball going straight down at paddle
            int vX = 0, vY = START_SPEED;

            // new game flag
            bool newGame = true;

            while (lives > 0)
            {
                // check new game flag
                if (newGame)
                {
                    // wait for user to move joystick
                    WaitForJystk();
                    newGame = false;
                }

                // draw paddle in new position
                DrawPaddle(PaddlePosition);

                // draw a new header
                DrawHeader(score, lives);

                // draw over old ball
                displayN18.Draw(ClearBall, BallPositionX - (BALL_DIAM / 2), BallPositionY - (BALL_DIAM / 2));

                // change the current position
                BallPositionX += vX;
                BallPositionY += vY;

                // check if out of bounds:
                // left
                if (BallPositionX < (BALL_DIAM / 2))
                    BallPositionX = (BALL_DIAM / 2);
                // right
                else if (BallPositionX > displayN18.Width - (BALL_DIAM / 2))
                    BallPositionX = displayN18.Width - (BALL_DIAM / 2);
                // top
                if (BallPositionY < (BALL_DIAM / 2) + HEADER_HEIGHT)
                    BallPositionY = (BALL_DIAM / 2) + HEADER_HEIGHT;
                // bottom
                else if (BallPositionY > displayN18.Height - (BALL_DIAM / 2) - PADDLE_HEIGHT)
                    BallPositionY = displayN18.Height - (BALL_DIAM / 2) - PADDLE_HEIGHT;

                // draw ball in new position
                displayN18.Draw(Ball, BallPositionX - (BALL_DIAM / 2), BallPositionY - (BALL_DIAM / 2));

                // check for joystick movement
                double jstX = joystick.GetPosition().X;

                // right
                if (jstX > .5 && (BallPositionX < displayN18.Width - 23))
                {
                    PaddlePosition += PADDLE_STEP;
                }
                // left
                else if (jstX < -.5 && BallPositionX > 0)
                {
                    PaddlePosition -= PADDLE_STEP;
                }

                // check that paddle is in bounds
                if (PaddlePosition < 0)
                    PaddlePosition = 0;
                else if (PaddlePosition > displayN18.Width - PADDLE_WIDTH)
                    PaddlePosition = displayN18.Width - PADDLE_WIDTH;

                // if hit a wall
                if (BallPositionX <= (BALL_DIAM / 2) || BallPositionX >= displayN18.Width - (BALL_DIAM / 2))
                    // switch x direction
                    vX = -vX;

                // if hit top
                if (BallPositionY <= (BALL_DIAM / 2) + HEADER_HEIGHT)
                {
                    // add 1 to score
                    score++;

                    // switch y direction
                    vY = -vY;

                    // create random number generator
                    Random rand = new Random();

                    // random x speed
                    if (rand.Next(2) == 1)
                        vX = -rand.Next(8);
                    else
                        vX = rand.Next(8);

                    // increase y speed every 4 points
                    vY = START_SPEED + roundScore / SPEEDUP;
                }

                // if hit bottom
                else if (BallPositionY >= displayN18.Height - (BALL_DIAM / 2) - PADDLE_HEIGHT)
                {
                    // if hit paddle
                    if (BallPositionX <= PaddlePosition + PaddleWidth && BallPositionX >= PaddlePosition)
                        // switch y direction
                        vY = -vY;
                    
                    // if missed paddle
                    else
                    {
                        // remove a life
                        lives--;

                        // draw over the old ball
                        displayN18.Draw(ClearBall, BallPositionX - (BALL_DIAM / 2), BallPositionY - (BALL_DIAM / 2));

                        // restart ball position
                        BallPositionX = displayN18.Width / 2;
                        BallPositionY = displayN18.Height / 2;

                        // draw ball in new position
                        displayN18.Draw(Ball, BallPositionX - (BALL_DIAM / 2), BallPositionY - (BALL_DIAM / 2));

                        // restart paddle position
                        PaddlePosition = (displayN18.Width / 2) - (PADDLE_WIDTH / 2);

                        // draw paddle in new position
                        DrawPaddle(PaddlePosition);

                        // restart ball speed
                        vX = 0;
                        vY = START_SPEED;

                        // set new game flag
                        newGame = true;

                        // reset round score
                        roundScore = 0;
                    }
                }

                // wait
                Thread.Sleep(WAIT_TIME);
            }

            // draw over old ball
            displayN18.Draw(ClearBall, BallPositionX - (BALL_DIAM / 2), BallPositionY - (BALL_DIAM / 2));


            return score;
        }

        // Function: WaitForJystk
        // waits for user to move joystick to left or right
        private void WaitForJystk()
        {
            // wait
            while (joystick.GetPosition().X < .25 && joystick.GetPosition().X > -.2) ;
        }

        // Function: DrawHeader
        // given score and lives, draws a new header to the screen that displays the score and lives
        private void DrawHeader(int score, int lives)
        {
            // create new header bitmap
            Bitmap HeaderBitmap = new Bitmap(displayN18.Width, HEADER_HEIGHT);

            // create string to draw
            string str = "score: " + score + " lives: " + lives;

            // draw rectangle
            HeaderBitmap.DrawRectangle(Color.White, 1, 0, 0, displayN18.Width, HEADER_HEIGHT, 0, 0, Color.White, 0, 0, Color.White, displayN18.Width, HEADER_HEIGHT, 0xFF);

            // draw string
            HeaderBitmap.DrawText(str, Resources.GetFont(Resources.FontResources.small), Color.Black, 0, 0);

            // erase the previous header
            displayN18.Draw(new Bitmap(displayN18.Width, HEADER_HEIGHT), 0, 0);

            // draw the new header
            displayN18.Draw(HeaderBitmap, 0, 0);
        }
        
        // Function: DrawPaddle
        // given a position, draws the paddle there and removes the old one
        private void DrawPaddle (int x)
        {
            // draw over the old paddle
            displayN18.Draw(new Bitmap(displayN18.Width, HEADER_HEIGHT), 0, displayN18.Height - PADDLE_HEIGHT);

            // draw the new paddle
            displayN18.Draw(Paddle, x, displayN18.Height - PADDLE_HEIGHT);
        }
    }
}
