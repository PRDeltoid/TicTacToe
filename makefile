all: TicTacToe

TicTacToe: main.o
	g++  main.o -o TicTacToe -LSFML/lib -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp -ISFML/include 

clean:
	rm *o main
