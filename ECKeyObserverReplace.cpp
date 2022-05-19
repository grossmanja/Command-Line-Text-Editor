#include "ECKeyObserver.h"
#include <iostream>
using namespace std;

ECLetterKeyHandlerReplace :: ECLetterKeyHandlerReplace(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECReplace* ReplaceDocIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), replaceDoc(ReplaceDocIn){
}

bool ECLetterKeyHandlerReplace :: Handle(int keyPressed){
    
    if(keyPressed >= 32 && keyPressed <= 126){
        replaceDoc->addLetterToTextToReplace(char(keyPressed));
        
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}


ECEnterKeyHandlerReplace :: ECEnterKeyHandlerReplace(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECReplace* ReplaceDocIn, ECSearchDoc* searchDocIn, ECCommandHistory* cmdHistoryIn, ECCursorControl* cursorControlIn, ECHighlightText* highlightTextIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), replaceDoc(ReplaceDocIn), searchDoc(searchDocIn), cmdHistory(cmdHistoryIn), cursorControl(cursorControlIn), highlightText(highlightTextIn){
}

bool ECEnterKeyHandlerReplace :: Handle(int keyPressed){
    if(keyPressed == 13){
        //Start the process of replacing the text
        string textToRemove = searchDoc->getTextToSearch();
        string textToReplace = replaceDoc->getTextToReplace();
        vector<vector<int>> locations = searchDoc->getTextLocations();

        ECCommands* replaceCommand = new ECReplaceTextCommand(textDocCtrl, replaceDoc, locations, textToRemove, textToReplace);

        int returnValue = cmdHistory->ExecuteCmd(replaceCommand);
        //If we were successful in replacing text, then move the cursor, switch to edit mode, and run the search with our newly changed text
        if(returnValue == 1){
            cursorControl->MoveCursorNearestValid();
            replaceDoc->turnOffReplace();
            searchDoc->changeWholeTextToSearch(textToReplace);
            vector<vector<int>> tempLocations = searchDoc->findTextLocations();
            //If we found any locations of the replaced text, then highlight that text
            //If we don't, e.g. the replacement text is empty, then just reset the hightlighted text
            if(tempLocations[0][0] != -1){
                highlightText->resetHighlight();
                highlightText->highlightText(tempLocations, (int)textToReplace.length());
            }else{
                highlightText->resetHighlight();
            }
                
        }

        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}

ECESCKeyHandlerReplace :: ECESCKeyHandlerReplace(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECReplace* ReplaceDocIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), replaceDoc(ReplaceDocIn){
}

bool ECESCKeyHandlerReplace :: Handle(int keyPressed){
    if(keyPressed == 27){
        replaceDoc->turnOffReplace();
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}

ECBackspaceHandlerReplace :: ECBackspaceHandlerReplace(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECReplace* ReplaceDocIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), replaceDoc(ReplaceDocIn){
}
bool ECBackspaceHandlerReplace :: Handle(int keyPressed){
    if(keyPressed == 127){
        replaceDoc->removeLetterFromTextToReplace();
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}