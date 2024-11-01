#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <string>
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h" 
#include "engine/Utils.h"

// scoreboard colours
struct SBColours{
    Colour fill;
    Colour border;
    Colour title;
    Colour header;
    Colour scores;

    SBColours(): fill(Colour(76,86,106,255)), border(Colour(216, 222, 233,255)), title(Colour(236,239,244,255)), 
                 header(Colour(229,233,240,255)), scores(Colour(216,222,233,255)){}
};

class Scoreboard{
    ResourceManager* resources;
    Rect rect;
    SBColours colours;
    std::string font;
    int lineGap;

    void multiplyStr(std::string& line, int multiplier);
    // sets proportional gaps for line with 3 columns | takes id of text from resource manager
    void setLine(const std::string& id, const std::string& first, const std::string& second, const std::string& third, const Colour& colour);
    
    public:
    Scoreboard(ResourceManager* presources, const Rect& prect, const std::string& pfont);
    void updateTitle(const std::string& p1Name, const std::string p2Name);
    void updateScores(int p1Score, int ties, int p2Score);
    void render(DisplayManager* display);
    void setBorderColour(const Colour& colour);
};

#endif