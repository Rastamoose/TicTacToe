#include "stdafx.h"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine/Utils.h"
#include "engine/WindowManager.h"
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h"
#include "engine/AnimationHandler.h"
#include "engine/TimeManager.h"
#include "engine/ViewManager.h"
#include "mainmenu/MainMenu.h"
#include "scores/Leaderboard.h"
#include "Settings.h"
#include "game/Game.h"

// initialise resources used commonly across program
void loadGlobalResources(ResourceManager* resources){
    resources->ttf->loadFont("Little Orion", "./res/fonts/Little Orion.ttf", 64);
}

// clean up initialised resources
void delGlobalResources(ResourceManager* resources){
    resources->ttf->delFont("Little Orion");
}

int main(int argc, char* argv[]) {
    freopen("log.txt", "w", stdout);
    // initialise required classes
    WindowManager window = WindowManager("Project", Rect(200, 200, 1280, 700), SDL_WINDOW_SHOWN);
    ResourceManager resources = ResourceManager(window.getRenderer());
    resources.initIMG();
    resources.initTTF();
    loadGlobalResources(&resources);
    DisplayManager display = DisplayManager(&window);    
    TimeManager time = TimeManager(60);

    // initialise views
    ViewManager viewManager = ViewManager();

    MainMenu mainMenu = MainMenu(&resources, display.getSize());
    Leaderboard leaderboard = Leaderboard(resources.ttf, display.getSize());
    Game gamePlayer = Game(&resources, display.getSize(), false);
    Game gameAI = Game(&resources, display.getSize(), true);

    viewManager.addView("mainmenu", &mainMenu);
    viewManager.addView("leaderboard", &leaderboard);
    viewManager.addView("gamePlayer", &gamePlayer);
    viewManager.addView("gameAI", &gameAI);
    viewManager.setCurrentView("mainmenu");
    
    // Game loop
    bool running = true;
    while(running){
        time.updateStart();

        window.inputs.pollEvents();
        if(window.inputs.quit || viewManager.isQuit){
            running = false;
        }

        window.clear();
        viewManager.updateView(&display, time.dt);
        window.display();

        time.updateTime();
        
    }

    delGlobalResources(&resources);
    return 0;
}