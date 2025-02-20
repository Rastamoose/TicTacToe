#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "engine/View.h"
#include "engine/DisplayManager.h"
#include "engine/BackgroundShapes.h"
#include "engine/ResourceManager.h"
#include "engine/Utils.h"

class Leaderboard : public View{
    std::string changeView;
    BackgroundShapes backgroundShapes;
    TTFHandler* ttf;

    void updateButtons(DisplayManager* display, double dt);

    public:
    Leaderboard(TTFHandler* ttf, const Point& screenSize);
    std::string update(DisplayManager* display, double dt);
};

#endif