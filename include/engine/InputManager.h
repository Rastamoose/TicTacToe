// ABSTRACTION OVER API INPUT METHODS

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <vector>
#include <SDL2/SDL.h>
#include "engine/Utils.h"

class InputManager{ 
    std::vector<SDL_Keycode> pressedKeys;       // hold all keys pressed
    // array containing states of different sdl mousebuttonevents
    // index corresponds to mousebutton
    bool mouseButtons[33];

    public:
    bool quit;

    InputManager();
    void pollEvents();
    // check if key is pressed
    bool isPressed(const SDL_Keycode key);       
    bool isClicked(int button);
    // checks if mouse button is held down
    // takes one of the SDL_BUTTON_{BUTTON}MASKs (see below)
    bool isClickedContinuous(int button);                     
    Point getMousePos();
    // get all pressed keys
    std::vector<SDL_Keycode>* getPressed();
    const char* getKeyName(const SDL_Keycode key);

};

#endif

/*
BUTTON MASKS:
 SDL_BUTTON_LMASK   
 SDL_BUTTON_MMASK    
 SDL_BUTTON_RMASK   
 SDL_BUTTON_X1MASK
 SDL_BUTTON_X2MASK
*/
