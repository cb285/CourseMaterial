   M S B A M L   `   `   `  ����D  @Project 5, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullclr-namespace:ClientClient  5   	       Client.MainWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   &localclr-namespace:Client     5   	   -   5          � Title$  Client��5   	   $	��350��6   $	��525��6%   .��5	      � .+�5
   
   �� -   5         ��Name$ listBox��5
      $
��Left=�6#   $	��200��6>   $��	10,10,0,0q�6K   $	��Top=�6^   $	��497��6v   �� 5   
   -   5         ��Name$ textBox_Message��5      $
��Left=�6+   $��23��6F   $��
10,215,0,0q�6R    �� TextWrapping$
 Wrap=�6f   $	��Top=�6z   $	��442��6�    J� RenderTransformOrigin$ 0.25,-0.435&�6�    � ToolTip$# Enter a message to send here.��6�   6�   �� 5   
   -   5         ��Name$ button_Send��5      $
��Send��6&   $
��Left=�65   $��457,215,0,0q�6P   $	��Top=�6e   $��50��6}   $��23��6�   $ Sends the message.��6�   �� 5   
   -   5        $ textBox_Name��5      $
��Left=�6(   $��23��6C   $��
10,286,0,0q�6O   $
 Wrap=�6c   $	��Top=�6w   $	��120��6�   $8 2Enter a username for yourself. Cannot be "server".��6�   �� 5   
   -   5        $ button_Connect��5      $��Connect��6)   $
��Left=�6;   $��310,286,0,0q�6V   $	��Top=�6k   $��75��6�   $��23��6�   $ Connects to the server.��6�   �� 5   
   -   5         ��Name$ 
label_Name��5      $
��Name��6$   $
��Left=�63   $��
10,260,0,0q�6N   $	��Top=�6b   �� 5   
   -   5        $ textBox_IPAddress��5      $
��Left=�6-   $��23��6H   $��135,286,0,0q�6T   $
 Wrap=�6i   $	��Top=�6}   $	��120��6�   $��	127.0.0.1��6�   $) #Enter the IP Address of the server.��6�   �� 5   
   -	   5        $ textBox_Port��5      $
��Left=�6(   $��23��6C   $��260,286,0,0q�6O   $
 Wrap=�6d   $	��Top=�6x   $��45��6�   $
��9090��6�   $# Enter the port of the server.��6�   �� 5   
   -
   5        $ label_IP��5      $��
IP Address��6"   $
��Left=�67   $��135,260,0,0q�6R   $	��Top=�6g   �� 5   
   -   5        $ 
label_Port��5      $
��Port��6$   $
��Left=�63   $��260,260,0,0q�6N   $	��Top=�6c   �� 5   
   -   5        $ button_Disconnect��5      $��
Disconnect��6,   $
��Left=�6A   $��390,286,0,0q�6\   $	��Top=�6q   $��75��6�   $��23��6�   $ Connects to the server.��6�   5      5      