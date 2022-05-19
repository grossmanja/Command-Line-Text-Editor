#include "ECKeyObserver.h"
#include <iostream>
using namespace std;

ECLetterKeyHandlerSearch :: ECLetterKeyHandlerSearch(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECSearchDoc* searchDocIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), searchDoc(searchDocIn), textViewImp(textViewImpIn) {
}

bool ECLetterKeyHandlerSearch :: Handle(int keyPressed){
    if(keyPressed >= 32 && keyPressed <= 126){
        searchDoc->addLetterToTextToSearch(char(keyPressed));
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}


ECEnterKeyHandlerSearch :: ECEnterKeyHandlerSearch(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECSearchDoc* searchDocIn, ECHighlightText* highlightTextIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), searchDoc(searchDocIn), highlightText(highlightTextIn), textViewImp(textViewImpIn) {
}

bool ECEnterKeyHandlerSearch :: Handle(int keyPressed){
    if(keyPressed == 13){
        //Start the process of searching the text
        //textLength is important for when we highlight text
        int textLength = searchDoc->getTextToSearch().length();
        vector<vector<int>> locations = searchDoc->findTextLocations();
        highlightText->resetHighlight();
        //If didn't find any locations, return
        if(locations[0][0] == -1){
            return true;
        }else{
            //Otherwise, highlight the text we did find
            highlightText->highlightText(locations, textLength);
        }
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}


ECESCKeyHandlerSearch :: ECESCKeyHandlerSearch(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECSearchDoc* searchDocIn, ECHighlightText* highlightIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), searchDoc(searchDocIn), highlight(highlightIn), textViewImp(textViewImpIn) {
}

bool ECESCKeyHandlerSearch :: Handle(int keyPressed){
    
    if(keyPressed == 27){
        searchDoc->turnOffSearch();
        highlight->resetHighlight();
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}


ECBackspaceHandlerSearch :: ECBackspaceHandlerSearch(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECSearchDoc* searchDocIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), searchDoc(searchDocIn), textViewImp(textViewImpIn) {
}

bool ECBackspaceHandlerSearch :: Handle(int keyPressed){
    if(keyPressed == 127){
        searchDoc->removeLetterFromTextToSearch();
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}