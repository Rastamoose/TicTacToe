#include <string>
#include "engine/DisplayManager.h"
#include "engine/ResourceManager.h" 
#include "engine/Utils.h"
#include "game/Scoreboard.h"

#include <iostream>

Scoreboard::Scoreboard(ResourceManager* presources, const Rect& prect, const std::string& pfont): 
    resources(presources), rect(prect), colours(SBColours()), font(pfont), lineGap(75){
        
    resources->ttf->loadTextCentered("scoreboard", font, "Scoreboard", colours.title, rect.w);
    updateTitle("NA", "NA");
    updateScores(0,0,0);
}

void Scoreboard::multiplyStr(std::string& line, int multiplier){
    std::string initial = line;
    for(int i=0; i<(multiplier-1); i++){
        line += initial;
    }
}

void Scoreboard::setLine(const std::string& id, const std::string& first, const std::string& second, const std::string& third, const Colour& colour){
    // create temporary text textures so we can find their width
    resources->ttf->loadText("sb_first", font, first.c_str(), colour);
    resources->ttf->loadText("sb_second", font, second.c_str(), colour);
    resources->ttf->loadText("sb_third", font, third.c_str(), colour);
    resources->ttf->loadText("sb_gap", font, " ", colour);

    // find w of empty textures
    int firstW = resources->ttf->getTextSize("sb_first").x;
    int secondW = resources->ttf->getTextSize("sb_second").x;
    int thirdW = resources->ttf->getTextSize("sb_third").x;
    int gapW = resources->ttf->getTextSize("sb_gap").x;

    // find space available after adding text
    int space = rect.w-(firstW+secondW+thirdW);
    // find number of gaps we can have between text
    int noGaps = space/gapW;
    // there will be 4 gaps in the column so divide by 4
    std::string gap = " ";
    multiplyStr(gap, noGaps/4);

    // delete temp text
    resources->ttf->delText("sb_first");
    resources->ttf->delText("sb_second");
    resources->ttf->delText("sb_third");
    resources->ttf->delText("sb_gap");

    std::string line = gap+first+gap+second+gap+third;
    // reload text for scores | (need to convert values to a c string)
    resources->ttf->loadText(id, font, line.c_str(), colour);
}

void Scoreboard::updateTitle(const std::string& p1Name, const std::string p2Name){
    setLine("sb_title", p1Name, "Ties", p2Name, colours.title);
}

void Scoreboard::updateScores(int p1Score, int ties, int p2Score){
    setLine("sb_scores", std::to_string(p1Score), std::to_string(ties), std::to_string(p2Score), colours.scores);
}

void Scoreboard::render(DisplayManager* display){
    // render border and filled square
    display->drawThickRect(rect, 7, colours.border);
    display->drawFillRect(rect, colours.fill);

    // draw leaderboard elements
    display->render(resources->ttf->getText("scoreboard"), Point(rect.x,rect.y));
    // title is after 1 lineGap and scores are after 2.5 lineGap
    display->render(resources->ttf->getText("sb_title"), Point(rect.x,rect.y+lineGap));
    display->render(resources->ttf->getText("sb_scores"), Point(rect.x,rect.y+2.5*lineGap));
}

void Scoreboard::setBorderColour(const Colour& colour){
    colours.border = colour;
}