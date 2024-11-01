#include <string>
#include <cstring>
#include <vector>
#include <SDL2/SDL.h>
#include "engine/Utils.h"
#include "engine/InputManager.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"
#include "engine/IMGHandler.h"
#include "game/Login.h"

Login::Login(ResourceManager* presources, const Point& ppos, const std::string& pfont): 
    resources(presources), colours(LoginColours()), pos(LoginPositions(ppos)), state(LoginState()), font(pfont), 
    userName(" "), input(){

    resources->ttf->loadTextWrapped("login_request", font, "Please enter your name", colours.text, pos.inputBox.w);
}

bool Login::getInput(InputManager* inputs){
    std::vector<int>* pressedKeys = inputs->getPressed();
    for(auto key:*pressedKeys){
        // if key is backspace, then remove last element instead of adding another
        if(key==SDLK_BACKSPACE){
            if(!input.empty())  input.pop_back();       // only remove if not empty

        // if key is enter, confirm input (if not empty)
        }else if(key==SDLK_RETURN){
            if(!input.empty())  return true;
        }
        
        else{
            const char* letter = inputs->getKeyName(key);
            // only add if its a number or letter (currently doesn't filter out symbols)
            if(strlen(letter)==1){
                // get key name and lowercase it
                input.push_back(tolower(*(inputs->getKeyName(key))));
            }
        }
    }

    return false;
}

std::string Login::getEnteredName(){
    std::string text = "";
    for(char letter:input){
        text+=letter;
    }
    return text;
}

void Login::render(DisplayManager* display){
    int borderWidth = 28;
    display->drawThickRect(pos.loginBox, borderWidth, colours.border);
    display->drawFillRect(pos.loginBox, colours.bg);
    display->drawThickRect(pos.inputBox, borderWidth/2, colours.inputBoxBorder);
    display->drawFillRect(pos.inputBox, colours.inputBox);
    display->render(resources->ttf->getText("login_request"), pos.text);
    // render user inputted text
    if(!input.empty()){
        std::string text = getEnteredName();
        resources->ttf->loadText("login_input", font, text.c_str(), colours.textInput);
        display->render(resources->ttf->getText("login_input"), pos.inputText);
    }
}

bool Login::isLoggedIn(std::string& output){
    if(state.loggedIn){
        return true;
    }
    return false;
}

void Login::getUserName(InputManager* inputs, const std::string& user, std::string& outputName){
    // if this is a new entry
    if(userName!=user){
        state.loggedIn = false;
        state.loggingIn = true;
        userName = user;
    // else if this name has been inputted by user
    } else if(state.loggedIn){
        // output changes and return
        outputName = getEnteredName();
        input.clear();
        return;
    }
    
    // load text for request
    std::string req = "Please enter your name "+user;
    resources->ttf->loadTextWrapped("login_request", font, req.c_str(), colours.text, pos.inputBox.w);

    // get text input
    if(getInput(inputs)){
        state.loggingIn = false;
        state.loggedIn = true;

    }
}