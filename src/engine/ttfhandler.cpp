#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include "engine/Utils.h"
#include "engine/TTFHandler.h"

//ADD LOAD CHECKING

TTFHandler::TTFHandler(SDL_Renderer* prenderer):
    renderer(prenderer){
    if(TTF_Init() < 0){
        std::cout << "<TTFHANDLER> TTF could not be initialised. SDL_ERROR: " << SDL_GetError() << std::endl;
    } else{
        std::cout << "<TTFHANDLER> TTF initialised.\n";
    }
}

TTFHandler::~TTFHandler(){
    for(auto& item: fonts){
        TTF_CloseFont(item.second);            // destroy font    
    }
    std::cout << "<TTFHANDLER> Fonts successfully destroyed.\n";

    for(auto& item:textures){
        SDL_DestroyTexture(item.second);
    }
    std::cout << "<TTFHANDLER> Textures successfully destroyed.\n";
}


void TTFHandler::multiplyStr(std::string& line, int multiplier){
    std::string initial = line;
    for(int i=0; i<(multiplier-1); i++){
        line += initial;
    }
}

void TTFHandler::loadTex(SDL_Surface* surf, const std::string& name){
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);      
    /* insert texture even if texture doesnt load successfully 
     * as to not cause crashes in other classes trying to access tex */
    // if key value pair doesnt exist then create a new one
    if(textures.count(name) == 0){
        textures.emplace(name, tex);               // use insertion operator in case key already exists                       
    } else{             // else change value of key
        textures.at(name) = tex;
    }
    if (tex == NULL){
        std::cout << "<TTFHANDLER> Failed to load text. Error: " << SDL_GetError() << std::endl;
    }
}

void TTFHandler::loadFont(const std::string& name, const char* path, int size){
    TTF_Font* font = TTF_OpenFont(path, size);
    fonts.emplace(name, font);

    if (font == NULL){
        std::cout << "<TTFHANDLER> Failed to load a font. Error: " << SDL_GetError() << std::endl;
    }
}

void TTFHandler::delFont(const std::string& name){
    TTF_CloseFont(fonts.at(name));
    fonts.erase(name);
}

void TTFHandler::loadText(const std::string& name, const std::string& fontName, const char* text, const Colour& colour){     // use TTF_RenderUTF8_Solid()
    if(fonts.contains(fontName)){
        SDL_Surface* surf = TTF_RenderUTF8_Solid(fonts.at(fontName), text, SDL_Color(colour.r, colour.g, colour.b, colour.a));
        loadTex(surf, name);
    } else
        std::cout << "<TTFHANDLER> Font not found. Error: Font '" << fontName << "' not loaded.\n";
    
}

void TTFHandler::loadTextCentered(const std::string& name, const std::string& fontName, const std::string& text, const Colour& colour, uint32_t lineLength){     // lineLength in px
    if(fonts.contains(fontName)){
        // create temporary text textures so we can find their width
        loadText("temp_centering", fontName, text.c_str(), colour);
        loadText("temp_gap", fontName, " ", colour);

        // find w of empty textures
        int textW = getTextSize("temp_centering").x;
        int gapW = getTextSize("temp_gap").x;

        // find space available after adding text
        int space = lineLength-textW;
        // find number of gaps we can have between text
        int noGaps = space/gapW;
        // there will be a gap on either side so divide by 2
        std::string gap = " ";
        multiplyStr(gap, noGaps/2);
        // if there was no extra space left then make gap nothing
        if(noGaps<=0)   gap="";
        
        // delete temp text
        delText("temp_centering");
        delText("temp_gap");

        std::string line = gap+text+gap;
        // load text normally now
        loadText(name, fontName, line.c_str(), colour);
    } else
        std::cout << "<TTFHANDLER> Font not found. Error: Font '" << fontName << "' not loaded.\n";
    
}

void TTFHandler::loadTextWrapped(const std::string& name, const std::string& fontName, const char* text, const Colour& colour, uint32_t wrapLength){  // wrapLength is row length in pixels
    if(fonts.contains(fontName)){
        SDL_Surface* surf = TTF_RenderUTF8_Solid_Wrapped(fonts.at(fontName), text, SDL_Color(colour.r, colour.g, colour.b, colour.a), wrapLength);
        loadTex(surf, name);
    } else{
        std::cout << "<TTFHANDLER> Font not found. Error: Font " << fontName << " not loaded.\n";
    }
}

void TTFHandler::loadTextWrappedCentered(const std::string& name, const std::string& fontName, const char* text, const Colour& colour, uint32_t wrapLength){  // wrapLength is row length in pixels
    if(fonts.contains(fontName)){
        SDL_Surface* surf = TTF_RenderUTF8_Solid_Wrapped(fonts.at(fontName), text, SDL_Color(colour.r, colour.g, colour.b, colour.a), wrapLength);
        loadTex(surf, name);
    } else{
        std::cout << "<TTFHANDLER> Font not found. Error: Font " << fontName << " not loaded.\n";
    }
}

void TTFHandler::delText(const std::string& name){
    SDL_DestroyTexture(textures.at(name));
    textures.erase(name);
}

SDL_Texture* TTFHandler::getText(const std::string& name){
    if(textures.contains(name)){
        return textures.at(name);
    } else{
        std::cout << "<TTFHANDLER> Text not found. Error: Key " << name << " doesn't exist.\n";
        return NULL;
    }
}

Point TTFHandler::getTextSize(const std::string& name){
    Point size = Point(0, 0);
    SDL_QueryTexture(getText(name), NULL, NULL, &size.x, &size.y);
    return size;
}
