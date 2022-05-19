#include "ECCursorControl.h"
#include <iostream>
using namespace std;

ECCursorControl :: ECCursorControl(ECTextViewImp* textViewImpIn, ECTextDocCtrl* textDocCtrlIn) : textViewImp(textViewImpIn), textDocCtrl(textDocCtrlIn){}

bool ECCursorControl :: CursorInit(){
    textViewImp->SetCursorX(0);
    textViewImp->SetCursorY(0);
    return true;
}

bool ECCursorControl :: MoveCursorRight(){
    int currentX = textViewImp->GetCursorX();
    int currentY = textViewImp->GetCursorY();
    //Are we not on the edge of the line (in the middle of the line)?  Then just move the cursor right one
    if (currentX < textDocCtrl->getNumCols(currentY)){
        textViewImp->SetCursorX(currentX + 1);
        return true;
    //Else, check if there's a line below us.  If so, then move our cursor to the beginning of that line
    }else if(currentY < textDocCtrl->getNumRowsInPage() - 1){
        return MoveCursorDownAndLeft();
    //Else, check if there's a page afterwards that we can move our cursor to.  If so, change pages and move the cursor to the beginning of the page.
    }else if(textDocCtrl->getPageExists(textDocCtrl->getCurrentIndex() + 1)){
        textDocCtrl->changeNextPage();
        CursorInit();
    }
    //If none pass (cursor is at the very end of the document), then return false
    return false;
}

bool ECCursorControl :: MoveCursorLeft(){
    int currentX = textViewImp->GetCursorX();
    int currentY = textViewImp->GetCursorY();
    //Are we not on the edge of the line (in the middle of the line)?  Then just move the cursor left one
    if (currentX > 0){
        textViewImp->SetCursorX(currentX - 1);
        return true;
    //Else, check if there's a line above us.  If so, then move our cursor to the end of that line
    }else if(currentY > 0){
        return MoveCursorUpAndRight();
    //Else, check if there's a page before us that we can move our cursor to.  If so, change pages and move the cursor to the end of the page.
    }else if(textDocCtrl->getCurrentIndex() > 0){
        textDocCtrl->changePreviousPage();
        textViewImp->SetCursorY(textDocCtrl->getNumRowsInPage() - 1);
        textViewImp->SetCursorX(textDocCtrl->getNumCols(textDocCtrl->getNumRowsInPage() - 1));
    }
    return false;
}

//Only used when creating a new line, since we always move the cursor to the beginning of that row
bool ECCursorControl :: MoveCursorDownAndLeft(){
    int currentY = textViewImp->GetCursorY();
    //If there's no line to move down to, then change pages instead
    if(currentY == textViewImp->GetRowNumInView() - 1){
        textDocCtrl->changeNextPage();
        CursorInit();
    }else{
        textViewImp->SetCursorX(0);
        textViewImp->SetCursorY(currentY + 1);
    }
    return true;
}

//Used when we move the cursor all the way to the left and need to move to the end of the previous row;
bool ECCursorControl :: MoveCursorUpAndRight(){
    int current = textViewImp->GetCursorY();
    textViewImp->SetCursorX(textDocCtrl->getNumCols(current - 1));
    textViewImp->SetCursorY(current - 1);
    return true;
}

int ECCursorControl :: MoveCursorUp(){
    int currentX = textViewImp->GetCursorX();
    int currentY = textViewImp->GetCursorY();

    //If we're at the very beginning of the document, then just return false
    if(currentY == 0 && !textDocCtrl->getPageExists(textDocCtrl->getCurrentIndex() - 1)) return false;
    //Else, if we're at the beginning of a page and a previous page DOES exist, then change pages
    else if(currentY == 0 && textDocCtrl->getPageExists(textDocCtrl->getCurrentIndex() - 1)){
        textDocCtrl->changePreviousPage();
        if(textDocCtrl->getNumCols(textDocCtrl->getNumRowsInPage() - 1) < currentX){
            textViewImp->SetCursorX(textDocCtrl->getNumCols(textDocCtrl->getNumRowsInPage() - 1));
        }
        textViewImp->SetCursorY(textDocCtrl->getNumRowsInPage(textDocCtrl->getCurrentIndex() - 1) - 1);
        
        return 2;
    }
    //Otherwise, move the cursor up one row, making sure we don't move the cursor out of bounds
    int sizeRowAbove = textDocCtrl->getNumCols(currentY - 1);
    if(currentX >= sizeRowAbove){
        textViewImp->SetCursorX(sizeRowAbove);
    }
    textViewImp->SetCursorY(currentY - 1);
    return true;
}

int ECCursorControl :: MoveCursorDown(){
    int currentX = textViewImp->GetCursorX();
    int currentY = textViewImp->GetCursorY();

    //If we're at the very end of the document, then jst return false
    if(currentY == textViewImp->GetRowNumInView() - 1 && !textDocCtrl->getPageExists(textDocCtrl->getCurrentIndex() + 1)) return false;
    //Else, if we're at the end of a page, and a next page DOES exist, then change to that page
    else if(currentY == textViewImp->GetRowNumInView() - 1 && textDocCtrl->getPageExists(textDocCtrl->getCurrentIndex() + 1)){
        textDocCtrl->changeNextPage();
        if(textDocCtrl->getNumCols(0) < currentX){
            textViewImp->SetCursorX(textDocCtrl->getNumCols(0));
        }
        textViewImp->SetCursorY(0);
        return 2;
    }

    //Otherwise, move the cursor down one row, making sure that we don't go out of bounds
    int sizeRowBelow = textDocCtrl->getNumCols(currentY + 1);
    if(sizeRowBelow == -1){
        return -1;
    }
    if(currentX >= sizeRowBelow){
        textViewImp->SetCursorX(sizeRowBelow);
    }
    textViewImp->SetCursorY(currentY + 1);
    return true;
}

bool ECCursorControl :: MoveCursorTo(const int x, const int y){
    return MoveCursorTo(x, y, textDocCtrl->getCurrentIndex());
}

bool ECCursorControl :: MoveCursorTo(const int x, const int y, int pageIndex){
    if(x < 0 || x > textDocCtrl->getNumCols(y, pageIndex) || y < 0 || y > textDocCtrl->getNumRowsInPage(pageIndex)) return false;
    textViewImp->SetCursorX(x);
    textViewImp->SetCursorY(y);
    return true;
}

//Moves the cursor to the nearest valid position.  Tries to move the cursor as little as possible, moving the cursor up and to the left as little as possible
bool ECCursorControl :: MoveCursorNearestValid(){
    int currentX = textViewImp->GetCursorX();
    int currentY = textViewImp->GetCursorY();
    int currentIndex = textDocCtrl->getCurrentIndex();
    bool done = false;

    //First check if there's any text at this row.  If not, then try to move the cursor up to a row where there is text.  This check can switch pages
    if(textDocCtrl->getNumCols(currentY) == -1){
        while(currentIndex >= 0){
            for(int j = currentY; j >= 0; j--){
                if(textDocCtrl->getNumCols(j) >= 0){
                    //Once we've found a row with text, set currentY to that row and stop the loops
                    currentY = j;
                    done = true;
                    break;
                }
            }
            if(done) break;
            //If we've exhausted all rows on the current page, then swap to the next page and continue.
            textDocCtrl->changePreviousPage();
            currentIndex = textDocCtrl->getCurrentIndex();
            currentY = textDocCtrl->getNumRowsInPage() - 1;
        }
    }
    //There's an issue with my getNumCols() where it reports rows existing on the status row, so this is just a check
    //To make sure that we're not on the status row
    if(currentY == textDocCtrl->getNumRowsInPage()){
        currentY--;
    }

    //Then, check if our cursor's x position is out of bounds
    //If so, then move our cursor to the end of that row
    if(textDocCtrl->getNumCols(currentY) < currentX){
        currentX = textDocCtrl->getNumCols(currentY);
    }
    textViewImp->SetCursorX(currentX);
    textViewImp->SetCursorY(currentY);
    return true;
}