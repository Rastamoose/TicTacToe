#ifndef MAINMENU_H
#define MAINMENU_H

#include <string>
#include <unordered_map>
#include "engine/View.h"
#include "engine/DisplayManager.h"
#include "engine/InputManager.h"
#include "engine/ResourceManager.h"
#include "engine/Button.h"
#include "engine/BackgroundShapes.h"
#include "engine/Utils.h"

struct MenuColours{
    Colour title;
    Colour text;
    Colour buttonText;

    MenuColours(): title(Colour(0x88,0xC0,0xD0,255)), text(Colour(0x8F,0xBC,0xBB, 255)),
                   buttonText(Colour(94,129,172,255)){}
};

class MainMenu : public View{
    std::string changeView;
    std::unordered_map<std::string, Button> buttons;
    MenuColours colours;
    BackgroundShapes backgroundShapes;
    ResourceManager* resources;
    Point titlePos;

    void updateButtons(DisplayManager* display, double dt);

    public:
    MainMenu(ResourceManager* resources, const Point& screenSize);
    std::string update(DisplayManager* display, double dt);
};

#endif