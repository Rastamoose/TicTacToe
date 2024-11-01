#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "engine/Utils.h"
#include "engine/InputManager.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"
#include "engine/IMGHandler.h"

struct LoginColours{
    Colour bg;
    Colour border;
    Colour text;
    Colour textInput;
    Colour inputBox;
    Colour inputBoxBorder;

    LoginColours(): bg(Colour(59,66,82,255)), border(Colour(46,52,64,255)), 
                    text(Colour(0x88,0xC0,0xD0,255)), textInput(Colour(0xB4,0x8E,0xAD, 255)),
                    inputBox(Colour(229,233,240,255)), inputBoxBorder(Colour(216,222,233,255)){}
};

struct LoginPositions{
    private:
    Point size;          // whole box size | used for easier initialisation in member initialiser list
    public:
    Rect loginBox;         // the whole unit
    Rect inputBox;
    Point text;
    Point inputText;

    // takes pos of whole structure
    LoginPositions(Point pos): size(Point(720,225)), loginBox(Rect(pos.x-size.x/2,pos.y-size.y/2,size.x,size.y)), inputBox(Rect(loginBox.x+25,loginBox.y+120,size.x-50,75)),
                               text(Point((pos.x-size.x/2)+25,(pos.y-size.y/2)+15)), inputText(Point(inputBox.x+10,inputBox.y+5)){}
};

struct LoginState{
    bool loggingIn;    // requesting input
    bool loggedIn;      // input acquired

    LoginState(): loggingIn(true), loggedIn(false){}
};

class Login{
    ResourceManager* resources;
    LoginColours colours;
    LoginPositions pos;
    LoginState state;
    std::string font;
    std::string userName;       // name that user is addressed by
    std::vector<char> input;

    // returns true if input is confirmed
    bool getInput(InputManager* inputs);
    std::string getEnteredName();

    public:
    // pos will be centre of login menu
    Login(ResourceManager* presources, const Point& ppos, const std::string& pfont);
    void render(DisplayManager* display);
    // if logged in, return true and edits output variable to name
    bool isLoggedIn(std::string& output);
    void getUserName(InputManager* inputs, const std::string& user, std::string& outputName);
};

#endif