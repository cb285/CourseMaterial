// Clayton Brutus
// EE356 Project 1 - Dictionary Words
// 9/5/16

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using Microsoft.Win32;
using Microsoft.VisualBasic;
using System.Text.RegularExpressions;

namespace Project_1
{
    /// Interaction logic for MainWindow.xaml
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        // Function: bt_Start_Click
        // handles a start button click
        private void bt_Start_Click(object sender, RoutedEventArgs e)
        {
            string path; // for holding path to word list file

            // check if custom path box is checked
            if (cb_CustPath.IsChecked ?? false)
                path = txtbox_Path.Text;
            else
                // if not checked then use default list
                path = "CrsWdLst.txt";

            // Read length text box
            string length = txtbox_Length.Text.ToLower().Replace(" ", string.Empty);

            // Find Words:
            if (rdo_FindWords.IsChecked ?? false)
            {
                // Read string text box
                string str = txtbox_String.Text.ToLower().Replace(" ", string.Empty);

                List<string> words;

                // Read list from file
                try
                {
                    words = ReadToList(path);
                } catch (ArgumentException exp)
                {   // display error message if couldn't open file
                    txtbox_Output.Text = ("Error: " + exp.Message);
                    return;
                }
                
                List<string> foundWords = new List<string>(); // create empty list for storing words

                int n = 0;

                // try to parse length text box for a number
                if (int.TryParse(length, out n))
                {
                    foundWords = SearchList(ref words, str, n, n);

                    // output list of words
                    if (!OutputList(ref foundWords))
                        txtbox_Output.Text = "No words found.";
                }

                // if length text box contains just an asterisc
                else if (length == "*")
                {
                    // search in word list
                    foundWords = SearchList(ref words, str);

                    // output list of words
                    if (!OutputList(ref foundWords))
                        txtbox_Output.Text = "No words found.";
                }
                // if length text box contains a range of numbers
                else if (length.Contains("-"))
                {
                    // create vars for storing range of numbers
                    string[] range;
                    int n1 = 0, n2 = 0;

                    // parse for both numbers
                    range = length.Split('-');
                    if(!int.TryParse(range[0], out n1) || !int.TryParse(range[1], out n2))
                    {
                        // output error message if couldn't parse
                        txtbox_Output.Text = "Error, invalid length input.";
                        return;
                    }
                    // search the word list for matching words
                    foundWords = SearchList(ref words, str, n1, n2);

                    // output the words
                    if (!OutputList(ref foundWords))
                        // output an error message if couldn't find any matches
                        txtbox_Output.Text = "No words found.";
                }
                // display error message if input is invalid
                else
                    txtbox_Output.Text = "Error, invalid length input.";
            }

            // Find Anagrams:
            else if (rdo_FindAnagrams.IsChecked ?? false)
            {
                // Read string text box
                string str = txtbox_String.Text.ToLower().Replace(" ", string.Empty);

                // create dictionary from word list file
                Dictionary<string, List<string>> AnagramDictionary = ReadToAnagramDict(path);

                // alphabetize the words for each word
                string wordLetters = Alphabetize(txtbox_String.Text.ToLower());
                
                // create new list
                List<string> anagrams = new List<string>();

                // see if there are any matches in the dictionary
                if (!AnagramDictionary.TryGetValue(wordLetters, out anagrams))
                {
                    // output message if none could be found
                    txtbox_Output.Text = "No anagrams exist.";
                    return;
                }
                // output the list of anagrams
                OutputList(ref anagrams);
            }

            // Find Wild Card Sequence:
            else if (rdo_FindWildCard.IsChecked ?? false)
            {
                // creates vars to store length range
                int n1 = 0, n2 = 0;

                // create list of words from word list file
                List<string> words = ReadToList(path);

                // Read string text box
                string str = txtbox_String.Text.ToLower().Replace(" ", string.Empty);

                // try to parse length textbox
                if (int.TryParse(length, out n2))
                {
                    // create list of matching words
                    List<string> foundWords = SearchList(ref words, str, n2, n2);

                    // output list of found words
                    if (!OutputList(ref foundWords))
                        // if no words are found then output a message
                        txtbox_Output.Text = "No words found.";
                }
                // if length contains a range of lengths
                else if (length.Contains("-"))
                {
                    // create var for storing range
                    string[] range;

                    // parse for numbers
                    range = length.Split('-');
                    if (!int.TryParse(range[0], out n1) || !int.TryParse(range[1], out n2))
                        txtbox_Output.Text = "Error, invalid Length input.";

                    // search for matching words
                    List<string> foundWords = SearchList(ref words, str, n1, n2);
                    
                    // output matching words
                    if (!OutputList(ref foundWords))
                        // output message if couldn't find any words
                        txtbox_Output.Text = "No words found.";
                }
                else
                {
                    // output message if Length input is invalid
                    txtbox_Output.Text = "Error, invalid Length input.";
                }
            }

            // Word Ladder:
            else
            {
                // create var for holding start and end words
                string[] targetWords;
                
                // parse string text box for words
                if (txtbox_String.Text.Contains("-") && !txtbox_String.Text.Contains(","))
                    targetWords = txtbox_String.Text.ToLower().Replace(" ", "").Split('-');
                else if (txtbox_String.Text.Contains(",") && !txtbox_String.Text.Contains("-"))
                    targetWords = txtbox_String.Text.ToLower().Replace(" ", "").Split(',');
                else if (txtbox_String.Text.Contains(" ") && !txtbox_String.Text.Contains(",") && !txtbox_String.Text.Contains("-"))
                    targetWords = txtbox_String.Text.ToLower().Split(' ');
                else
                {
                    // display message if input was invalid
                    txtbox_Output.Text = "Error, invalid String input.";
                    return;
                }

                // check if parsed both words correctly
                if (targetWords.Count() != 2)
                {
                    // output error message if input was invalid
                    txtbox_Output.Text = "Error, invalid String input.";
                    return;
                }
                
                // store start and end words and remove any spaces
                string startWord = targetWords[0].Replace(" ", "");
                string endWord = targetWords[1].Replace(" ", "");

                // create var for storing nextWord
                string nextWord;

                // read text file of words to a list
                List<string> words = ReadToList(path);

                // put the starting word in a list
                List<string> wordList = new List<string>();
                wordList.Add(startWord);

                // put the list in a queue
                Queue<List<string>> queueOfLists = new Queue<List<string>>();
                queueOfLists.Enqueue(wordList);

                // while not done
                while (queueOfLists.Count != 0)
                {
                    // get the next list from queue
                    List<string> currentList = queueOfLists.Dequeue();

                    while (true)
                    {
                        // find next real word that hasn't been used
                        nextWord = FindNextWord (ref words, ref queueOfLists, ref currentList);
                        
                        // create new list
                        List<string> listNew;

                        // if found end word
                        if (nextWord == endWord)
                        {
                            // output the path from start word to end word
                            OutputList(ref currentList);
                            txtbox_Output.AppendText(endWord);

                            return;
                        }
                        // if found a next word
                        else if (nextWord != "")
                        {
                            // put a copy of the current list in the new list
                            listNew = currentList.GetRange(0, currentList.Count);

                            // add the next word to new list
                            listNew.Add(nextWord);
                            
                            // enqueue the new list
                            queueOfLists.Enqueue(listNew);
                        }
                        // if couldn't find a next word then move on to next list in the queue
                        else if (nextWord == "")
                            break;
                    }
                }
                // if couldn't find a path to the end word
                txtbox_Output.Text = "Could not find a path.";
            }
        }

        // Function: FindNextWord
        // input: list of words (dictionary), queue of lists of strings, list of words
        // returns: a word that is made from changing one letter of the last word in the list of words, that is not in any list in the queue and is in the dictionary list
        private string FindNextWord(ref List<string> words, ref Queue<List<string>> queueOfLists, ref List<string> currentList)
        {
            string lastWord = currentList[currentList.Count - 1];

            for (int i = 0; i < lastWord.Length; i++)
            {
                // change letter i
                    char[] charray = (lastWord).ToCharArray();
                    charray[i] = '.';
                    string wordPattern = new string(charray);

                List<string> foundWords = SearchList(ref words, wordPattern, -2);


                    foreach (string foundWord in foundWords)
                    {
                        if (!QueueOfStringListsContains(queueOfLists, foundWord) && !currentList.Contains(foundWord))
                        {
                            if (words.Contains(foundWord))
                                return foundWord;
                        }
                    }
            }
            return "";
        }

        private bool QueueOfStringListsContains(Queue<List<string>> listQueue, string word)
        {
            foreach (List<string> list in listQueue)
            {
                if (list.Contains(word))
                    return true;
            }
            return false;
        }

        // Function: SearchList
        // input: list of strings, target string, range of length
        // returns a list of strings with a length within n1 and n2 that contains the target string
        private List<string> SearchList(ref List<string> words, string str, int n1 = -1, int n2 = 0)
        {
            List<string> foundWords = new List<string>();
            
            // if n1 is -2 then it is for word ladder
            if (n1 == -2)
            {
                // create regular expression pattern from string
                int L = str.Length;
                Regex pattern = new Regex("^" + str + "$");

                // search list for words matching pattern and length
                foreach (string word in words)
                    if (word.Length == L)
                        if (pattern.IsMatch(word))
                            foundWords.Add(word);
            }

            // if for wildcard
            else if (str.Contains("*") || str.Contains("."))
            {
                // create regular expression pattern from string
                Regex pattern = new Regex("^" + str.Replace("*", ".*") + "$");

                // search list for words matching pattern and length range
                foreach (string word in words)
                {
                    if (pattern.IsMatch(word) && (word.Length >= n1) && (word.Length <= n2))
                        foundWords.Add(word);
                }
            }

            // if n1 is -1 then for find words with any length
            else if (n1 == -1)
            {
                // search for words containing string
                foreach (string word in words)
                {
                    if (word.Contains(str))
                        foundWords.Add(word);
                }
            }

            // otherwise find words with length between n1 and n2
            else
            {
                // search for words containing string and between n1 and n2
                foreach (string word in words)
                {
                    if (word.Contains(str) && word.Length >= n1 && word.Length <= n2)
                        foundWords.Add(word);
                }
            }
            // return list of matching words
            return foundWords;
        }

        // Function: ReadToList
        // input: path to word list txt file
        // returns the list of words
        private List<string> ReadToList(string path)
        {
            // check if file exists
            if (File.Exists(path))
            {
                // create vars for storing words
                string line;
                List<string> words = new List<string>();

                // read each line into list
                using (StreamReader sr = new StreamReader(path))
                {
                    for (int i = 0; (line = sr.ReadLine()) != null; i++)
                    {
                        words.Add(line.ToLower());
                    }

                    // close the file
                    sr.Close();
                    // return the word list
                    return words;
                }
            }

            // throw exception if file doesn't exist
            else
            {
                throw new ArgumentException("File doesn't exist.");
            }  
        }
        // Function: ReadToAnagramDict
        // input: path to word list txt file
        // returns a dictionary of strings with keys that contain the letters making up each word in alphabetical order
        private Dictionary<string, List<string>> ReadToAnagramDict(string path)
        {
            // check if file exists
            if (File.Exists(path))
            {
                // create vars for storing words and key
                Dictionary<string, List<string>> dict = new Dictionary<string, List<string>>();
                string line = null;
                string key = null;

                // read each line into dictionary
                using (StreamReader sr = new StreamReader(path))
                {
                    while ((line = sr.ReadLine()) != null)
                    {
                        key = Alphabetize(line);

                        if (dict.ContainsKey(key))
                            dict[key].Add(line);
                        else
                        {
                            List<string> newList = new List<string>();
                            newList.Add(line);
                            dict.Add(key, newList);
                        }
                    }
                    // close the file
                    sr.Close();
                    // return the dictionary
                    return dict;
                }
            }
            // throw exception if file doesn't exist
            else
            {
                throw new ArgumentException("Error, file does not exist.");
            }
        }
        // Function: Alphabetize
        // input: a string
        // returns string with letters in alphabetic order
        private string Alphabetize(string str)
        {
            // sort the string
            char[] arr = str.ToArray();
            Array.Sort(arr);
            // return the string
            return new string(arr);
        }
        // Function: OutputList
        // input: list of strings
        // writes each string in the list to a line in the output text box
        // returns true if succeeded, false if list was empty
        private bool OutputList(ref List<string> list)
        {
            // clear the output text box
            txtbox_Output.Clear();

            // return false if list is empty
            if (list.Count == 0)
                return false;
            // right each string to a line in the output box
            for (int i = 0; i < list.Count; i++)
            {
                txtbox_Output.AppendText(list[i]);
                txtbox_Output.AppendText("\n");
            }
            // return true when done
            return true;
        }
        // Function: but_Pickfile_Click
        // handles clicking of "Custom List" button
        // opens openfiledialog
        // writes path to the path text box if user picked a file
        // clears path text box if they didn't pick a file
        private void but_Pickfile_Click(object sender, RoutedEventArgs e)
        {
            // open file chooser
            OpenFileDialog fileChooser = new OpenFileDialog();
            if (fileChooser.ShowDialog() == false)
                // clear txt box if did't pick a file
                txtbox_Path.Text = "";
            else
                // write path to txt box if did pick a file
                txtbox_Path.Text = fileChooser.FileName;
        }
        // Function: bt_About_Click
        // handles clicking of "About" button
        // displays project title, author, creation date
        private void bt_About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("EE356 Project 1: Dictionary Words\nAuthor: Clayton Brutus\nDate: 9/4/2016");
        }
        // Function: bt_Help_Click
        // handles clicking of "Help" button
        // displays instructions for using each of the program's functions
        private void bt_Help_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Find Words: enter a sequence of characters you wish to find in the String field, and enter a word length, a range of word lengths, or an asterisk for any word length.\n\nFind Anagrams: Enter any word in the String field to find words which are made up of the same letters.\n\nFind Wildcard: Enter a sequence of characters in the String field, and place a '.' to replace it with any letter, or place an '*' to replace it with any sequence of letters. Enter a specific word length or a range of lengths in the Length field.\n\nWord Ladder: enter any 2 words of equal length sparated by a space, '-', or ',' to try to find a path from one word to the other by changing only 1 letter at a time.");
        }
    }       
}
