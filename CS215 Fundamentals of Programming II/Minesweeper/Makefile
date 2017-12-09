sweeper : sweeper.o sweepergrid.o sweepercell.o
	g++ -Wall -o sweeper sweeper.o sweepergrid.o sweepercell.o

sweeper.o : sweeper.cpp sweepergrid.h
	g++ -Wall -c sweeper.cpp

sweepergrid.o : sweepergrid.cpp sweepergrid.h
	g++ -Wall -c sweepergrid.cpp

sweepercell.o : sweepercell.cpp sweepercell.h
	g++ -Wall -c sweepercell.cpp

archive :
	tar cvf submit.tar sweepergrid.h sweepergrid.cpp sweeper.cpp Makefile
