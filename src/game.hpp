#ifndef GAME_H
#define GAME_H

#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

#define BOARDSIZE 3
#define SQUARESIZE 200

struct Space {
    sf::RectangleShape shape;
    char type;
};

struct Pos {
    int x;
    int y;
};

enum State {MENU, RUNNING, FINISHED};

class Game
{
    public:
        Game();
        void Loop();
        void Show();
        void Clear();
        void Draw();
        void Draw_Menu();
        void Draw_Game();
        char Check_Location(int x, int y);
        void Set_Location(char type, int x, int y);
        void Click(int mouse_x, int mouse_y);
        bool Check_Stalemate();
        bool Check_For_Winner();
        void Switch_Player();
    private:
        Space spaces[9];
        sf::RenderWindow window;
        int current_player;
        Pos last_played_pos;
        int moves_made;
        State game_state;
};


#endif
