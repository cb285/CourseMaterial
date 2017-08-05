# File: Makefile
# Class: CS 215                           Instructor: Dr. Don Roberts
# Assignment: Machine Problem 5           Date assigned: 3/23/16
# Programmer: Clayton Brutus              Date completed: 3/30/16


editor2 : editor2.o document2.o dnode2.o
	g++ -Wall -o editor2 editor2.o document2.o dnode2.o

editor.o : editor2.cpp document2.h
	g++ -Wall -c editor2.cpp

document.o : document2.cpp document2.h
	g++ -Wall -c document2.cpp

dnode2.o : dnode2.cpp dnode2.h
	g++ -Wall -c dnode2.cpp

archive :
	tar cvf submit.tar Makefile dnode2.h dnode2.cpp document2.h document2.cpp editor2.cpp
