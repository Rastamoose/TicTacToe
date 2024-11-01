#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include "engine/View.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"
#include "engine/BackgroundShapes.h"

class Settings : public View{
    std::string changeView;
    BackgroundShapes backgroundShapes;

    void updateButtons(DisplayManager* display, double dt);

    public:
    Settings(const Point& screenSize);
    std::string update(DisplayManager* display, double dt);
};

#endif