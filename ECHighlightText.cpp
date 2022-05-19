#include "ECHighlightText.h"
#include <iostream>
using namespace std;

ECHighlightText :: ECHighlightText(ECTextViewImp* textViewImpIn) : textViewImp(textViewImpIn){
}

//We get the vector<vector<int>> of locations from ECSearchDoc
void ECHighlightText :: highlightText(vector<vector<int>> locations, int textLength){
    for(int i = 0; i < (int)locations.size(); i++){
        //Since locations only holds the x,y location of the start of the text we want to highlight, we 
        textViewImp->SetColor(locations[i][1], locations[i][0], locations[i][0] + textLength - 1, TEXT_COLOR_RED);
    }
}

void ECHighlightText :: resetHighlight(){
    int rows = textViewImp->GetRowNumInView();
    int cols = textViewImp->GetColNumInView();
    for(int i = 0; i < rows; i++){
        //For some reason, when I try to reset the color of the row with just one call of SetColor(), it doesn't work
        //So I just call SetColor() on every index on every row.
        //A lot less efficient, but it works.
        for(int j = 0; j < cols - 1; j++){
            textViewImp->SetColor(i, j, textViewImp->GetColNumInView(), TEXT_COLOR_DEF);
        }
    }
}