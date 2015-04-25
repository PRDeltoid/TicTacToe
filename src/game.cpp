#include "game.hpp"

//Ctor
Game::Game() {
    //Initialize the game logic variables
    game_state = MENU;
    last_move = INVALID;
    current_player = 1;
    last_played_pos.x = 0;
    last_played_pos.y = 0;
    moves_made = 0;
    //Creates the game window
    window.create(sf::VideoMode(BOARDSIZE*SQUARESIZE, BOARDSIZE*SQUARESIZE), "TIC TAC TOE");
    //Initialize the board
    Initialize_Board();
    Initialize_Text();
}

//Sets up the board to be completely blank
void Game::Initialize_Board() {
    for(int i=0;i<BOARDSIZE*BOARDSIZE;i++) {
        this->spaces[i].type = 'n';
        this->spaces[i].shape.setFillColor(sf::Color::White);
        this->spaces[i].shape.setPosition((i%BOARDSIZE)*SQUARESIZE, (i/BOARDSIZE)*SQUARESIZE);
        this->spaces[i].shape.setSize(sf::Vector2f(SQUARESIZE, SQUARESIZE));
    }
}

//Creates all the text objects and sets their properties
void Game::Initialize_Text() {
    //Load font
    font.loadFromFile("arial.ttf");

    //Create MENU Text objects and set properties
    menu_title_text.setString("Tic Tac Toe");
    menu_title_text.setFont(font);
    menu_title_text.setCharacterSize(30);
    menu_title_text.setColor(sf::Color::White);
    menu_title_text.setOrigin(menu_title_text.getLocalBounds().width/2, menu_title_text.getLocalBounds().height/2);
    menu_title_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE);

    oneplayer_text.setString("Play Single Player");
    oneplayer_text.setFont(font);
    oneplayer_text.setCharacterSize(20);
    oneplayer_text.setColor(sf::Color::White);
    oneplayer_text.setOrigin(oneplayer_text.getLocalBounds().width/2, oneplayer_text.getLocalBounds().height/2);
    oneplayer_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE*1.5);

    twoplayer_text.setString("Play Two Player");
    twoplayer_text.setFont(font);
    twoplayer_text.setCharacterSize(20);
    twoplayer_text.setColor(sf::Color::White);
    twoplayer_text.setOrigin(twoplayer_text.getLocalBounds().width/2, twoplayer_text.getLocalBounds().height/2);
    twoplayer_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE*2);

    //Create FINISHED text objects and properties
    gameover_text.setString("Game Over");
    gameover_text.setFont(font);
    gameover_text.setCharacterSize(30);
    gameover_text.setColor(sf::Color::Black);
    gameover_text.setOrigin(gameover_text.getLocalBounds().width/2, gameover_text.getLocalBounds().height/2);
    gameover_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE);

    winner_text.setString("Winner! Player X");
    winner_text.setFont(font);
    winner_text.setCharacterSize(20);
    winner_text.setColor(sf::Color::Black);
    winner_text.setOrigin(winner_text.getLocalBounds().width/2, winner_text.getLocalBounds().height/2);
    winner_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE*1.5);

    restart_text.setString("Restart?");
    restart_text.setFont(font);
    restart_text.setCharacterSize(20);
    restart_text.setColor(sf::Color::Black);
    restart_text.setOrigin(restart_text.getLocalBounds().width/2, restart_text.getLocalBounds().height/2);
    restart_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE*2);

    //Create STALEMATE text objects + props
    stalemate_text.setString("The game was a draw");
    stalemate_text.setFont(font);
    stalemate_text.setCharacterSize(20);
    stalemate_text.setColor(sf::Color::Black);
    stalemate_text.setOrigin(stalemate_text.getLocalBounds().width/2, stalemate_text.getLocalBounds().height/2);
    stalemate_text.setPosition((SQUARESIZE*BOARDSIZE)/2, SQUARESIZE*1.5);
}

//Re-initializes all starting values, so a new game can be played
void Game::Restart() {
        Initialize_Board();
        current_player = 1;
        game_state = TWOPLAYER;
        moves_made = 0;
        last_move = INVALID;
}

//Main Game loop. 
void Game::Loop() {
    while (this->window.isOpen())
    {
        sf::Event event;
        while (this->window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                //Send click pos to the Click handler, which determines which Click function to use
                this->Click(event.mouseButton.x, event.mouseButton.y);
                //If the game is running, check for gameover conditions (win, stalemate) and switch players if needed
                if (game_state == ONEPLAYER || game_state == TWOPLAYER) { 
                    //Check for a stalemate 
                    if(this->Check_Stalemate() == true) {
                        game_state = STALEMATE;
                        std::cout << "Draw found" << std::endl;
                    }
                    //Check for a win
                    if(this->Check_For_Winner() == true) {
                        game_state = WIN;
                        std::cout << "Winner: " << current_player << std::endl;
                    }
                    //Switch players if game is not over and the last move was valid (ie. it was not on an already-played space)
                    if(game_state == TWOPLAYER && last_move == VALID)
                        this->Switch_Player(); 
                }
            }
        }
        this->Clear();  //Clear the board
        this->Draw();   //Draw, depending in current game state
        this->Show();   //Show the drawing
    }
}

//Render the screen (drawing is done before this)
void Game::Show() {
    this->window.display();
}

//Clear the screen
void Game::Clear() {
    this->window.clear();
}

//Draw handler. Determines which Draw function to use depending on game state
void Game::Draw() {
    if(game_state == ONEPLAYER || game_state == TWOPLAYER) {
        Draw_Game();
    } else if(game_state == WIN) {
        Draw_Game();
        Draw_Gameover();
    } else if(game_state == STALEMATE) {
        Draw_Game();
        Draw_Stalemate();
    } else if(game_state == MENU) {
        Draw_Menu();
    }
}

//Draws the main menu
void Game::Draw_Menu() {
    //Draw the text on the screen
    this->window.draw(menu_title_text);
    this->window.draw(oneplayer_text);
    this->window.draw(twoplayer_text);

}

//Draws the game board
void Game::Draw_Game() {
    for(int i=0;i<BOARDSIZE*BOARDSIZE;i++) {
        this->window.draw(spaces[i].shape);
    }
}

//Draws the Gameover by Win screen, with the game board in the background
void Game::Draw_Gameover() {
    //Make the Winner String
    std::stringstream winner_stream;
    winner_stream << "Winner! Player " << current_player;
    std::string winner_string= winner_stream.str();
    //Update the winner_text object with the current winner
    winner_text.setString(winner_string);
    //Draw the text on the screen
    this->window.draw(gameover_text);
    this->window.draw(winner_text);
    this->window.draw(restart_text);
}

//Draws the Gameover by Stalemate screen, with the game board in the background
void Game::Draw_Stalemate() {
    //Draw the text objets
    this->window.draw(gameover_text);
    this->window.draw(stalemate_text);
    this->window.draw(restart_text);
}

//Check the space type (x, o, or n) given an input coordinate (using tictactoe grid, not pixel grid).
char Game::Check_Location(int x, int y) {
    return spaces[x+y*BOARDSIZE].type;
}

//Sets the space type given an input coordinate and space type to set to. 
void Game::Set_Location(char type, int x, int y) {
    //Determine the index in the single-dimension array
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

//Click handler
void Game::Click(int mouse_x, int mouse_y) {
    if(game_state == ONEPLAYER || game_state == TWOPLAYER) {
        Game_Click(mouse_x, mouse_y);
    } else if(game_state == MENU) {
        Menu_Click(mouse_x, mouse_y);
    } else if(game_state == WIN || game_state == STALEMATE) {
        Finished_Click(mouse_x, mouse_y);
    }
}

//Handles clicks while the game is running
void Game::Game_Click(int mouse_x, int mouse_y) {
    int x = mouse_x/SQUARESIZE;
    int y = mouse_y/SQUARESIZE;
    last_move = INVALID;
    if(this->Check_Location(x, y) == 'n') {
        last_move = VALID;
        moves_made++; //A legit move has been made
        if(current_player==1)
            this->Set_Location('x', x, y);
        else 
            this->Set_Location('o', x, y);
        last_played_pos.x = x;
        last_played_pos.y = y;
    }
}

//Handles click on the Gameover screen
void Game::Finished_Click(int mouse_x, int mouse_y) {
    if(mouse_x > restart_text.getGlobalBounds().left &&
       mouse_x < restart_text.getGlobalBounds().left + restart_text.getLocalBounds().width &&
       mouse_y > restart_text.getGlobalBounds().top &&
       mouse_y < restart_text.getGlobalBounds().top + restart_text.getLocalBounds().height) {
        std::cout << "Restart clicked" << std::endl;
        this->Restart();
    }

}

//Handles clicks on the Main Menu screen
void Game::Menu_Click(int mouse_x, int mouse_y) {
    if(mouse_x > oneplayer_text.getGlobalBounds().left &&
       mouse_x < oneplayer_text.getGlobalBounds().left + oneplayer_text.getLocalBounds().width &&
       mouse_y > oneplayer_text.getGlobalBounds().top &&
       mouse_y < oneplayer_text.getGlobalBounds().top + oneplayer_text.getLocalBounds().height) {
        game_state = ONEPLAYER;
    } else if(mouse_x > twoplayer_text.getGlobalBounds().left &&
       mouse_x < twoplayer_text.getGlobalBounds().left + twoplayer_text.getLocalBounds().width &&
       mouse_y > twoplayer_text.getGlobalBounds().top &&
       mouse_y < twoplayer_text.getGlobalBounds().top + twoplayer_text.getLocalBounds().height) {
        game_state = TWOPLAYER;
    }
}

//Checks for a stalemate by comparing the total moves made to the total number of squares
bool Game::Check_Stalemate() {
    bool stalemate = false;
    if(moves_made==BOARDSIZE*BOARDSIZE)
        stalemate = true;
    return stalemate;
}

//Checks for a winner
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

//Switches the current player
void Game::Switch_Player() {
    if(current_player == 1)
        current_player = 2; 
    else 
        current_player = 1;
}
