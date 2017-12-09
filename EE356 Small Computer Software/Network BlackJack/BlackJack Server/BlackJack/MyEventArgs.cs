using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class MyEventArgs : EventArgs
{
    public string MyEventString { get; set; }

    public MyEventArgs(string myString)
    {
        this.MyEventString = myString;
    }
}
