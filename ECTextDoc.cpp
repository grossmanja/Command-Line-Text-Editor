#include "ECTextDoc.h"
#include <iostream>
using namespace std;

ECTextDoc :: ECTextDoc(int rowsInViewIn) : rowsInView(rowsInViewIn) {
    numRows = 0;
    createNewRow(0,0);
}

//ECTextDoc is responsible for freeing the RowsOfText objects
ECTextDoc :: ~ECTextDoc(){
    for(int i = 0; i < (int)documentText.size(); i++){
        delete documentText[i];
    }
}

//One of the most used functions, it converts the page, row system to the actual index in documentText
//Since a "page" is just a portion of documentText, we can calculate what the actual location in documentText we want to go
//For example, if the view can show 10 rows, then the first page is from rows 0-9, the second page is 10-19, and so on,
//So if we want row 2 (third row from the top) on page 2, then we do 1 * 10 () + 2 = 12, which is the actual index in documentText that we
//Need to go to find the text that we want.
int ECTextDoc :: getRealRowNum(int y, int pageIndex) const{
    return pageIndex * rowsInView + y;
}

char ECTextDoc :: getCharAt(int x, int y, int pageIndex) const{
    return documentText[getRealRowNum(y, pageIndex)]->getCharAt(x);
}

int ECTextDoc :: getTotalNumRows() const{
    return numRows;
}

int ECTextDoc :: getNumRowsInPage(int pageIndex) const{
    //If there's a row on the next page, then we know that this page must have every row filled, so just return the number of rows in the view
    if(getNumCols(0, pageIndex + 1) != -1) return rowsInView;
    int realRowNum = getRealRowNum(0, pageIndex);
    //If that's not the case, then we must be at the end of the text document, so just return the differnce in total rows and the first row on the last page
    //Since the total row will be the last row, we just subtract the two and that's the number of rows on the last page
    return(numRows - realRowNum);
}

int ECTextDoc :: getNumCols(int y, int pageIndex) const{
    if(y < 0 || pageIndex < 0 || getRealRowNum(y, pageIndex) > numRows - 1) return -1;
    
    return (int)documentText[getRealRowNum(y, pageIndex)]->getRowLen();
}

//Check if a page exists by seeing if there are any rows on the first line of that page
bool ECTextDoc :: getPageExists(int pageIndex) const{
    return getNumCols(0, pageIndex) != -1;
}

//Uses integer division between the total number of rows and the total rows in view to calculate the number of pages there are
//Like if rowsInView = 5, and we are using 3 lines, then 3//5 = 0, 0 + 1 = 1, so we know we have one page 
//However, if the page is full, then integer division fails.  If rowsInView = 5, and we fill all five rows on the first page, then 5//5 = 1, 1 + 1 = 2, which is incorrect, since there still is only one page
//So we need to check if getTotalNumRos is divisible by rowsInView, and adjust our calculation accordingly
int ECTextDoc :: getNumPages() const{
    if(getTotalNumRows() % rowsInView == 0){
        return getTotalNumRows() / rowsInView;
    }
    return getTotalNumRows() / rowsInView + 1;
}

string ECTextDoc :: getRowText(int row, int pageIndex) const{
    return documentText[getRealRowNum(row, pageIndex)]->getWholeRow();
}

bool ECTextDoc :: insertCharAt(int x, int y, int pageIndex, const char input){
    if(x > documentText[getRealRowNum(y, pageIndex)]->getRowLen()) return false;
    
    documentText[getRealRowNum(y, pageIndex)]->insertCharAt(x, input);
    return true;
}

int ECTextDoc :: deleteCharAt(int x, int y, int pageIndex){
    int realRowNum = getRealRowNum(y, pageIndex);
    if(x > documentText[realRowNum]->getRowLen()) return -1;
    
    documentText[realRowNum]->removeCharAt(x);
    return 1;
    
}

int ECTextDoc :: deleteRow(int y, int pageIndex){
    int returnValue;
    int realRowNum = getRealRowNum(y, pageIndex);
    //If we're at the beginning of the document, then we can't delete this row
    if(realRowNum == 0){
        return -1;
    } 
    //Check if there's any text on the row that we're deleting, if there isn't any, then just delete the row
    if(documentText[realRowNum]->getRowLen() == 0){
        //Making sure that we free any RowOfText objects that we create 
        delete documentText[realRowNum];
        documentText.erase(documentText.begin() + realRowNum);
        returnValue = 2;
    }//If there is text though, then move the text to the previous row
    else{
        string temp = documentText[realRowNum]->getWholeRow();
        int prevRowLen = documentText[realRowNum - 1]->getRowLen();
        for(int i = 0; i < (int)temp.length(); i++){
            documentText[realRowNum - 1]->insertCharAt(prevRowLen, temp[i]);
            prevRowLen++;
        }
        delete documentText[realRowNum];
        documentText.erase(documentText.begin() + realRowNum);
        returnValue = 3;
    }
    numRows--;
    return returnValue;
}

int ECTextDoc :: createNewRow(int y, int pageIndex){
    ECRowOfText* temp = new ECRowOfText();
    documentText.insert(documentText.begin() + getRealRowNum(y, pageIndex), temp);
    numRows++;
    return 1;
}

//splitIntoNewRow is when you press enter in the middle of a row and split the text into two rows, one of them being a new row
//Hence the name, splitIntoNewRow, very creative
bool ECTextDoc :: splitIntoNewRow(int x, int y, int pageIndex){
    //First, create a new row to move the split text into
    createNewRow(y+1, pageIndex);
    int realRowNum = getRealRowNum(y, pageIndex);
    vector<char> tempSplit;
    int length = documentText[realRowNum]->getRowLen();
    
    //Store the text that we are moving to the next row, and delete it from the current row
    //Why I did a vector<char> instead of just a string, i will never know, but hey it works just fine 
    for(int i = x; i < length; i++){
        tempSplit.push_back(documentText[realRowNum]->getCharAt(x));
        documentText[realRowNum]->removeCharAt(x);
    }
    //Then, insert it into the new row
    for(int i = 0; i < (int)tempSplit.size(); i++){
        documentText[realRowNum+1]->insertCharAt(i, tempSplit[i]);
    }
    return true;
}

//Sort of like splitIntoNewRow, but instead of moving the text to a new row, you just return the removed text
string ECTextDoc :: splitRow(int x, int y, int pageIndex){  
    string tempSplit;
    int realRowIndex = getRealRowNum(y, pageIndex);
    int length = documentText[realRowIndex]->getRowLen();
    for(int i = x; i < length; i++){
        tempSplit += documentText[realRowIndex]->getCharAt(x);
        documentText[realRowIndex]->removeCharAt(x);
    }
    
    return tempSplit;
}

//Command to delete a row of text, but without moving the text or doing anything fancy, just deleteing it
//Completely delete since we're just getting rid of any text on it, no return since I previously had a command
//called completelyDeleteRow() that also returned the text, so this one is a void function that has no return.
//This command is only really used by ECFileReading to get rid of the extra empty line.
void ECTextDoc :: completelyDeleteRowNoReturn(int y, int pageIndex){
    if(y < 0 || getRealRowNum(y, pageIndex) >= numRows){
        return;
    } 
    int realRowNum = getRealRowNum(y, pageIndex);
    delete documentText[realRowNum];
    documentText.erase(documentText.begin() + realRowNum);
    numRows--;
    return;
}