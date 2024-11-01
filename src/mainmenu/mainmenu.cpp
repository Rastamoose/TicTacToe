#include <string>
#include <unordered_map>
#include "engine/View.h"
#include "engine/DisplayManager.h"
#include "engine/InputManager.h"
#include "engine/ResourceManager.h"
#include "engine/Button.h"
#include "engine/BackgroundShapes.h"
#include "engine/Utils.h"
#include "mainmenu/MainMenu.h"

#include <iostream>

MainMenu::MainMenu(ResourceManager* presources, const Point& screenSize):       
    changeView(""), colours(MenuColours()), backgroundShapes(BackgroundShapes(screenSize, 3, 15, 3, Colour(67,76,94,255), Colour(151,69,63,255), Colour(90,116,122,255), 20, 2)),
    resources(presources), titlePos(Point(500,10)){

    buttons.emplace("singleplayer", Button(resources->img, "./res/img/mainmenu/menubutton/play", 1, 6, Point(175,325)));
    buttons.emplace("two_player", Button(resources->img, "./res/img/mainmenu/menubutton/play", 1, 6, Point(650,325)));
    buttons.emplace("quit", Button(resources->img, "./res/img/mainmenu/menubutton/quit", 1, 8, Point(1184,30)));

    // title font
    resources->ttf->loadFont("Little Orion_80", "./res/fonts/Little Orion.ttf", 80);

    resources->ttf->loadText("menu_title", "Little Orion_80", "Tic Tac Toe", colours.title);
    resources->ttf->loadText("menu_subTitle", "Little Orion", "Select a gamemode", colours.text);
    resources->ttf->loadTextCentered("PvP", "Little Orion", "Singleplayer", colours.buttonText, buttons.at("singleplayer").getSize().x);
    resources->ttf->loadTextCentered("PvC", "Little Orion", "Two player", colours.buttonText, buttons.at("two_player").getSize().x);
}

void MainMenu::updateButtons(DisplayManager* display, double dt){
    // update and render all buttons
    for (auto& button: buttons){
        button.second.update(display->inputs, dt);
        button.second.render(display);
    }

    // check for viewchanges
    if (buttons.at("singleplayer").isPressed || display->inputs->isPressed(SDLK_s)){
        changeView = "gameAI";
    } else if (buttons.at("two_player").isPressed || display->inputs->isPressed(SDLK_t)){
        changeView = "gamePlayer";
    } else if (buttons.at("quit").isPressed){
        changeView = "quit";
    }
}

std::string MainMenu::update(DisplayManager* display, double dt){
    changeView = "";
    display->setBg(Colour(46, 52, 73, 255));
    backgroundShapes.update(display, dt);
    updateButtons(display, dt);

    // render text elements

    //titlePos = Point(0,0);
    display->render(resources->ttf->getText("menu_title"), titlePos);
    display->render(resources->ttf->getText("menu_subTitle"), Point(titlePos.x-45, titlePos.y+resources->ttf->getTextSize("menu_title").y));
    display->render(resources->ttf->getText("PvP"), Point(buttons.at("singleplayer").getPos().x-15, buttons.at("singleplayer").getPos().y+85));
    display->render(resources->ttf->getText("PvC"), Point(buttons.at("two_player").getPos().x, buttons.at("two_player").getPos().y+85));

    // if going to switch off view then reset buttons
    if (changeView != ""){
        for (auto& button:buttons){
            button.second.resetAnimations();
        }
    }

    return changeView;
}
