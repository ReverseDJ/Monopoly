Monopoly.exe: main.o functions.o classes.o  makeCards.o makeBoard.o shuffle.o 
	g++ main.o functions.o classes.o makeCards.o makeBoard.o shuffle.o -o Monopoly.exe

main.o: main.cpp
	g++ -c main.cpp

functions.o: functions.cpp functions.h
	g++ -c functions.cpp

classes.o: classes.cpp classes.h
	g++ -c classes.cpp

makeCards.o: makeCards.cpp
	g++ -c makeCards.cpp

makeBoard.o: makeBoard.cpp
	g++ -c makeBoard.cpp

shuffle.o: shuffle.cpp
	g++ -c shuffle.cpp




clean:
	rm *.o

