using System;
using System.Windows;
using System.Windows.Forms.Integration;
using System.Windows.Forms.DataVisualization.Charting;
using System.IO;
using System.Media;
using System.ComponentModel;
using System.Threading;

namespace Project3
{
    /// Interaction logic for MainWindow.xaml
    public partial class MainWindow : Window
    {
        // create chart for plotting graphs
        private Chart cht = new Chart();

        // create BackgroundWorker for doing FFT
        static BackgroundWorker bkw = new BackgroundWorker();

        // vars for storing data
        private float[] wavData;
        private Complex[] complexData;
        private int sampleRate;
        private int numSamples;
        private int extraSamples;

        public MainWindow()
        {
            InitializeComponent();

            // set up BackgroundWorker
            bkw.DoWork += new DoWorkEventHandler(bkw_DoWork);
            bkw.ProgressChanged += new ProgressChangedEventHandler(bkw_ProgressChanged);
            bkw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bkw_Completed);
            bkw.WorkerReportsProgress = true;
            bkw.WorkerSupportsCancellation = true;

            // initialize progress bar
            progressbar.Value = 0;

            // set dimensions of chart
            cht.Width = 950;
            cht.Height = 500;
            cht.Location = new System.Drawing.Point(10, 10);
        }

        // Function: cntChart_Loaded
        // runs when chart is loaded
        // creates a windowsformshost to display the chart
        private void cnvChart_Loaded(object sender, RoutedEventArgs e)
        {
            WindowsFormsHost host = new WindowsFormsHost();
            host.Child = cht;
            // Add the chart to the canvas so it can be displayed
            this.cnvChart.Children.Add(host);
        }

        // Function: OpenFile_Click
        // opens an open file dialog for user to choose wav file
        private void OpenFile_Click(object sender, RoutedEventArgs e)
        {
            // open file chooser
            Microsoft.Win32.OpenFileDialog fileChooser = new Microsoft.Win32.OpenFileDialog();
            // only show wav files
            fileChooser.Filter = "Wav Files|*.wav";

            // if chose a file, display the filename in textbox
            if (fileChooser.ShowDialog() != false)
                txtbox_Path.Text = fileChooser.FileName;
        }

        // Function: Play_Click
        // plays the loaded wav file
        private void Play_Click(object sender, RoutedEventArgs e)
        {
            // if no file loaded, then display messagebox
            if (txtbox_Path.Text == "Filename")
            {
                System.Windows.MessageBox.Show("Choose a valid wav file first!", "File not loaded", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            
            // create sound player
            SoundPlayer player = new SoundPlayer(txtbox_Path.Text);

            // play the file
            try
            {
                player.Play();
            }
            catch
            {
                // if couldn't play, display message box
                System.Windows.MessageBox.Show("Invalid file choice!", "Can't play file", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
        }

        // Function: Clear_Click
        // clears the graph when button is clicked
        private void Clear_Click(object sender, RoutedEventArgs e)
        {
            // clear the chart areas and series
            cht.ChartAreas.Clear();
            cht.Series.Clear();
        }

        // Function: Plot_Click
        // loads and plots the wav file
        private void Plot_Click(object sender, RoutedEventArgs e)
        {
            // if no file chosen, then display message box
            if (txtbox_Path.Text == "Filename")
            {
                System.Windows.MessageBox.Show("Choose a valid file first!", "File not loaded", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // open the file
            FileStream fIn = new FileStream(txtbox_Path.Text, FileMode.Open);
            BinaryReader reader = new BinaryReader(fIn);

            // read the data
            int chunkID = reader.ReadInt32();
            int fileSize = reader.ReadInt32();
            int riffType = reader.ReadInt32();
            int fmtID = reader.ReadInt32();
            int fmtSize = reader.ReadInt32();
            int fmtCode = reader.ReadInt16();
            int channels = reader.ReadInt16();
            if (channels != 1)
            {
                // display message box if not a mono channel file
                string msg = "File must be mono only";
                System.Windows.MessageBox.Show(msg, "Mono Only", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // read the rest of data
            sampleRate = reader.ReadInt32();
            int fmtAvgBPS = reader.ReadInt32();
            int fmtBlockAlign = reader.ReadInt16();
            int bitDepth = reader.ReadInt16();
            if (fmtSize == 18)
            {// Read any extra values 
                int fmtExtraSize = reader.ReadInt16();
                reader.ReadBytes(fmtExtraSize);
            }
            int dataID = reader.ReadInt32();
            int dataSize = reader.ReadInt32();
            byte[] byteData = reader.ReadBytes(dataSize);
            reader.Close();
            if (byteData.Length > 0)
            {
                wavData = new float[byteData.Length / 2];
                for (int i = 0; i < byteData.Length; i += 2)
                {
                    int d = (Int16)(byteData[i + 1] * 256 + byteData[i]);
                    if (d >= 0)
                        wavData[i / 2] = (float)d / 32768;
                    else
                        wavData[i / 2] = -(float)(~(d - 1)) / 32768;
                }

                // close the wav file
                fIn.Close();

                // clear the chart
                cht.ChartAreas.Clear();
                cht.Series.Clear();

                // if time radio box is chosen, then plot in time
                but_Plot.IsEnabled = false;

                // plot wavdata in time
                Series timeSeries = new Series();
                timeSeries.ChartType = SeriesChartType.Line;
                double t = 0;
                double T = 1 / (double)sampleRate;
                for (int i = 0; i < wavData.Length; i++)
                {
                    timeSeries.Points.AddXY(t, wavData[i]);
                    t += T;
                }
                // add axis titles
                cht.Series.Add(timeSeries);
                cht.ChartAreas.Add("time");
                timeSeries.ChartArea = "time";
                cht.ChartAreas[0].AxisX.Title = "Time";
                cht.ChartAreas[0].AxisX.LabelStyle.Format = "{0.00}";
                cht.ChartAreas[0].AxisY.Title = "wavData";
                cht.ChartAreas[0].AxisY.LabelStyle.Format = "{0.00}";

                // do FFT
                numSamples = wavData.Length;
                bkw.RunWorkerAsync();
            }
        }

        // Function: bkw_DoWork
        // does FFT in background
        private void bkw_DoWork(object sender, DoWorkEventArgs e)
        {
            int N = numSamples;

            int extra = 0;

            // make N a power of 2
            while (!((N + extra != 0) && ((N + extra & (N + extra - 1)) == 0)))
                extra++;

            extraSamples = extra;

            complexData = new Complex[N + extra];

            // copy data to array of complex numbers
            int j;
            for (j = 0; j < N; j++)
                complexData[j] = new Complex(wavData[j], 0);

            // fill the end of the data with 0's
            while (j < N + extra)
            {
                complexData[j] = new Complex(0, 0);
                j++;
            }

            // run FFT
            FFT(complexData, N + extra, (int)(Math.Log10(N + extra) / Math.Log10(2)));
        }

        // Function: bkw_ProgressChanged
        // updates the progress bar while FFT is running
        private void bkw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressbar.Value = e.ProgressPercentage;
        }

        // Function: bkw_Completed
        // runs when FFT is completed
        // plots the results on a graph
        private void bkw_Completed(object sender, RunWorkerCompletedEventArgs e)
        {
            // reset the progress bar
            progressbar.Value = 0;

            // plot wavdata in frequency
            Series freqSeries = new Series();
            freqSeries.ChartType = SeriesChartType.Line;
            freqSeries.Color = System.Drawing.Color.Red;
            double f = 0;
            double F = (double)sampleRate/ (double)(numSamples + extraSamples);
            for (int i = 0; i < (numSamples + extraSamples) / 2; i++)
            {
                freqSeries.Points.AddXY(f, complexData[i].Magnitude());
                f += F;
            }

            // add to the graph
            cht.Series.Add(freqSeries);
            cht.ChartAreas.Add("frequency");
            freqSeries.ChartArea = "frequency";
            cht.ChartAreas[1].AxisX.Title = "Frequency";
            cht.ChartAreas[1].AxisX.LabelStyle.Format = "{0.00}";
            cht.ChartAreas[1].AxisY.Title = "Magnitude";
            cht.ChartAreas[1].AxisY.LabelStyle.Format = "{0.00}";

            // re-enable the Plot button
            but_Plot.IsEnabled = true;
        }

        // Function: FFT
        // does fast fourier transform on wav file
        private void FFT(Complex[] d, int N, int p)
        {
            int i, j = 1, k, L;
            int ip;
            int exp2L;
            Complex u = new Complex(0, 0);
            Complex W = new Complex(0, 0);
            Complex t = new Complex(0, 0);
            for (i = 1; i < N; i++)
            {
                if (i < j)
                    Swap(ref d[i - 1], ref d[j - 1]);
                k = N / 2;
                while (k < j)
                {
                    j = j - k;
                    k = k / 2;
                }
                j = j + k;
            }

            for (L = 1; L <= p; L++)
            {
                // report progress to BackgroundWorker
                bkw.ReportProgress((int)(((double)L / (double)p) * 100));
                Thread.Sleep(100);

                exp2L = (int)Math.Pow(2, L) / 2;
                u.Real = 1; u.Imag = 0;
                W.Real = Math.Cos(Math.PI / exp2L);
                W.Imag = -Math.Sin(Math.PI / exp2L);
                for (j = 1; j <= exp2L; j++)
                {
                    for (i = j; i <= N; i = i + 2 * exp2L)
                    {
                        ip = i + exp2L;
                        t = d[ip - 1] * u;
                        d[ip - 1] = d[i - 1] - t;
                        d[i - 1] = d[i - 1] + t;
                    }
                    u = u * W;
                }
            }
        }

        // Function: Swap
        // switches the values of the 2 inputs
        private void Swap(ref Complex x, ref Complex y)
        {
            Complex tmp = new Complex(x.Real, x.Imag);
            x.Real = y.Real; x.Imag = y.Imag;
            y.Real = tmp.Real; y.Imag = tmp.Imag;
        }

        // Class: Complex
        // stores a number containing real and imaginary parts
        class Complex
        {
            public Complex()
            {
                real = 0;
                imag = 0;
            }
            public Complex(double x, double y)
            {
                real = x;
                imag = y;
            }
            private double real;
            private double imag;
            // 
            public double Real
            {
                get { return real; }
                set { real = value; }
            }
            public double Imag
            {
                get { return imag; }
                set { imag = value; }
            }
            public double Magnitude()
            {
                double tmp;
                tmp = Math.Sqrt(Real * Real + Imag * Imag);
                return tmp;
            }
            public static Complex operator +(Complex x, Complex y)
            {
                Complex cTmp = new Complex();
                cTmp.real = x.real + y.real;
                cTmp.imag = x.imag + y.imag;
                return cTmp;
            }
            public static Complex operator -(Complex x, Complex y)
            {
                Complex cTmp = new Complex();
                cTmp.real = x.real - y.real;
                cTmp.imag = x.imag - y.imag;
                return cTmp;
            }
            public static Complex operator *(Complex x, Complex y)
            {
                Complex cTmp = new Complex();
                cTmp.real = x.Real * y.Real - x.Imag * y.Imag;
                cTmp.imag = x.Real * y.Imag + x.Imag * y.Real;
                return cTmp;
            }
        }
    }
}
