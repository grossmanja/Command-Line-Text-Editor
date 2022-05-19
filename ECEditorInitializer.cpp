#include "ECTextViewImp.h"
#include "ECKeyObserver.h"
#include "ECtextDocCtrl.h"
#include "ECCommandHistory.h"
#include "ECFileReading.h"
#include "ECSearchDoc.h"
#include "ECReplace.h"
#include "ECHighlightText.h"
#include <iostream>
#include <cstdio>

using namespace std;

//! This is the main file that initializes all of the different objects and connects them together


int main(int argc, char* argv[]){
    //First, check for file name as an argument, if there isn't, throw an error message 
    if(argc < 2){
        cout << "Error, enter a file to open" << endl;
        cout << "Usage: ./editor [file Name]" << endl;
        return -1;
    }
    //Start to create all of the different objects
    ECTextViewImp textViewImp;
    ECStatusRow statusRow(&textViewImp);
    //Initalize the status Row to the default edit mode message
    statusRow.changeStatusRow("Page 1/1", "Edit Mode");
    ECTextDoc textDoc(textViewImp.GetRowNumInView());
    ECTextDocCtrl textDocCtrl(&textDoc, &textViewImp, &statusRow);
    ECCommandHistory commandHistory;
    ECCursorControl cursorController(&textViewImp, &textDocCtrl);
    ECFileWriting fileWriter(&textDocCtrl);
    ECHighlightText highlightText(&textViewImp);
    ECSearchDoc searchDoc(&textDocCtrl, &statusRow);
    ECReplace replaceDoc(&textDocCtrl, &statusRow);
    
    //Setup all of the key observers and the chain of responsibility
    //!Main key handlers for regular text input
    //Chain goes in this order:
    //Letters->right arrow -> left arrow -> up arrow -> down arrow -> enter key -> backspace -> undo -> redo -> quit
    ECQuitHandler quitHandler(NULL, &textDocCtrl, &fileWriter, argv[1], &textViewImp);
    ECRedoHandler redoHandler(&quitHandler, &textDocCtrl, &cursorController, &commandHistory, &textViewImp);
    ECUndoHandler undoHandler(&redoHandler, &textDocCtrl, &cursorController, &commandHistory, &textViewImp);
    ECBackspaceHandlerMain backspaceHandlerMain(&undoHandler, &textDocCtrl, &cursorController, &commandHistory, &textViewImp);
    ECEnterKeyHandlerMain enterKeyHandlerMain(&backspaceHandlerMain, &textDocCtrl, &cursorController, &commandHistory, &textViewImp);
    ECDownArrowKeyHandler downArrowHandler(&enterKeyHandlerMain, &cursorController, &textViewImp);
    ECUpArrowKeyHandler upArrowHandler(&downArrowHandler, & cursorController, &textViewImp);
    ECLeftArrowKeyHandler leftArrowHandler(&upArrowHandler, &cursorController, &textViewImp);
    ECRightArrowKeyHandler rightArrowHandler(&leftArrowHandler, &cursorController, &textViewImp);
    ECLetterKeyHandlerMain letterHandlerMain(&rightArrowHandler, &textDocCtrl, &cursorController, &commandHistory, &textViewImp);
    
    //!Key handlers for search function
    //Chain goes in this order
    //Letters -> esc key -> backspace -> enter key
    ECEnterKeyHandlerSearch enterKeyHandlerSearch(NULL, &textDocCtrl, &searchDoc, &highlightText, &textViewImp);
    ECBackspaceHandlerSearch backspaceHandlerSearch(&enterKeyHandlerSearch, &textDocCtrl, &searchDoc, &textViewImp);
    ECESCKeyHandlerSearch escKeyHandlerSearch(&backspaceHandlerSearch, &textDocCtrl, &searchDoc, &highlightText, &textViewImp);
    ECLetterKeyHandlerSearch letterHandlerSearch(&escKeyHandlerSearch, &textDocCtrl, &searchDoc, &textViewImp);

    //!Key handlers for replace function
    //Chain goes in this order (same as search)
    //Letters -> esc key -> backspace -> enter key
    ECEnterKeyHandlerReplace enterKeyHandlerReplace(NULL, &textDocCtrl, &replaceDoc, &searchDoc, &commandHistory, &cursorController, &highlightText, &textViewImp);
    ECBackspaceHandlerReplace backspaceHandlerReplace(&enterKeyHandlerReplace, &textDocCtrl, &replaceDoc, &textViewImp);
    ECESCKeyHandlerReplace escKeyHandlerReplace(&backspaceHandlerReplace, &textDocCtrl, &replaceDoc, &textViewImp);
    ECLetterKeyHandlerReplace letterHandlerReplace(&escKeyHandlerReplace, &textDocCtrl, &replaceDoc, &textViewImp);

    ECKeyObserverHandler keyObserverHandler(&letterHandlerMain, &letterHandlerSearch, &letterHandlerReplace, &searchDoc, &replaceDoc, &textViewImp);
    //Set the keyObserverhandler in searchDoc and replaceDoc
    searchDoc.setKeyObserverHandler(&keyObserverHandler);
    replaceDoc.setKeyObserverHandler(&keyObserverHandler);
    //Attack keyObserverHandler to textViewImp
    textViewImp.Attach(&keyObserverHandler);
    ECFileReading fileReader(&textDocCtrl);
    //Start reading from file and adding the text to textDoc
    bool returnValue = fileReader.readFromFile(argv[1], &textViewImp);
    //Start!
    textViewImp.Show();
}
