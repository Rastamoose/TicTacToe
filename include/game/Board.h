#ifndef BOARD_H
#define BOARD_H

#include "stdafx.h"
#include <string>
#include <SDL2/SDL.h>
#include <limits>
#include <unordered_map>
#include "engine/Utils.h"
#include "engine/DisplayManager.h"
#include "engine/InputManager.h"

#define NONE 0
#define P1 1
#define P2 2
#define AI 2
#define TIE 3
#define WIN 4

// move taketurnai to game method

// game colours
struct BoardColours{
    Colour grid;
    Colour border;  
    Colour winLine;
    Colour x;
    Colour o;
    Colour tie;
    Colour win;
    // SET COLOUR VALUES HERE Colour
    BoardColours() : grid(Colour(129,169,168,255)), border(Colour(228, 87, 46,255)), winLine(Colour(255,165,0,255)),
                     x(Colour(209,84,84,255)), o(Colour(155,184,184,255)), tie(Colour(216,222,233,255)), win(Colour(255,165,0,255)){}
};

// line across the board when player wins
struct WinLine{
    Point start;
    Point end;

    WinLine() : start(Point(0,0)), end(Point(0,0)){}
};

class Board{
    // grid slot shows 1 for X, 2 for O and 0 for none
    int rows;
    Point boardPos;
    int boardW;
    BoardColours colours;
    int drawThickness;
    WinLine winLine;
    std::vector<std::vector<uint8_t> > grid;
    std::unordered_map<int,int> minimaxScores;

    void getAvailableMoves(std::vector<Point>& output);
    int minimax(int depth, bool isMaximising);
    void drawGridLines(DisplayManager* display);
    void drawBorders(DisplayManager* display);
    void drawShapes(DisplayManager* display);
    void renderWin(DisplayManager* display);
    void printGrid(std::vector<std::vector<uint8_t>>& pGrid);
    

    public:
    Board(const Point& pos, int width, int pdrawThickness);
    // Return mousePos as a grid co-ord or (-1,-1) for no click
    Point getMousePos(Point mousePos);
    void takeTurnAI(int turn, bool& success);
    // return win state as NONE, TIE or winner
    int winCheck();
    void render(DisplayManager* display, bool gameOver);
    void resetGrid();
    // return true if could set box -i.e box was empty before, else false
    bool setBox(const Point& box, int turn);
    // sets colour based on turn
    void setBorder(int turn);
};

#endif