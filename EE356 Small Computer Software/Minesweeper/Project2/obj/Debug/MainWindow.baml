   M S B A M L   `   `   `  ����C  ?Project2, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null#clr-namespace:Project2Project2  5   	       Project2.MainWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   (localclr-namespace:Project2     5   	     � Icon$  mine.ico��5   	    � Title$ Minesweeper��5	   	   $	��300��6   $	��335��6*   .��5
      � .+�5   
   � -   5         �Name$
 grd1��5      $	��170��6   $��	0,89,35,0q�6*   $	��Top=�6=   $��Right=�6U   $	��213��6q   	� 5   
   -   5         	�Name$ rdo_Easy��5      $
��Easy��6(   $
��Left=�67   $��	131,6,0,0q�6R   $	��Top=�6e    d� 	IsChecked$
 TrueY�6}    � ToolTip$ 9 x 9 Grid with 10 Mines��6�   	� 5   
   -   5        $ 
rdo_Interm��5      $��Intermediate��6*   $
��Left=�6A   $��
131,21,0,0q�6\   $	��Top=�6p   $  16 x 16 Grid with 40 Mines��6�   	� 5   
   -   5        $ rdo_Hard��5      $
��Hard��6(   $
��Left=�67   $��
131,36,0,0q�6R   $	��Top=�6f   $  20 x 24 Grid with 99 Mines��6~   �� 5   
   -   5         ��Name$ but_Restart��5      $��Restart��6&   $
��Left=�68   $��
131,56,0,0q�6S   $	��Top=�6g   $��75��6   6�   �� 5   
   -   5         ��Name$ txtBox_Score��5      $
��Left=�6(   $��20��6C   $��	18,56,0,0q�6O    �� TextWrapping$
 Wrap=�6b   $	��Top=�6v   $��40��6�   	 �� 
IsReadOnly	 . 
 �� TextAlignment$
 Center=�6�   $% Number of uncovered empty cells��6�   �� 5   
   -   5        $ txtBox_Time��5      $
��Left=�6'   $��20��6B   $��	65,56,0,0q�6N   $
 Wrap=�6a   $	��Top=�6u   $��40��6�   $
 Center=�6�   $# Time since start of this game��6�   �� 5   
   -   5         ��Name$ label_Score��5      $��Score��6%   $
��Left=�65   $��	17,30,0,0q�6P   $	��Top=�6c   $��40��6{   �� 5   
   -	   5        $ 
label_Time��5      $
��Time��6$   $
��Left=�63   $��	65,30,0,0q�6N   $	��Top=�6a   $��40��6y   �� 5   
   -
   5        $ 	but_About��5      $��About��6$   $
��Left=�64   $��10,6,0,0q�6O   $	��Top=�6a   $��50��6y   $��20��6�   6�   �� 5   
   -   5        $ but_Help��5      $
��Help��6#   $
��Left=�62   $��65,6,0,0q�6M   $	��Top=�6_   $��50��6w   $��20��6�   6�   5      5      