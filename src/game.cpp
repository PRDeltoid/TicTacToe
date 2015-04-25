#include "game.hpp"

Game::Game() {
    game_state = RUNNING;
    current_player = 1;
    last_played_pos.x = 0;
    last_played_pos.y = 0;
    moves_made = 0;
    window.create(sf::VideoMode(BOARDSIZE*SQUARESIZE, BOARDSIZE*SQUARESIZE), "TIC TAC TOE");
    //Initialize the board
    for(int i=0;i<BOARDSIZE*BOARDSIZE;i++) {
        this->spaces[i].type = 'n';
        this->spaces[i].shape.setFillColor(sf::Color::White);
        this->spaces[i].shape.setPosition((i%BOARDSIZE)*SQUARESIZE, (i/BOARDSIZE)*SQUARESIZE);
        this->spaces[i].shape.setSize(sf::Vector2f(SQUARESIZE, SQUARESIZE));
    }
}
void Game::Loop() {
    while (this->window.isOpen())
    {
        sf::Event event;
        while (this->window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                //Event when a user presses the left mouse while the game is running.
                if (event.mouseButton.button == sf::Mouse::Left && game_state == RUNNING) { 
                    //Determine where user clicked. Change the tile's ownership that they clicked
                    this->Click(event.mouseButton.x, event.mouseButton.y);
                    //Check for a draw
                    if(this->Check_Stalemate() == true) {
                        game_state = FINISHED;
                        std::cout << "Draw found" << std::endl;
                    }
                    //Check for a win
                    if(this->Check_For_Winner() == true) {
                        game_state = FINISHED;
                        std::cout << "Winner: " << current_player << std::endl;
                    }
                    //Switch players if game is not over
                    if(game_state != FINISHED)
                        this->Switch_Player();
                }
            }
        }
        this->Clear();  //Clear the board
        this->Draw();
        this->Show();
    }
}

void Game::Show() {
    this->window.display();
}

void Game::Clear() {
    this->window.clear();
}

void Game::Draw() {
    if(game_state == RUNNING || game_state == FINISHED) {
        Draw_Game();
    } else if(game_state == MENU) {
        Draw_Menu();
    }
}

void Game::Draw_Menu() {

}

void Game::Draw_Game() {
    for(int i=0;i<BOARDSIZE*BOARDSIZE;i++) {
        this->window.draw(spaces[i].shape);
    }
    if(game_state == FINISHED) {
        //Make the Winner String
        std::stringstream winner_stream;
        winner_stream << "Winner: Player " << current_player;
        std::string winner_string= winner_stream.str();
        //Load font
        sf::Font font;
        font.loadFromFile("arial.ttf");
        //Create Text objects and set properties
        sf::Text gameover_text("Game Over", font);
        gameover_text.setCharacterSize(30);
        gameover_text.setColor(sf::Color::Black);
        gameover_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE*1.5);
        sf::Text winner_text(winner_string, font);
        winner_text.setCharacterSize(30);
        winner_text.setColor(sf::Color::Black);
        winner_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE*2);
        //Draw the text on the screen
        this->window.draw(gameover_text);
        this->window.draw(winner_text);
    }
}

char Game::Check_Location(int x, int y) {
    return spaces[x+y*BOARDSIZE].type;
}

void Game::Set_Location(char type, int x, int y) {
    int location = x+y*BOARDSIZE;
    //Set the space's type (x, o or n)
    spaces[location].type = type;

    //Determine color depending on the type
    if(type == 'x')
        spaces[location].shape.setFillColor(sf::Color::Yellow);
    else if(type == 'o')
        spaces[location].shape.setFillColor(sf::Color::Red);
    else 
        spaces[location].shape.setFillColor(sf::Color::White);
}

void Game::Click(int mouse_x, int mouse_y) {
    int x = mouse_x/SQUARESIZE;
    int y = mouse_y/SQUARESIZE;
    if(this->Check_Location(x, y) == 'n') {
        if(current_player==1)
            this->Set_Location('x', x, y);
        else 
            this->Set_Location('o', x, y);
        last_played_pos.x = x;
        last_played_pos.y = y;
    }
}

bool Game::Check_Stalemate() {
    moves_made++;
    bool stalemate = false;
    if(moves_made==BOARDSIZE*BOARDSIZE)
        stalemate = true;
    return stalemate;
}

bool Game::Check_For_Winner() {
    char check_char;
    //Determine player to check
    if(current_player==1)
        check_char = 'x';
    else 
        check_char = 'o';

    //Check horizontal
    for(int i=0;i<BOARDSIZE;i++) {
        if(Check_Location(i, last_played_pos.y) != check_char)
            break;
        if(i == BOARDSIZE-1)
            return true;
    }
    //Check vertical
    for(int i=0;i<BOARDSIZE;i++) {
        if(Check_Location(last_played_pos.x, i) != check_char)
            break;
        if(i == BOARDSIZE-1)
            return true;
    }
    //Check diag if played in a diag position
    if(last_played_pos.x == last_played_pos.y)  {
        for(int i=0; i<BOARDSIZE;i++) {
            if(Check_Location(i, i) != check_char)
                break;
            if(i == BOARDSIZE-1)
                return true;
        }
    }
    //Check anti-diag
    for(int i=0; i<BOARDSIZE;i++) {
        if(Check_Location(i,(BOARDSIZE-1)-i) != check_char)
            break;
        if(i == BOARDSIZE-1){
            return true;
        }
    }
    
    return false;
}

void Game::Switch_Player() {
    if(current_player == 1)
        current_player = 2; 
    else 
        current_player = 1;
}
