#include "ECTextDocCtrl.h"
#include <iostream>
#include <cstdio>
using namespace std;

ECTextDocCtrl :: ECTextDocCtrl(ECTextDoc* textDocIn, ECTextViewImp* textViewImpIn, ECStatusRow* statusRowIn) : textDoc(textDocIn), textViewImp(textViewImpIn), statusRow(statusRowIn){
    currentIndex = 0;
    myRefreshState = true;
    if(textDoc != NULL){
        myRefresh();
    }
}

int ECTextDocCtrl :: getCurrentIndex() const{
    return currentIndex;
}

bool ECTextDocCtrl :: changeCurrentPage(int y){
    if(y > getNumPages() - 1 || y < 0){ 
        return false;
    }
    currentIndex = y;
    myRefresh();
    updateStatusRow();
    return true; 
}

bool ECTextDocCtrl :: changeNextPage(){
    return changeCurrentPage(currentIndex + 1);
}

bool ECTextDocCtrl :: changePreviousPage(){
    return changeCurrentPage(currentIndex - 1);
}

int ECTextDocCtrl :: createNewRow(int x, int y){
    int returnValue = createNewRow(x, y, currentIndex);
    myRefresh();
    updateStatusRow();
    return returnValue;
}

int ECTextDocCtrl :: createNewRow(int x, int y, int pageIndex){
    //If the cursor/x value is in the middle of the row, then do splitIntoNewRow instead
    if(x > 0 && x < textDoc->getNumCols(y, pageIndex)){
        textDoc->splitIntoNewRow(x, y, pageIndex);
        myRefresh();
        updateStatusRow();
        return 3;
    }
    //If the cursor/x value is at the beginning of the row, then create a new row at the current line instead
    //This way, it moves all of the text down into the next line
    if(x == 0){
        int returnValue = textDoc->createNewRow(y, pageIndex);
        myRefresh();
        updateStatusRow();
        return returnValue;
    }
    //Otherwise, create a new row on the next line
    textDoc->createNewRow(y+1, pageIndex);
    updateStatusRow();
    myRefresh();
    return 1;
}
//Updates the status row with the current page, and the total number of pages while in Edit mode
//Called whenever we add a new line, delete a line, or change pages
bool ECTextDocCtrl :: updateStatusRow(){
    string temp = "Page: " + to_string(getCurrentIndex() + 1) + "/" + to_string(getNumPages());
    statusRow->changeStatusRow(temp, "Edit Mode");
    return true;
}

void ECTextDocCtrl :: myRefresh(){
    //First, check if we've disabled myRefresh, only done by ECFileReading
    if(!myRefreshState){
        return;
    }
    //Clear all rows
    textViewImp->InitRows();
    //Then, print out all of the rows on the current page
    for(int i = 0; i < getNumRowsInPage(); i++){
        textViewImp->AddRow(textDoc->getRowText(i, currentIndex));
    }
    textViewImp->Refresh();
}

//Enables/disables myRefresh
void ECTextDocCtrl :: switchMyRefresh(bool state){
    myRefreshState = state;
}