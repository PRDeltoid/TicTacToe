#ifndef GAME_H
#define GAME_H

#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

//Height or Width of Window Size (Window is Square)
const int WINDOWSIZE = 800;
//Height or Width of board size
const int BOARDSIZE = 3;
//Size of each tictactoe square
const int SQUARESIZE = WINDOWSIZE/BOARDSIZE;

struct Space {
    sf::RectangleShape shape;
    char type;
};

struct Pos {
    int x;
    int y;
};

enum Valid {VALID, INVALID};
enum State {MENU, ONEPLAYER, TWOPLAYER, WIN, STALEMATE};

class Game
{
    public:
        Game();
        void Initialize_Board();
        void Initialize_Text();
        void Restart();
        void Loop();
        void Show();
        void Clear();
        void Draw();
        void Draw_Menu();
        void Draw_Game();
        void Draw_Gameover();
        void Draw_Stalemate();
        char Check_Location(int x, int y);
        void Set_Location(char type, int x, int y);
        void Click(int mouse_x, int mouse_y);
        void Game_Click(int mouse_x, int mouse_y);
        void Finished_Click(int mouse_x, int mouse_y);
        void Menu_Click(int mouse_x, int mouse_y);
        bool Check_Stalemate();
        bool Check_For_Winner();
        void Switch_Player();
    private:
        Space spaces[BOARDSIZE*BOARDSIZE];
        sf::RenderWindow window;
        int current_player;
        Pos last_played_pos;
        int moves_made;
        State game_state;
        Valid last_move;
        sf::Font font;
        sf::Text oneplayer_text;
        sf::Text twoplayer_text;
        sf::Text gameover_text;
        sf::Text winner_text;
        sf::Text stalemate_text;
        sf::Text menu_title_text;
        sf::Text restart_text;
};


#endif
