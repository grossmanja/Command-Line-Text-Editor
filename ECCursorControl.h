#ifndef ECCURSORCONTROL_H
#define ECCURSORCONTROL_H

#include "ECTextViewImp.h"
#include "ECTextDocCtrl.h"

//Class that controls how the cursor moves about on the screen
class ECCursorControl{
public:
    ECCursorControl(ECTextViewImp* docIn, ECTextDocCtrl* textViewIn);
    virtual ~ECCursorControl(){}

    //Command that moves the cursor to the origin (0,0)
    bool CursorInit();
    //Command that moves the cursor to the right by one increment
    //If the cursor is at the end of the line, then the cursor will move to the beginning of the next line
    //Will also move the view to the beginning of the next page if there is one
    bool MoveCursorRight();
    //Command that moves the cursor to the left by one increment
    //If the cursor is at the beginning of the line, then the cursor will move to the end of the previous line
    //Will also move the view to the end of the previous page if there is one
    bool MoveCursorLeft();
    //Command that moves the cursor up one row
    //Will move to the previous page if at the top of the page
    int MoveCursorUp();
    //Command that moves the cursor down one row
    //Will move to the next page if at the bottom of the page
    int MoveCursorDown();
    //Command to move cursor to next line at pos 0 when you create a new empty line or move right at end of a line
    bool MoveCursorDownAndLeft();
    //Command to move cursor to end of prev line when you delete an empty line
    bool MoveCursorUpAndRight();
    //Moves the cursor to a specified x,y location at the current page
    bool MoveCursorTo(const int x, const int y);
    //Moves  the cursor to a specified x,y location at a specified page
    bool MoveCursorTo(const int x, const int y, int pageIndex);
    //Moves the cursor to the nearest valid location
    //Used when undoing and redoing, where cursor location may change suddenly
    bool MoveCursorNearestValid();
private:
    ECTextViewImp* textViewImp;
    ECTextDocCtrl* textDocCtrl;
};

#endif