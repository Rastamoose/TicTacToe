#include "stdafx.h"
#include <string>
#include <unordered_map>
#include "engine/View.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"         
#include "engine/Button.h"
#include "game/Board.h"
#include "game/Scoreboard.h"
#include "game/Login.h"
#include "game/Game.h"

Game::Game(ResourceManager* presources, const Point& screenSize, bool isAI):       
    changeView(""), font("Little Orion"), resources(presources), colours(TextColours()), 
    board(Board(Point(80,80),550,10)), sb(Scoreboard(presources, Rect(775,175,480,300), font)),
    login(Login(resources, Point(screenSize.x/2,screenSize.y/2), font)), state(GameState(isAI)), stats(Stats()), names(Names()),
    returnButton(Button(resources->img, "./res/img/game/return", 1, 8, Point(1184,30))){

    // load fonts and text
    resources->ttf->loadText("draw", font, "Draw!", colours.tie);
    updateNames();          // will generate all text related to names
    sb.updateScores(0,0,0);

    state.turn = P1;
}

void Game::takeTurnPlayer(InputManager* inputs, bool& success){
    if(inputs->isClicked(SDL_BUTTON_LEFT)){
        // get point clicked on board
        Point clicked = board.getMousePos(inputs->getMousePos());
        // if clicked in a valid board position, then try to place on board
        if(clicked.x != -1 && clicked.y != -1){
            // setBox determines whether piece can be put there
            success = board.setBox(clicked, state.turn);
        }
    }
}

void Game::resetGame(){
    state.gameOver = false;
    state.gameRunning = true;
    state.winner = NONE;
    state.turn = P1;
    board.setBorder(state.turn);
    board.resetGrid();
}

void Game::gameOver(){
    state.gameOver = true;
    state.gameEnd = true;
    state.gameRunning = false;
    stats.turnNo = 0;
}

void Game::updateScores(){
    // update winner score
    if(state.winner==P1){
        stats.p1Wins += 1;
    }else if(state.winner==P2){
        stats.p2Wins += 1;
    }else if(state.winner==TIE){
        stats.ties += 1;
    }

    sb.updateScores(stats.p1Wins, stats.ties, stats.p2Wins);
}

void Game::updateText(DisplayManager* display){
    Point textPos = Point(80,5);
    
    // wins
    if(state.winner==P1){
        display->render(resources->ttf->getText("x_win"), textPos);
    } else if(state.winner==P2){
        display->render(resources->ttf->getText("o_win"), textPos);
    } else if(state.winner==TIE){
        display->render(resources->ttf->getText("draw"), textPos);
    }
    // game continuing
    else if(state.turn==P1){
        display->render(resources->ttf->getText("x_turn"), textPos);
    } else{
        display->render(resources->ttf->getText("o_turn"), textPos);
    }
}

void Game::updateTurn(InputManager* inputs){
    bool success = false;

    // if it is AI turn
    if(state.againstAI && state.turn == P2){
        board.takeTurnAI(state.turn, success);
    } else{             // else take player turn
        takeTurnPlayer(inputs, success);
    }

    // if play has been made, toggle turn and change border colour
    if(success){
        state.turn = (state.turn==P1) ? P2:P1;
        stats.turnNo++;
        board.setBorder(state.turn);
    }
}

void Game::updateWinner(){
    // only check for win if there have been 5 moves (i.e P1 has done 3 moves)
    if(stats.turnNo>=5){
        state.winner = board.winCheck();
    }
    // if game has ended update borders and gameOver state
    if(state.winner!=NONE){
        // if there is a tie, set border to tie
        if(state.winner == TIE){
            board.setBorder(TIE);
        } else{         // else a player has won, so set to win
            board.setBorder(WIN);
        }
        gameOver();
    }
}

void Game::updateNames(){
    // phrases that come after name
    std::string turn = "'s turn";
    std::string wins = " wins!";

    // update turn and wins
    resources->ttf->loadText("x_turn", font, (names.x+turn).c_str(), colours.x);
    resources->ttf->loadText("o_turn", font, (names.o+turn).c_str(), colours.o);
    resources->ttf->loadText("x_win", font, (names.x+wins).c_str(), colours.win);
    resources->ttf->loadText("o_win", font, (names.o+wins).c_str(), colours.win);

    // update scoreboard
    sb.updateTitle(names.x, names.o);
}

// update scoreboard border colour based on who has most wins
void Game::updateSBBorder(){
    // if p1 has most wins
    if(stats.p1Wins > stats.p2Wins && stats.p1Wins > stats.ties){
        sb.setBorderColour(colours.x);
    // if p2 has most wins
    } else if(stats.p2Wins > stats.p1Wins && stats.p2Wins > stats.ties){
        sb.setBorderColour(colours.o);
    // if there are more ties or if p1 and p2 score is equal
    } else{
        sb.setBorderColour(colours.tie);
    }
}

void Game::updateBoard(DisplayManager* display){
    // reset game if user clicks screen after game ends
    if(state.gameOver && display->inputs->isClicked(SDL_BUTTON_LEFT)){
        resetGame();
    }

    // make sure gameEnd is only true for 1 loop
    if(state.gameEnd){
        updateScores();
        updateSBBorder();
        state.gameEnd = false;    
    }    

    // only update board if game ongoing
    if(state.gameRunning){
        updateTurn(display->inputs);
        updateWinner();
    }  
    
    sb.render(display);
    updateText(display);
    board.render(display, state.gameOver);
}

void Game::updateLogin(DisplayManager* display){
    // if names are nothing then 
    if(names.x=="X"){
        login.getUserName(display->inputs, "Player 1", names.x);
    // if o is ai then set name to bot
    } else if(state.againstAI && names.o=="O"){
        names.o = "bot";
    } else if(names.o=="O"){
        login.getUserName(display->inputs, "Player 2", names.o);
    } else{
        state.loggingIn = false;
        state.gameRunning = true;
        updateNames();
    }

    login.render(display);
}

// blur background by placing translucent rect over it
void Game::blurBackground(DisplayManager* display){
    Point screenSize = display->getSize();
    display->drawFillRect(Rect(0,0,screenSize.x,screenSize.y), Colour(0,0,0,150));
}

void Game::setName(int player, const std::string name){
    if(player==P1){
        names.x = name;
    } else if(player==P2){
        names.o = name;
    }

    updateNames();
}

std::string Game::update(DisplayManager* display, double dt){
    changeView = "";
    
    updateBoard(display);
    if(state.loggingIn){
        blurBackground(display);
        updateLogin(display);
    }
    returnButton.update(display->inputs, dt);
    returnButton.render(display);

    // handle view changes
    if (display->inputs->isPressed(SDLK_ESCAPE) || returnButton.isPressed){
        changeView = "mainmenu";
    }
    return changeView;
}

