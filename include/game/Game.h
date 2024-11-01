#ifndef GAME_H
#define GAME_H

#include <string>
#include <unordered_map>
#include "engine/View.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"         
#include "engine/Button.h"
#include "game/Board.h"
#include "game/Scoreboard.h"
#include "game/Login.h"

#define NONE 0
#define P1 1
#define P2 2
#define TIE 3
#define WIN 4

// colours set here to avoid clutter in .cpp file
struct TextColours{
    Colour x;
    Colour o;
    Colour win;
    Colour tie;

    TextColours():x(Colour(209,84,84,255)), o(Colour(155,184,184,255)), win(Colour(255,165,0,255)), tie(Colour(255,255,255,255)){}
};

// store names
struct Names{
    std::string x;
    std::string o;

    // default names to "x" and "o"
    Names():x("X"), o("O"){}
};

// state of different game variables
struct GameState{
    bool loggingIn;       // is game getting user names
    bool gameRunning;
    bool gameOver;
    // gameEnd is a trigger, meaning it only lasts for one loop
    bool gameEnd;
    bool againstAI;         // is game against ai
    int turn;
    int winner;     
    // Constructor
    GameState(bool isAI) : loggingIn(true), gameRunning(false), gameOver(false), gameEnd(false), againstAI(isAI), turn(NONE), winner(NONE){}
};

// numerical stats about running game
struct Stats{
    int p1Wins;
    int p2Wins;
    int ties;
    int turnNo;     // number of turns that have passed
    // Constructor
    Stats() : p1Wins(0), p2Wins(0), ties(0), turnNo(0){}
};



class Game : public View{
    std::string changeView;
    std::string font;
    ResourceManager* resources;
    TextColours colours;
    Board board;   
    Scoreboard sb;
    Login login;
    GameState state;
    Stats stats;
    Names names;
    Button returnButton;

    void takeTurnPlayer(InputManager* inputs, bool& success);
    void resetGame();
    void gameOver();
    void updateScores();
    void updateText(DisplayManager* display);
    void updateTurn(InputManager* inputs);
    void updateWinner();
    // update all atributes associated with a name after name change
    void updateNames();
    // update scoreboard border colour based on who has most wins
    void updateSBBorder();
    // group all board update logic
    void updateBoard(DisplayManager* display);
    // group all login update logic
    void updateLogin(DisplayManager* display);
    void blurBackground(DisplayManager* display);
    // set player names - player is P1, P2 and name is string
    void setName(int player, const std::string name);

    public:
    Game(ResourceManager* resources, const Point& screenSize, bool isAI);
    std::string update(DisplayManager* display, double dt);

};

#endif