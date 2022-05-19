#ifndef ECHIGHLIGHTTEXT_H
#define ECHIGHLIGHTTEXT_H
#include "ECTextViewImp.h"

//Class that highlights text in the view, used in ECSearchDoc
class ECHighlightText{
public:
    ECHighlightText(ECTextViewImp* textViewImpIn);
    ~ECHighlightText(){}
    //Command that highlights the text at specified (x,y) locations in locations.
    //locations is vector of (x,y) locations [which are stored as a vector of ints] of each location where we need to highlight
    //textLength tells us how many characters we need to highlight to highlight the whole text
    void highlightText(std::vector<std::vector<int>> locations, int textLength);
    //Command that resets all text to the default color.
    void resetHighlight();

private:
    ECTextViewImp* textViewImp;
};

#endif