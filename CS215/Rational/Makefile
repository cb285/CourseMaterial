# File: Makefile
# Class: CS 215                           Instructor: Dr. Don Roberts
# Assignment: Machine Problem 2           Date assigned: 2/5/16
# Programmer: Clayton Brutus              Date completed: 2/10/16

testrational : testrational.o rational.o
	g++ -Wall -o testrational testrational.o rational.o

testrational.o : testrational.cpp rational.h
	g++ -Wall -c testrational.cpp

rational.o : rational.cpp rational.h
	g++ -Wall -c rational.cpp

archive :
	tar cvf submit.tar rational.h rational.cpp testrational.cpp Makefile
