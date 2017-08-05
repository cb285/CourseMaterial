# File: Makefile
# Class: CS 215                           Instructor: Dr. Don Roberts
# Assignment: Machine Problem 2           Date assigned: 2/5/16
# Programmer: Clayton Brutus              Date completed: 2/10/16

editor : editor.o document.o
	g++ -Wall -o editor editor.o document.o

editor.o : editor.cpp document.h
	g++ -Wall -c editor.cpp

document.o : document.cpp document.h
	g++ -Wall -c document.cpp

archive :
	tar cvf submit.tar document.h document.cpp editor.cpp Makefile
