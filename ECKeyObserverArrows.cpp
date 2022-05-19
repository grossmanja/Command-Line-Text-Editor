#include "ECKeyObserver.h"

//************************
//*LeftArrowKeyHandler
//************************

ECLeftArrowKeyHandler :: ECLeftArrowKeyHandler(ECKeyHandlerBasic* nextHandlerIn, ECCursorControl* cursorControlIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), cursorControl(cursorControlIn), textViewImp(textViewImpIn) {}

ECLeftArrowKeyHandler :: ~ECLeftArrowKeyHandler(){}

bool ECLeftArrowKeyHandler :: Handle(int keyPressed){
    if(keyPressed == 1000){
        return cursorControl->MoveCursorLeft();
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}

//************************
//*RightArrowKeyHandler
//************************
                         
ECRightArrowKeyHandler :: ECRightArrowKeyHandler(ECKeyHandlerBasic* nextHandlerIn, ECCursorControl* cursorControlIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), cursorControl(cursorControlIn), textViewImp(textViewImpIn) {}

ECRightArrowKeyHandler :: ~ECRightArrowKeyHandler() {}

bool ECRightArrowKeyHandler :: Handle(int keyPressed){
    if(keyPressed == 1001){
        return cursorControl->MoveCursorRight();
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}

//************************
//*UpArrowKeyHandler
//************************

ECUpArrowKeyHandler :: ECUpArrowKeyHandler(ECKeyHandlerBasic* nextHandlerIn, ECCursorControl* cursorControlIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), cursorControl(cursorControlIn), textViewImp(textViewImpIn) {}

ECUpArrowKeyHandler :: ~ECUpArrowKeyHandler() {}

bool ECUpArrowKeyHandler :: Handle(int keyPressed){
    if(keyPressed == 1010){
        int returnValue = cursorControl->MoveCursorUp();
        return (returnValue == 1 || returnValue == 2);
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}

//************************
//*DownArrowKeyHandler
//************************

ECDownArrowKeyHandler :: ECDownArrowKeyHandler(ECKeyHandlerBasic* nextHandlerIn, ECCursorControl* cursorControlIn, ECTextViewImp* textViewImpIn) : ECKeyHandlerBasic(nextHandlerIn), cursorControl(cursorControlIn), textViewImp(textViewImpIn) {}

ECDownArrowKeyHandler :: ~ECDownArrowKeyHandler() {}

bool ECDownArrowKeyHandler :: Handle(int keyPressed){
    if(keyPressed == 1011){
        int returnValue = cursorControl->MoveCursorDown();
        return (returnValue == 1 || returnValue == 2);
    }else{
        return ECKeyHandlerBasic::Handle(keyPressed);
    }
}