#include "ECKeyObserver.h"
using namespace std;
//***************
//*UndoHandler
//***************

ECUndoHandler :: ECUndoHandler(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn) :  ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), cursorControl(cursorControlIn), cmdHistory(cmdHistoryIn){
}

ECUndoHandler :: ~ECUndoHandler(){}

bool ECUndoHandler :: Handle(int keyPressed){
    if(keyPressed == 26){
        bool returnValue = cmdHistory->Undo();
        cursorControl->MoveCursorNearestValid();
        return returnValue;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}

//***************
//*RedoHandler
//***************

ECRedoHandler :: ECRedoHandler(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), cursorControl(cursorControlIn), cmdHistory(cmdHistoryIn), textViewImp(textViewImpIn) {
}

ECRedoHandler :: ~ECRedoHandler(){}

bool ECRedoHandler :: Handle(int keyPressed){
    
    if(keyPressed == 25){
        bool returnValue = cmdHistory->Redo();
        cursorControl->MoveCursorNearestValid();
        return returnValue;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}

//***************
//*QuitHandler
//***************

ECQuitHandler :: ECQuitHandler(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECFileWriting* fileWriterIn, std::string fileNameIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), textDocCtrl(textDocCtrlIn), fileWriter(fileWriterIn), fileName(fileNameIn), textViewImp(textViewImpIn){
}

ECQuitHandler :: ~ECQuitHandler(){}

bool ECQuitHandler :: Handle(int keyPressed){
    
    if(keyPressed == 17){
        fileWriter->writeToFile(fileName);
        textViewImp->Quit();
        return true;
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}