Monopoly.exe: main.o functions.o classes.o makeCards.o makeBoard.o shuffle.o botFunctions.o
	g++ main.o functions.o classes.o makeCards.o makeBoard.o shuffle.o botFunctions.o -o Monopoly.exe

main.o: main.cpp
	g++ -c main.cpp

functions.o: functions.cpp Makefile functions.h
	g++ -c functions.cpp

classes.o: classes.cpp Makefile functions.h
	g++ -c classes.cpp

makeCards.o: makeCards.cpp
	g++ -c makeCards.cpp

makeBoard.o: makeBoard.cpp
	g++ -c makeBoard.cpp

shuffle.o: shuffle.cpp
	g++ -c shuffle.cpp

botFunctions.o: botFunctions.cpp
	g++ -c botFunctions.cpp




clean:
	rm *.o

all:
	g++ -std=c++14 main.cpp functions.cpp classes.cpp makeCards.cpp makeBoard.cpp shuffle.cpp botFunctions.cpp -o Monopoly.exe

run:
	./Monopoly.exe
