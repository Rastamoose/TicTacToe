#include "stdafx.h"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "engine/View.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"
#include "Settings.h"

Settings::Settings(const Point& screenSize):       
    changeView(""), backgroundShapes(BackgroundShapes(screenSize, 3, 15, 3, Colour(28,88,88,255), Colour(28,88,88,255), Colour(28,88,88,255), 20, 2)){}

std::string Settings::update(DisplayManager* display, double dt){
    changeView = "";
    display->setBg(Colour(233, 150, 76, 255));
    backgroundShapes.update(display, dt);

    if (display->inputs->isPressed(SDLK_ESCAPE)){
        changeView = "mainmenu";
    }

    return changeView;
}