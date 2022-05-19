#include "ECKeyObserver.h"
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;
//************************
//*LetterKeyHandlerBasic
//************************

ECKeyHandlerBasic :: ECKeyHandlerBasic(ECKeyHandlerBasic* nextHandlerIn) : nextHandler(nextHandlerIn){}

ECKeyHandlerBasic :: ~ECKeyHandlerBasic() {}

void ECKeyHandlerBasic :: Update() {}

bool ECKeyHandlerBasic :: Handle(int keyPressed){
    if(nextHandler != NULL){
        return nextHandler->Handle(keyPressed);
    }else{
        return false;
    }
}

//************************
//*LetterKeyObserverHandler
//************************

ECKeyObserverHandler :: ECKeyObserverHandler(ECKeyHandlerBasic* pKeyObserver, ECKeyHandlerBasic* searchKeyObserverIn, ECKeyHandlerBasic* replaceKeyObserverIn, ECSearchDoc* searchDocIn, ECReplace* replaceDocIn, ECTextViewImp* textViewImpIn) : mainHandler(pKeyObserver), searchHandler(searchKeyObserverIn), replaceHandler(replaceKeyObserverIn), searchDoc(searchDocIn), replaceDoc(replaceDocIn), textViewImp(textViewImpIn){
    inSearchMode = false;
    inReplaceMode = false;
}

void ECKeyObserverHandler :: Update(){
    int keyPressed = textViewImp->GetPressedKey();
    //First, check if CTRL-F is pressed to switch into Search mode
    if(keyPressed == 6){
        changeSearchMode(true);
        return;
    }//Then, check if we're in Search mode and / is pressed to switch into Replace mdoe
    else if(inSearchMode && keyPressed == 47){
        changeReplaceMode(true);
        changeSearchMode(false);
        return;
    }
    //Otherwise, send the pressed key down the correct chain of responsiblity
    if(inReplaceMode){
        replaceHandler->Handle(keyPressed);
    }else if(inSearchMode){
        searchHandler->Handle(keyPressed);
    }else{
        mainHandler->Handle(keyPressed);
    }    
}

//Change to Search mode and initialize it
void ECKeyObserverHandler :: changeSearchMode(bool status){
    inSearchMode = status;
    if(inSearchMode){
        searchDoc->initializeSearch();
    }   
}

//Change to Replace mode and intitialize it
void ECKeyObserverHandler :: changeReplaceMode(bool status){
    inReplaceMode = status;
    if(inReplaceMode){
        replaceDoc->initalizeReplace();
    }else{
        changeSearchMode(true);
    }
}

//************************
//*LetterKeyHandler
//************************

ECLetterKeyHandlerMain :: ECLetterKeyHandlerMain(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), cursorControl(cursorControlIn), cmdHistory(cmdHistoryIn), textViewImp(textViewImpIn){}

ECLetterKeyHandlerMain :: ~ECLetterKeyHandlerMain() {}

bool ECLetterKeyHandlerMain :: Handle(int keyPressed){
    //Check if our pressed key is an ASCII value
    if(keyPressed >= 32 && keyPressed <= 126){
        ECCommands* textInsert = new ECInsertTextCommand(textDocCtrl, textViewImp->GetCursorX(), textViewImp->GetCursorY(), char(keyPressed));
        int returnValue = cmdHistory->ExecuteCmd(textInsert);
        if(returnValue == 1){
            return cursorControl->MoveCursorRight();
        }else{
            return false;
        }
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}

//************************
//*EnterKeyHandlerMain
//************************

ECEnterKeyHandlerMain :: ECEnterKeyHandlerMain(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), cursorControl(cursorControlIn), cmdHistory(cmdHistoryIn), textViewImp(textViewImpIn) {}

bool ECEnterKeyHandlerMain :: Handle(int keyPressed){
    
    if(keyPressed == 13){
        int currentX = textViewImp->GetCursorX();
        int currentY = textViewImp->GetCursorY();

        ECCommands* newLineCommand = new ECInsertRowCommand(textDocCtrl, currentX, currentY);
        cmdHistory->ExecuteCmd(newLineCommand);
        cursorControl->MoveCursorDownAndLeft();
        return true;
            
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    } 
}

//************************
//*BackspaceHandlerMain
//************************
ECBackspaceHandlerMain :: ECBackspaceHandlerMain(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), cursorControl(cursorControlIn), cmdHistory(cmdHistoryIn), textViewImp(textViewImpIn) {}

ECBackspaceHandlerMain :: ~ECBackspaceHandlerMain() {}

bool ECBackspaceHandlerMain :: Handle(int keyPressed){
    
    if(keyPressed == 127){
        int currentX = textViewImp->GetCursorX();
        int currentY = textViewImp->GetCursorY();
        //First, check if we're at the very beginning of the first page
        //If so, then don't do anything
        if(currentX == 0 && currentY == 0 && textDocCtrl->getCurrentIndex() == 0) return false;
        
        int tempEndOfPrevLine;
        if(currentY != 0){
            //Get the location of the end of the previous line, which is needed when moving the cursor
            //Looking at it now, I totally could've used this for undo/redo, i don't know why i didn't but it's too late now so ¯\_(ツ)_/¯
            tempEndOfPrevLine = textDocCtrl->getNumCols(currentY - 1, textDocCtrl->getCurrentIndex());
        }else{
            tempEndOfPrevLine = textDocCtrl->getNumCols(textDocCtrl->getNumRowsInPage(textDocCtrl->getCurrentIndex() - 1) - 1, textDocCtrl->getCurrentIndex() - 1);
        }
        
        int returnValue;
        //Check if we're at the beginning of a line
        //If we are, then we want to do remove row
        //If we aren't, then we wanto do remove text
        if(currentX != 0){
            //cerr << "BackspaceHandlerMain creating RemoveTextCommand" << endl;
            ECCommands* removeText = new ECRemoveTextCommand(textDocCtrl, currentX, currentY);
            returnValue = cmdHistory->ExecuteCmd(removeText);
            if(returnValue != -1){
                cursorControl->MoveCursorLeft();
            }
        }
        else{
            ECCommands* removeRow = new ECRemoveRowCommand(textDocCtrl, currentX, currentY);
            returnValue = cmdHistory->ExecuteCmd(removeRow);
            //If we successfully removed a row, then we need to move the cursor
            if(returnValue != -1){
                //If we're at the beginning of a page, then we need to switch pages and move the cursor to tempEndOfPrevLine
                //Otherwise, just move the cursor to tempEndOfPrevLine
                if(currentY == 0){
                    cursorControl->MoveCursorTo(tempEndOfPrevLine, textDocCtrl->getNumRowsInPage(textDocCtrl->getCurrentIndex() - 1) - 1, textDocCtrl->getCurrentIndex() - 1);
                    textDocCtrl->changePreviousPage();
                }else{
                    cursorControl->MoveCursorTo(tempEndOfPrevLine, currentY - 1);
                }
            }
        }
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    } 
}