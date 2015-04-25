all: TicTacToe

TicTacToe: main.o game.o
	g++  obj/main.o -o TicTacToe -LSFML/lib -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp -o obj/main.o -ISFML/include 

game.o: src/game.cpp
	g++ -c src/game.cpp -o obj/game.o -ISFML/include

clean:
	del obj /q
	del TicTacToe.exe
