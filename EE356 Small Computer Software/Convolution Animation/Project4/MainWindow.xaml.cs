using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Project4
{
    /// Interaction logic for MainWindow.xaml
    public partial class MainWindow : Window
    {
        // create backgroudworker
        static BackgroundWorker bkw = new BackgroundWorker();

        // create lists of bitmaps
        List<RenderTargetBitmap> slidingBmpList = new List<RenderTargetBitmap>();
        List<RenderTargetBitmap> convOutputBmpList = new List<RenderTargetBitmap>();

        // create bitmap for impulse response
        RenderTargetBitmap impulseBmp;

        // create bitmap for final output
        RenderTargetBitmap outputBmp;

        // create bitmap for empty convolution step output
        RenderTargetBitmap emptyConvBmp;

        // create bool for checking if data is ready to display
        int currStep = 0;
        
        // create var for storing impulse response length
        int numImpulseEls = 0;

        // create vars for storing step distances;
        double widthStep;
        double heightStep;

        public MainWindow()
        {
            InitializeComponent();
            // set up background worker
            bkw.DoWork += new DoWorkEventHandler(bkw_DoWork);
            bkw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bkw_RunWorkerCompleted);
            bkw.ProgressChanged += new ProgressChangedEventHandler(bkw_ProgressChanged);
            bkw.WorkerReportsProgress = true;

            // disable step mode buttons
            button_Back.IsEnabled = false;
            button_Forward.IsEnabled = false;
            button_Finish.IsEnabled = false;
        }

        // Function: bkw_ProgressChanged
        // runs when backgroundworker reports progress
        // displays the next step in the convolution on the output
        private void bkw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            // get current place
            int x = (int)e.UserState;

            // show the correct reversed impulse bitmap
            image_convolving.Source = slidingBmpList[x];

            // show the correct output for this step
            if (x >= 3)
                image_output.Source = convOutputBmpList[x - 3];
        }

        // Function: bkw_RunWorkerCompleted
        // runs when background worker is finished (after sliding animation is finished)
        private void bkw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            // enable the start button
            button_Start.IsEnabled = true;

            // display the impulse response
            image_convolving.Source = impulseBmp;

            // display the final output
            image_output.Source = outputBmp;
        }

        // Function: bkw_DoWork
        // runs when background worker is started
        // 
        private void bkw_DoWork(object sender, DoWorkEventArgs e)
        {
            // loop through the bitmaps
            for (int x = 0; x < slidingBmpList.Count; x++)
            {
                // report progress
                bkw.ReportProgress(0, x);
                // wait
                Thread.Sleep(9000 / slidingBmpList.Count);
            }
        }

        // Function: button_Start_Click
        // runs when user clicks the start button
        // gets the user input, and does the convolution calculation
        private void button_Start_Click(object sender, RoutedEventArgs e)
        {
            // Disable the start button
            button_Start.IsEnabled = false;
             
            // read input and impulse text boxes
            string[] inputStrs = txtbox_input.Text.Replace(" ", "").Split(",".ToCharArray());
            string[] impulseStrs = txtbox_impulseresp.Text.Replace(" ", "").Split(",".ToCharArray());

            // create lists for storing the data
            List<double> input = new List<double>();
            List<double> impulse = new List<double>();

            double n = 0;

            // parse input
            foreach (string element in inputStrs)
            {
                if (!double.TryParse(element, out n))
                {
                    // display an error message if could not parse
                    MessageBox.Show("Invalid input data!", "Input Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
                // add the data to the list
                input.Add(n);
            }

            // parse impulse response
            foreach (string element in impulseStrs)
            {
                if (!double.TryParse(element, out n))
                {
                    // display an error message if could not parse
                    MessageBox.Show("Invalid impulse response data!", "Input Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
                // add the data to the list
                impulse.Add(n);
            }

            // calculate needed the distance between ticks in order to fit data on graph
            if (input.Count >= impulse.Count)
                widthStep = (image_input.Width / input.Count) / 5;
            else
                widthStep = (image_input.Width / impulse.Count) / 5;

            double max1 = 0, max2 = 0;

            foreach (double element in input)
                if (Math.Abs(element) > max1)
                    max1 = Math.Abs(element);
            foreach (double element in impulse)
                if (Math.Abs(element) > max2)
                    max2 = Math.Abs(element);

            if (max1 > max2)
                heightStep = (image_input.Height / 2) / (max1 + 1);
            else
                heightStep = (image_input.Height / 2) / (max2 + 1);

            // create list of bitmaps for displaying each convolution step output
            convOutputBmpList = new List<RenderTargetBitmap>();

            // Delcare pen
            Pen blkPen = new Pen(Brushes.Black, 1);

            //Create a DrawingVisual and a DrawingContext
            DrawingVisual vis = new DrawingVisual();
            DrawingContext dc;

            // Open the DrawingContext 
            dc = vis.RenderOpen();

            // draw the axis's and ticks
            DrawAxis(ref dc);

            // Draw the input data
            for (int x = 0; x < input.Count; x++)
            {
                dc.DrawLine(blkPen, new Point((image_input.Width/2) + x * widthStep, image_input.Height/2), new Point((image_input.Width / 2) + x * widthStep, (image_input.Height / 2) - heightStep * input[x]));
            }
            
            // Close the drawing context
            dc.Close();
            // Create the bitmap for the visual
            RenderTargetBitmap bmp = new RenderTargetBitmap((int)image_input.Width, (int)image_input.Height, 96, 96, PixelFormats.Pbgra32);
            // Render the visual to the bitmap
            bmp.Render(vis);

            // display bitmap on the input image
            image_input.Source = bmp;

            // draw a bitmap of the impulse response
            vis = new DrawingVisual();
            dc = vis.RenderOpen();

            // draw the axis's and ticks
            DrawAxis(ref dc);

            // draw the impulse response data
            for (int x = 0; x < impulse.Count; x++)
            {
                dc.DrawLine(blkPen, new Point((image_output.Width / 2) + x * widthStep, image_output.Height / 2), new Point((image_output.Width / 2) + x * widthStep, (image_output.Height / 2) - heightStep * impulse[x]));
            }

            // close the drawing context
            dc.Close();
            // render the visual to a bitmap
            bmp = new RenderTargetBitmap((int)image_convolving.Width, (int)image_convolving.Height, 96, 96, PixelFormats.Pbgra32);
            bmp.Render(vis);

            // store the bitmap for displaying later
            impulseBmp = bmp;

            // create dictionary for holding reversed impulse data
            Dictionary<int, double> reversedImpulse = new Dictionary<int, double>();

            // reverse the impulse response and store in dictionary
            for (int i = 0; i < impulse.Count; i++)
                reversedImpulse[-i] = impulse[i];

            // create lists for storing outputs
            List<Dictionary<int, double>> convOutputs = new List<Dictionary<int, double>>();
            List<double> output = new List<double>();

            // save the number of elements in each list
            int numinputEls = input.Count;
            numImpulseEls = reversedImpulse.Count;

            // add necessary filler of 0's to reversed impulse response
            for (int i = 0; i < numinputEls + numImpulseEls + 1; i++)
                reversedImpulse[-(numImpulseEls + i)] = 0;

            // draw sliding reversed impulse response:
            slidingBmpList = new List<RenderTargetBitmap>();

            // draw a bitmap for each step
            for (int x = -numImpulseEls - 2; x <= numinputEls; x++)
            {
                // open the visual
                vis = new DrawingVisual();
                dc = vis.RenderOpen();

                // draw the axis's
                DrawAxis(ref dc);

                // draw the shifted reversed impulse
                for (int i = 0; i < numImpulseEls; i++)
                {
                    n = 0;
                    reversedImpulse.TryGetValue(-(numImpulseEls - 1) + i, out n);
                    dc.DrawLine(blkPen, new Point((image_convolving.Width / 2) + (x + i) * widthStep, image_convolving.Height / 2), new Point((image_convolving.Width / 2) + (x + i) * widthStep, (image_convolving.Height / 2) - heightStep * n));
                }

                // close the drawing context
                dc.Close();

                // render to a bitmap
                bmp = new RenderTargetBitmap((int)image_convolving.Width, (int)image_convolving.Height, 96, 96, PixelFormats.Pbgra32);
                bmp.Render(vis);

                // Add the bitmap to the List
                slidingBmpList.Add(bmp);
            }

            // create vars for handling output
            double total;
            double result;

            // do convolution calculation
            for (int x = 0; x < numinputEls + reversedImpulse.Count; x++)
            {
                convOutputs.Add(new Dictionary<int, double>());

                total = 0;
                for (int i = 0; i < x+1; i++)
                {
                    n = 0;
                    reversedImpulse.TryGetValue(-x + i, out n);
                    if (i < input.Count)
                        result = (n * input[i]);
                    else
                        result = 0;

                    // store data in dictionaries
                    convOutputs[x][i] = result;
                    // add up each result at each step for final output
                    total += result;
                }
                // store final output in list
                output.Add(total);
            }

            // calculate necessary distance between points for displaying convolution steps on graph
            max1 = 0;

            foreach (Dictionary<int, double> element in convOutputs)
            {
                for (int i = element.Keys.ElementAt(0); i < element.Keys.ElementAt(element.Keys.Count - 1); i++)
                {
                    n = 0;
                    element.TryGetValue(i, out n);
                    if (n > max1)
                        max1 = n;
                }
            }

            heightStep = (image_output.Height / 2) / (max1+1);

            // draw a bitmap for each convolution step
            foreach (Dictionary<int, double> dict in convOutputs)
            {
                // open the visual
                vis = new DrawingVisual();
                dc = vis.RenderOpen();

                // draw the axis's and ticks
                DrawAxis(ref dc);

                // draw the data
                for (int j = 0; j < dict.Count; j++)
                {
                    n = 0;
                    dict.TryGetValue(j, out n);
                    dc.DrawLine(blkPen, new Point((image_output.Width / 2) + j * widthStep, image_output.Height / 2), new Point((image_output.Width / 2) + j * widthStep, (image_output.Height / 2) - heightStep * n));
                }

                // close the drawing context
                dc.Close();

                // render the visual to a bitmap
                bmp = new RenderTargetBitmap((int)image_convolving.Width, (int)image_convolving.Height, 96, 96, PixelFormats.Pbgra32);
                bmp.Render(vis);

                // Add the bitmap to the List
                convOutputBmpList.Add(bmp);
            }
            
            // draw an empty convolution step bitmap
            dc = vis.RenderOpen();

            // draw the axis's and ticks
            DrawAxis(ref dc);

            // Close the drawing context
            dc.Close();

            // Create the bitmap for the visual
            bmp = new RenderTargetBitmap((int)image_input.Width, (int)image_input.Height, 96, 96, PixelFormats.Pbgra32);
            // Render the visual to the bitmap
            bmp.Render(vis);

            // display a empty graph on output
            image_output.Source = bmp;
            emptyConvBmp = bmp;

            // calculate necessary distance between points for displaying final output on graph
            max1 = 0;

            foreach (double element in output)
                if (Math.Abs(element) > max1)
                    max1 = Math.Abs(element);

            heightStep = (image_output.Height / 2) / (max1 + 1);

            // draw the output data:
            // open the visual
            vis = new DrawingVisual();
            dc = vis.RenderOpen();

            // draw the axis's and ticks
            DrawAxis(ref dc);

            // draw the data
            for (int x = 0; x < output.Count; x++)
            {
                dc.DrawLine(blkPen, new Point((image_output.Width / 2) + x * widthStep, image_output.Height / 2), new Point((image_output.Width / 2) + x * widthStep, (image_output.Height / 2) - heightStep * output[x]));
            }

            // close the drawing context
            dc.Close();

            // render to bitmap
            bmp = new RenderTargetBitmap((int)image_output.Width, (int)image_output.Height, 96, 96, PixelFormats.Pbgra32);
            bmp.Render(vis);

            // store bitmap for displaying later
            outputBmp = bmp;

            // check if user is in "Step Mode"
            if (!checkBox_StepThrough.IsChecked ?? false)
            {
                // if not, run backgroundworker to display animation
                bkw.RunWorkerAsync();
            }
            else
            {
                // if yes, then display first step and wait for user to step through
                currStep = 0;
                DisplayStep();
                button_Finish.IsEnabled = true;
            }
        }

        // Function: DisplayStep
        // for "Step Mode", displays the current step of the convolution animation
        private void DisplayStep()
        {
            // if at first step, disable the back button
            if (currStep == 0)
            {
                button_Back.IsEnabled = false;
                button_Forward.IsEnabled = true;
            }
            // if at last step, disable the forward button
            else if (currStep == slidingBmpList.Count - 1)
            {
                button_Forward.IsEnabled = false;
                button_Back.IsEnabled = true;
            }
            // otherwise, enable both
            else
            {
                button_Back.IsEnabled = true;
                button_Forward.IsEnabled = true;
            }

            // display the reversed impuse bitmap for this step
            image_convolving.Source = slidingBmpList[currStep];

            // display the output at this step
            if (currStep >= 3)
                image_output.Source = convOutputBmpList[currStep - 3];
            else
                image_output.Source = emptyConvBmp;
        }

        // Function: DrawAxis
        // draws the x and y axis, along with tick marks
        // input (passed by reference): drawing context to draw on
        private void DrawAxis(ref DrawingContext dc)
        {
            // create a gray pen
            Pen grayPen = new Pen(Brushes.LightGray, 1);
            // draw the x and y axis's
            dc.DrawLine(grayPen, new Point(0, image_output.Height / 2), new Point(image_output.Width,(image_output.Height / 2)));
            dc.DrawLine(grayPen, new Point(image_output.Width / 2, 0), new Point(image_output.Width / 2, image_output.Height));

            // draw x-axis tick marks
            for (int i = 0; i < image_output.Width; i++)
                dc.DrawLine(grayPen, new Point(i*widthStep, (image_output.Height / 2) + (heightStep / 2)), new Point(i*widthStep, (image_output.Height / 2) - (heightStep / 2)));

            // draw the y-axis tick marks
            for (int i = 0; i < image_output.Height / 2; i++)
            {
                dc.DrawLine(grayPen, new Point((image_output.Width / 2) + (widthStep / 2), (image_output.Height / 2) + i * heightStep), new Point((image_output.Width / 2) - (widthStep / 2), (image_output.Height / 2) + i * heightStep));
                dc.DrawLine(grayPen, new Point((image_output.Width / 2) + (widthStep / 2), (image_output.Height / 2) - i * heightStep), new Point((image_output.Width / 2) - (widthStep / 2), (image_output.Height / 2) - i * heightStep));
            }
        }

        // Function: button_Back_Click
        // runs when user clicks back button
        // For "Step Mode", displays the previous step of the animation
        private void button_Back_Click(object sender, RoutedEventArgs e)
        {
            // decrement curret step variable
            currStep--;
            // display the current step
            DisplayStep();
        }

        // Function: button_Forward_Click
        // runs when user clicks forward button
        // For "Step Mode", displays the next step of the animation
        private void button_Forward_Click(object sender, RoutedEventArgs e)
        {
            // increment the current step variable
            currStep++;
            // display the current step
            DisplayStep();
        }

        // Function: button_Finish_Click
        // runs when user clicks Finish button
        // For "Step Mode", displays the final output
        private void button_Finish_Click(object sender, RoutedEventArgs e)
        {
            // enable the start button
            button_Start.IsEnabled = true;

            // disable the step buttons
            button_Back.IsEnabled = false;
            button_Forward.IsEnabled = false;
            button_Finish.IsEnabled = false;

            // display the impulse response
            image_convolving.Source = impulseBmp;

            // display the final output
            image_output.Source = outputBmp;
        }

        // Function: button_Ex1_Click
        // runs when user clicks Ex1 button
        // puts example data into the textboxes
        private void button_Ex1_Click(object sender, RoutedEventArgs e)
        {
            txtbox_impulseresp.Text = ".5, 1, 2";
            txtbox_input.Text = "1, 2, 2, 1";
        }

        // Function: button_Ex2_Click
        // runs when user clicks Ex2 button
        // puts example data into the textboxes
        private void button_Ex2_Click(object sender, RoutedEventArgs e)
        {
            txtbox_impulseresp.Text = "-3, -1, 7, 1";
            txtbox_input.Text = "6, 6, 2, 3, 7, 8";
        }
    }
}
