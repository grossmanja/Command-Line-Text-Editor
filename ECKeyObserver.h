#ifndef ECKEYOBSERVER_H
#define ECKEYOBSERVER_H

class ECKeyObserverHandler;

#include "ECObserver.h"
#include "ECTextViewImp.h"
#include "ECCursorControl.h"
#include "ECCommandHistory.h"
#include "ECCommands.h"
#include "ECFileWriting.h"
#include "ECReplace.h"
#include "ECSearchDoc.h"
#include "ECHighlightText.h"
#include "ECTextDocCtrl.h"

//Interface for the basic Key Handlers
class ECKeyHandlerBasic : public ECObserver{
public:
    //ECKeyHandlerBasic();
    ECKeyHandlerBasic(ECKeyHandlerBasic* nextHandlerIn);
    virtual ~ECKeyHandlerBasic();
    virtual void Update();
    virtual bool Handle(int keyPressed);
private:
    ECKeyHandlerBasic* nextHandler;
};

//Class that initally handles the Notify() and starts the chain of responsibility
//Also keeps track of which chain of responsibility to use based on which mode we're in: Edit, Search, or Replace 
class ECKeyObserverHandler : public ECObserver{
public:
    ECKeyObserverHandler(ECKeyHandlerBasic* pKeyObserver, ECKeyHandlerBasic* searchKeyObserver, ECKeyHandlerBasic* replaceKeyObserver, ECSearchDoc* searchDocIn, ECReplace* replaceDocIn, ECTextViewImp* textViewImpIn);
    //Command that executes when the observer updates
    virtual void Update();
    //Command that enables/disables search mode
    void changeSearchMode(bool status);
    //Command that enables/disables replace mode
    //If neither search mode nor replace mode are enabled, then it defaults to edit mode 
    void changeReplaceMode(bool status);
private:
    ECKeyHandlerBasic* mainHandler;     //First in the chain of responsibility for edit mode 
    ECKeyHandlerBasic* searchHandler;   //First in the chain of responsibility for search mode 
    ECKeyHandlerBasic* replaceHandler;  //First in the chain of responsibility for replace mode

    ECSearchDoc* searchDoc;
    ECReplace* replaceDoc;
    ECTextViewImp* textViewImp;
    bool inSearchMode;                  //Keeps track of whether search mode is enabled/disabled
    bool inReplaceMode;                 //Keeps track of whether replace mode is enabled/disabled
};

//Class that handles the input of characters in edit mode
//Handles all the main ASCII characters inputted by the user
class ECLetterKeyHandlerMain : public ECKeyHandlerBasic{
public:
    ECLetterKeyHandlerMain(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textViewIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn);
    virtual ~ECLetterKeyHandlerMain();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECCursorControl* cursorControl;
    ECCommandHistory* cmdHistory;
    ECTextViewImp* textViewImp;
};
//Class that handles the left arrow key input in edit mode
class ECLeftArrowKeyHandler : public ECKeyHandlerBasic{
public:
    ECLeftArrowKeyHandler(ECKeyHandlerBasic* nextHandlerIn, ECCursorControl* cursorControlIn, ECTextViewImp* textViewImpIn);
    virtual ~ECLeftArrowKeyHandler();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECCursorControl* cursorControl;
    ECTextViewImp* textViewImp;
};

//Class that handles the right arrow key input in edit mode
class ECRightArrowKeyHandler : public ECKeyHandlerBasic{
public:
    ECRightArrowKeyHandler(ECKeyHandlerBasic* nextHandlerIn, ECCursorControl* cursorControlIn, ECTextViewImp* textViewImpIn);
    virtual ~ECRightArrowKeyHandler();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECCursorControl* cursorControl;
    ECTextViewImp* textViewImp;
};

//Class that handles the up arrow key input in edit mode
class ECUpArrowKeyHandler : public ECKeyHandlerBasic{
public:
    ECUpArrowKeyHandler(ECKeyHandlerBasic* nextHandlerIn, ECCursorControl* cursorControlIn, ECTextViewImp* textViewImpIn);
    virtual ~ECUpArrowKeyHandler();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECCursorControl* cursorControl;
    ECTextViewImp* textViewImp;
};

//Class that handles the down arrow key input in edit mode
class ECDownArrowKeyHandler : public ECKeyHandlerBasic{
public:
    ECDownArrowKeyHandler(ECKeyHandlerBasic* nextHandlerIn, ECCursorControl* cursorControlIn, ECTextViewImp* textViewImpIn);
    virtual ~ECDownArrowKeyHandler();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECCursorControl* cursorControl;
    ECTextViewImp* textViewImp;
};

//Class that handles the enter key input in edit mode
class ECEnterKeyHandlerMain : public ECKeyHandlerBasic{
public:
    ECEnterKeyHandlerMain(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textViewIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn);
    virtual ~ECEnterKeyHandlerMain(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECCursorControl* cursorControl;
    ECCommandHistory* cmdHistory;
    ECTextViewImp* textViewImp;
};

//Class that handles the backspace key input in edit mode
//Handles whether to delete a character or delete a row of text based on cursor's x coordinate
class ECBackspaceHandlerMain : public ECKeyHandlerBasic{
public:
    ECBackspaceHandlerMain(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn);
    virtual ~ECBackspaceHandlerMain();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECCursorControl* cursorControl;
    ECCommandHistory* cmdHistory;
    ECTextViewImp* textViewImp;
};


//**********************************
//*CTRL Key Observers
//***********************************

//Class that handles the CTRL-Z input
class ECUndoHandler : public ECKeyHandlerBasic{
public:
    ECUndoHandler(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn);
    ~ECUndoHandler();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECCursorControl* cursorControl;
    ECCommandHistory* cmdHistory;
    ECTextViewImp* textViewImp;
};

//Class that handles the CTRL-Y input
class ECRedoHandler : public ECKeyHandlerBasic{
public:
    ECRedoHandler(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECCursorControl* cursorControlIn, ECCommandHistory* cmdHistoryIn, ECTextViewImp* textViewImpIn);
    ~ECRedoHandler();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECCursorControl* cursorControl;
    ECCommandHistory* cmdHistory;
    ECTextViewImp* textViewImp;
};

//Class that handles the CTRL-Q input
//Closes the text editor and saves it to a file
class ECQuitHandler : public ECKeyHandlerBasic{
public:
    ECQuitHandler(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECFileWriting* fileWriterIn, std::string fileNameIn, ECTextViewImp* textViewImpIn);
    ~ECQuitHandler();
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECFileWriting* fileWriter;
    const std::string fileName;
    ECTextViewImp* textViewImp;
};



//***********************************
//*Search Key Observers
//***********************************



//Class that handles ASCII letter key inputs in search mode 
class ECLetterKeyHandlerSearch : public ECKeyHandlerBasic{
public:
    ECLetterKeyHandlerSearch(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECSearchDoc* searchDocIn, ECTextViewImp* textViewImpIn);
    ~ECLetterKeyHandlerSearch(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECSearchDoc* searchDoc;
    ECTextViewImp* textViewImp;
};

//Class that handles the enter key input in search mode
//Runs the search
class ECEnterKeyHandlerSearch : public ECKeyHandlerBasic{
public:
    ECEnterKeyHandlerSearch(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECSearchDoc* searchDocIn, ECHighlightText* highlightTextIn, ECTextViewImp* textViewImpIn);
    ~ECEnterKeyHandlerSearch(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECSearchDoc* searchDoc;
    ECHighlightText* highlightText;
    ECTextViewImp* textViewImp;
};

//Class that handles the backspace key in search mode
class ECBackspaceHandlerSearch : public ECKeyHandlerBasic{
public:
    ECBackspaceHandlerSearch(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECSearchDoc* searchDocIn, ECTextViewImp* textViewImpIn);
    ~ECBackspaceHandlerSearch(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECSearchDoc* searchDoc;
    ECTextViewImp* textViewImp;
};

//Class that handles the ESC key in search mode
//Switches the editor from search mode to edit mode
class ECESCKeyHandlerSearch : public ECKeyHandlerBasic{
public:
    ECESCKeyHandlerSearch(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECSearchDoc* searchDocIn, ECHighlightText* highlightIn, ECTextViewImp* textViewImpIn);
    ~ECESCKeyHandlerSearch(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECSearchDoc* searchDoc;
    ECHighlightText* highlight;
    ECTextViewImp* textViewImp;
};

//*********************
//*Replace Key Handlers
//*********************

//Class that handles ASCII letter inputs in replace mode
class ECLetterKeyHandlerReplace : public ECKeyHandlerBasic{
public:
    ECLetterKeyHandlerReplace(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECReplace* replaceDocIn, ECTextViewImp* textViewImpIn);
    ~ECLetterKeyHandlerReplace(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECReplace* replaceDoc;
    ECTextViewImp* textViewImp;
};

//Class that handles the backspace key input in replace mode
class ECBackspaceHandlerReplace : public ECKeyHandlerBasic{
public:
    ECBackspaceHandlerReplace(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECReplace* replaceDocIn, ECTextViewImp* textViewImpIn);
    ~ECBackspaceHandlerReplace(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECReplace* replaceDoc;
    ECTextViewImp* textViewImp;
};

//Class that handles the enter key input in replace mode
//Runs the replace and switches the editor to search mode
class ECEnterKeyHandlerReplace : public ECKeyHandlerBasic{
public:
    ECEnterKeyHandlerReplace(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECReplace* replaceDocIn, ECSearchDoc* searchDocIn, ECCommandHistory* cmdHistoryIn, ECCursorControl* cursorControlIn, ECHighlightText* highlightTextIn, ECTextViewImp* textViewImpIn);
    ~ECEnterKeyHandlerReplace(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECReplace* replaceDoc;
    ECSearchDoc* searchDoc;
    ECCommandHistory* cmdHistory;
    ECCursorControl* cursorControl;
    ECHighlightText* highlightText;
    ECTextViewImp* textViewImp;
};

//Class that handles the ESC key input in Replace mode
//Switches the editor from replace mode to search mode
class ECESCKeyHandlerReplace : public ECKeyHandlerBasic{
public:
    ECESCKeyHandlerReplace(ECKeyHandlerBasic* nextHandlerIn, ECTextDocCtrl* textDocCtrlIn, ECReplace* replaceDocIn, ECTextViewImp* textViewImpIn);
    ~ECESCKeyHandlerReplace(){}
    virtual void Update(){}
    virtual bool Handle(int keyPressed);
private:
    ECTextDocCtrl* textDocCtrl;
    ECReplace* replaceDoc;
    ECTextViewImp* textViewImp;
};

#endif