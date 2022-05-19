#include "ECTextDocCtrl.h"
using namespace std;





char ECTextDocCtrl :: getCharAt(int x, int y) const{
    return textDoc->getCharAt(x, y, currentIndex);
}

string ECTextDocCtrl :: getRowTextatPage(int row, int pageIndex) const{
    return textDoc->getRowText(row, pageIndex);
}

int ECTextDocCtrl :: getNumPages() const{
    return textDoc->getNumPages();
}

bool ECTextDocCtrl :: getPageExists(int pageIndex) const{
    return textDoc->getPageExists(pageIndex);
}

int ECTextDocCtrl :: getTotalNumRows() const{
    return textDoc->getTotalNumRows();
}

int ECTextDocCtrl :: getNumRowsInPage(int pageIndex) const{
    if(currentIndex >= getNumPages()) return -1; 
    return textDoc->getNumRowsInPage(pageIndex);
}

int ECTextDocCtrl :: getNumRowsInPage() const{
    return getNumRowsInPage(currentIndex);
}

int ECTextDocCtrl :: getNumCols(int y) const{
    return textDoc->getNumCols(y, currentIndex);
}

int ECTextDocCtrl :: getNumCols(int y, int pageIndex) const{  
    return textDoc->getNumCols(y, pageIndex);
}

bool ECTextDocCtrl :: insertCharAtCursor(const char input){
    bool returnValue =  textDoc->insertCharAt(textViewImp->GetCursorX(), textViewImp->GetCursorY(), currentIndex, input);
    myRefresh();
    return returnValue;
}

bool ECTextDocCtrl :: insertCharAt(int x, int y, const char input){
    bool returnValue =  textDoc->insertCharAt(x, y, currentIndex, input);
    myRefresh();
    return returnValue;
}

bool ECTextDocCtrl :: insertCharAt(int x, int y, const char input, int pageIndex){
    bool returnValue = textDoc->insertCharAt(x, y, pageIndex, input);
    myRefresh();
    return returnValue;
}

int ECTextDocCtrl :: deleteCharAt(int x, int y){
    int returnValue = textDoc->deleteCharAt(x, y, currentIndex);
    myRefresh();
    return returnValue;
}

int ECTextDocCtrl :: deleteCharAt(int x, int y, int pageIndex){
    int returnValue = textDoc->deleteCharAt(x, y, pageIndex);
    myRefresh();
    return returnValue;
}

bool ECTextDocCtrl :: splitIntoNewRow(int y, int pageIndex){
    bool returnValue = textDoc->splitIntoNewRow(textViewImp->GetCursorX(), y, pageIndex);
    myRefresh();
    return returnValue;
}

bool ECTextDocCtrl :: splitIntoNewRow(int x, int y, int pageIndex){
    bool returnValue = textDoc->splitIntoNewRow(x, y, pageIndex);
    myRefresh();
    return returnValue;
}

string ECTextDocCtrl :: splitRow(int x, int y){
    string returnValue = textDoc->splitRow(x, y, currentIndex);
    myRefresh();
    return returnValue;
}

int ECTextDocCtrl :: deleteRow(int y){
    int returnValue = textDoc->deleteRow(y, currentIndex);
    myRefresh();
    updateStatusRow();
    return returnValue;
}

int ECTextDocCtrl :: deleteRow(int y, int pageIndex){
    int returnValue = textDoc->deleteRow(y, pageIndex);
    myRefresh();
    updateStatusRow();
    return returnValue;
}

void ECTextDocCtrl :: completelyDeleteRowNoReturn(int y){
    textDoc->completelyDeleteRowNoReturn(y, currentIndex);
    myRefresh();
}