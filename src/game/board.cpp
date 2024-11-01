#include "stdafx.h"
#include <string>
#include <SDL2/SDL.h>
#include <limits>
#include "engine/DisplayManager.h"
#include "engine/Utils.h"
#include "engine/InputManager.h"
#include "game/Board.h"

#include <iostream>

Board::Board(const Point& pos, int width, int pdrawThickness):
    rows(3), boardPos(pos), boardW(width), colours(BoardColours()),  
    drawThickness(pdrawThickness), winLine(WinLine()){
    
    // add minimaxScores to map
    minimaxScores.emplace(P1,-10);
    minimaxScores.emplace(P2,10);
    minimaxScores.emplace(TIE,0);
        
    // fill grid in with 0s
    for(int i=0; i<rows; i++){
        grid.push_back(std::vector<uint8_t>{});
        for(int j=0; j<rows; j++){
            grid.at(i).push_back(0);
        }
    }
}

void Board::getAvailableMoves(std::vector<Point>& output){
    // go through potential spots
    for(int i=0;i<rows;i++){
        for(int j=0;j<rows;j++){
            // if spot is available
            if(grid.at(j).at(i)==NONE){
                output.push_back(Point(i,j));
            }
        }
    }
}

int Board::minimax(int depth, bool isMaximising){
    int result = winCheck();
    // if this move results in end of game, 
    // it is end of this minimax recursion, return score
    if(result != NONE){
        return minimaxScores[result];
    }
    if(isMaximising){
        int bestScore = -1000;
        std::vector<Point> availableSlots = {};
        // get potential spots
        getAvailableMoves(availableSlots);
        // go through available slots
        for(Point slot:availableSlots){
            grid.at(slot.y).at(slot.x) = AI;    // take turn here
            // evaluate score of player (minimising player responding to this move)
            int score = minimax(depth+1, false);
            grid.at(slot.y).at(slot.x) = NONE;  // reset the position
            // if greater than bestScore, this is best move
            bestScore = std::max(score, bestScore);
        }
        return bestScore;
    // else if minimising player
    } else{
        int bestScore = 1000;
        std::vector<Point> availableSlots = {};
        // get potential spots
        getAvailableMoves(availableSlots);
        // go through available slots
        for(Point slot:availableSlots){
            grid.at(slot.y).at(slot.x) = P1;    // take turn here
            // evaluate score of ai (maximising player responding to this move)
            int score = minimax(depth+1, true);
            grid.at(slot.y).at(slot.x) = NONE;  // reset the position
            // if greater than bestScore, this is best move
            bestScore = std::min(score, bestScore);
        }
        return bestScore;     
    }
}


void Board::drawGridLines(DisplayManager* display){
    int rowW = boardW/rows;  // width of a row in px

    // draw lines before all rows except first
    for(int i=1; i<rows; i++){
        int y = boardPos.y+(i*rowW);
        Point startPos = Point(boardPos.x, y);
        Point endPos = Point(boardPos.x+boardW, y);
        display->drawThickLine(startPos, endPos, drawThickness, colours.grid);
    }

    // draw lines before all cols except first
    for(int i=1; i<rows; i++){
        int x = boardPos.x+(i*rowW);
        Point startPos = Point(x, boardPos.y);
        Point endPos = Point(x, boardPos.y+boardW);
        display->drawThickLine(startPos, endPos, drawThickness, colours.grid);
    }
}

void Board::drawBorders(DisplayManager* display){
    int borderW = drawThickness/2;
    // add padding of borderW on each side
    Rect rect = Rect(boardPos.x-borderW, boardPos.y-borderW, boardW+borderW*2, boardW+borderW*2);
    display->drawThickRect(rect, borderW, colours.border);
}

void Board::drawShapes(DisplayManager* display){
    int rowW = boardW/rows;  // width of a row in px
    int padding = 15;
    int shapeW = rowW - padding;      // create gap between rows/cols and shapes

    // go through each individual shape
    for(int i=0;i<rows;i++){
        for(int j=0;j<rows;j++){
            Point box = Point(boardPos.x+padding+(j*rowW), boardPos.y+padding+(i*rowW));
            // if x
            if(grid.at(i).at(j)==1){
                Point end = Point(box.x-padding, box.y-padding);
                // draw cross
                display->drawThickLine(box,Point(end.x+shapeW, end.y+shapeW), drawThickness, colours.x);
                display->drawThickLine(Point(box.x,box.y+shapeW-padding),Point(end.x+shapeW,end.y+padding), drawThickness, colours.x);
            } else if(grid.at(i).at(j)==2){         // else if o
                int radius = (shapeW-2*padding)/2;
                display->drawThickCircle(Point(box.x+radius, box.y+radius), radius, drawThickness, colours.o);
            }
        }
    }
}

void Board::renderWin(DisplayManager* display){
    int rowW = boardW/rows;  // width of a row in px
    int halfW = rowW/2;
    Point realStart = Point(0,0);
    Point realEnd = Point(0,0);

    // check direction of line and convert to real coords accordingly
    // if invalid line (i.e game was a draw), then skip
    if(winLine.start.x==-1){
        return;
    
    // if winline is horizontal
    } else if(winLine.start.y==winLine.end.y){
        // make y centered in square and x reach edges
        realStart = Point(winLine.start.x*rowW+boardPos.x, winLine.start.y*rowW+boardPos.y+halfW);
        realEnd = Point((winLine.end.x+1)*rowW+boardPos.x, winLine.end.y*rowW+boardPos.y+halfW);
    
    // if winLine is vertical
    } else if(winLine.start.x==winLine.end.x){      
        // make x centered in square and y reach edges
        realStart = Point(winLine.start.x*rowW+boardPos.x+halfW, winLine.start.y*rowW+boardPos.y);
        realEnd = Point(winLine.end.x*rowW+boardPos.x+halfW, (winLine.end.y+1)*rowW+boardPos.y);    
    
    // if winLine is top left to bottom right
    } else if(winLine.start.x==winLine.start.y && winLine.end.x==winLine.end.y){             
        realStart = Point(winLine.start.x*rowW+boardPos.x, winLine.start.y*rowW+boardPos.y);
        realEnd = Point((winLine.end.x+1)*rowW+boardPos.x, (winLine.end.y+1)*rowW+boardPos.y); 

    // if winLine is top right to bottom left
    } else{
        realStart = Point((winLine.start.x+1)*rowW+boardPos.x, winLine.start.y*rowW+boardPos.y);
        realEnd = Point(winLine.end.x*rowW+boardPos.x, (winLine.end.y+1)*rowW+boardPos.y); 
    }


    display->drawThickLine(realStart, realEnd, drawThickness, colours.winLine);
}

Point Board::getMousePos(Point mousePos){
    // remove grid offset from mousePos
    mousePos.x -= boardPos.x;
    mousePos.y -= boardPos.y;
    
    // divide mousePos by row width to get grid pos
    int rowW = boardW/rows;
    mousePos.x /= rowW;
    mousePos.y /= rowW;

    // set mouse pos to (-1,-1) if off grid
    if(mousePos.x < 0 or mousePos.x > 2 or mousePos.y < 0 or mousePos.y > 2){
        mousePos.x = -1;
        mousePos.y = -1;
    }
    return mousePos;
}

void Board::takeTurnAI(int turn, bool& success){
    int bestScore = -1000;
    Point newPos = Point(-1,-1);       // where we are going to move to
    std::vector<Point> availableSlots = {};
    // get potential spots
    getAvailableMoves(availableSlots);
    // go through available slots
    for(Point slot:availableSlots){
        grid.at(slot.y).at(slot.x) = AI;    // take turn here
        // evaluate score of player (minimising player responding to this move)
        int score = minimax(0, false);
        grid.at(slot.y).at(slot.x) = NONE;  // reset the position
        // if greater than bestScore, this is best move
        if(score > bestScore){
            // update bestScore and best move
            bestScore = score;
            newPos = slot;
        }
    }

    // apply new turn to board
    grid.at(newPos.y).at(newPos.x) = AI;
    success = true;
}

int Board::winCheck(){
    // go through 1 (x), then 2 (o)
    for(int i=1;i<=2;i++){
        // check each row
        for(int row=0;row<rows;row++){
            bool win = true;
            // go through each col in the row, if not i then can't be win there
            for(int col=0;col<rows;col++){
                if(grid.at(row).at(col) != i){
                    win = false;
                }
            }
            // if all elements in row were i
            if(win==true){   
                winLine.start = Point(0, row);
                winLine.end = Point(2, row);
                return i;
            }
        }

        // check each col
        for(int col=0;col<rows;col++){
            bool win = true;
            // go through each row in the col, if not i then can't be win there
            for(int row=0;row<rows;row++){
                if(grid.at(row).at(col) != i){
                    win = false;
                }
            }
            // if all elements in row were i
            if(win==true){
                winLine.start = Point(col, 0);
                winLine.end = Point(col, 2);
                return i;
            }
        }

        // check diagonals
        // top left to bottom right
        bool win = true;
        for(int j=0;j<rows;j++){
            if(grid.at(j).at(j) != i){
                win = false;
            }
        }
        if(win==true){
            winLine.start = Point(0,0);
            winLine.end = Point(2,2);
            return i;
        }
        // top right to bottom left
        if(grid.at(2).at(0)==i && grid.at(1).at(1)==i && grid.at(0).at(2)==i){
            winLine.start = Point(2,0);
            winLine.end = Point(0,2);
            return i;
        }
    }

    // check for draw
    // if there are no empty boxes it is now a draw
    for(int i=0;i<rows;i++){
        for(int j=0;j<rows;j++){
            // if slot empty
            if(grid.at(i).at(j) == 0){
                // return game still running
                return NONE;
            }
        }
    }
    // tie, so make winline invalid
    winLine.start = Point(-1,-1);
    winLine.end = Point(-1,-1);    
    return TIE;
}

void Board::render(DisplayManager* display, bool gameOver){
    drawBorders(display);
    drawGridLines(display);
    drawShapes(display);
    // render winner line across board
    if(gameOver){
        renderWin(display);
    }
}

void Board::resetGrid(){
    for(int i=0;i<rows;i++){
        for(int j=0;j<rows;j++){
            grid.at(i).at(j) = 0;
        }
    }
}

bool Board::setBox(const Point& box, int turn){
    // only edit if empty
    if(grid.at(box.y).at(box.x)==NONE){
        // set grid box to 1 if x or 0 if o
        grid.at(box.y).at(box.x) = turn;
        return true;  
    }

    return false;
}

void Board::setBorder(int turn){
    // change colour based on turn
    switch(turn){
        case P1:
            colours.border = colours.x;
            break;
        case P2:
            colours.border = colours.o;
            break;
        case TIE:
            colours.border = colours.tie;
            break;
        case WIN:
            colours.border = colours.win;
            break;
    }
}

void Board::printGrid(std::vector<std::vector<uint8_t>>& pGrid){
    for(auto& row:pGrid){
        std::cout << std::endl;
        for(int element: row){
            std::cout << element << " ";
        }
    }
    std::cout << std::endl;
}
