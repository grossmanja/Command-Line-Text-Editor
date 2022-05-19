#include "ECCommands.h"
#include <iostream>

using namespace std;

ECCommands :: ~ECCommands(){}


//********************
//*Insert Text Command
//********************

ECInsertTextCommand :: ECInsertTextCommand(ECTextDocCtrl* doc, int xIn, int yIn, const char inputIn) : textDocCtrl(doc), x(xIn), y(yIn), input(inputIn){
    //For each command, we save the current page index in case we undo/redo on a different page
    pageNum = textDocCtrl->getCurrentIndex();
}

int ECInsertTextCommand :: execute(){
    textDocCtrl->insertCharAt(x, y, input, pageNum);
    return 1;
}

int ECInsertTextCommand :: unexecute(){
    textDocCtrl->deleteCharAt(x, y, pageNum);
    return 1;
}

//********************
//*Remove Text Command
//********************

ECRemoveTextCommand :: ECRemoveTextCommand(ECTextDocCtrl* doc, int xIn, int yIn) : textDocCtrl(doc), x(xIn), y(yIn){
    pageNum = textDocCtrl->getCurrentIndex();
}

int ECRemoveTextCommand :: execute(){
    //When we hit backspace, we are trying to delete the character one before the cursor, hence doing x-1 instead of x
    if(x-1 >= 0){
        input = textDocCtrl->getCharAt(x-1, y);
    }else{
        input = NULL;
    }

    return textDocCtrl->deleteCharAt(x - 1, y, pageNum);
    
}

int ECRemoveTextCommand :: unexecute(){
    return textDocCtrl->insertCharAt(x-1, y, input, pageNum);
    
}

//********************
//*Insert Row Command
//********************

ECInsertRowCommand :: ECInsertRowCommand(ECTextDocCtrl* doc, int xIn, int yIn) : textDocCtrl(doc), x(xIn), y(yIn){
    pageNum = textDocCtrl->getCurrentIndex();
}

int ECInsertRowCommand :: execute(){
    return textDocCtrl->createNewRow(x, y, pageNum);
    
}

int ECInsertRowCommand :: unexecute(){
    return textDocCtrl->deleteRow(y+1, pageNum);
}

//********************
//*Remove Row Command
//********************

ECRemoveRowCommand :: ECRemoveRowCommand(ECTextDocCtrl* doc, int xIn, int yIn) : textDocCtrl(doc), x(xIn), y(yIn){
    pageNum = textDocCtrl->getCurrentIndex();
    textToSplit = "";
}
int ECRemoveRowCommand :: execute(){
    string temp = textDocCtrl->getRowTextatPage(y, pageNum);
    textToSplit = temp.substr(x);
    return textDocCtrl->deleteRow(y, pageNum);
}
//RemoveRowCommand's unexecute is much more complex since deleteRow was not designed with being reversible in mind
int ECRemoveRowCommand :: unexecute(){
    int textToSplitLen = (int)textToSplit.length();
    //Check if we moved any text in the execute, if we didn't, then just create a new empty row like normal
    if(textToSplitLen == 0){
        return textDocCtrl->createNewRow(0, y, pageNum);
    }else{
        //Then we check if we are on the first line of a page and if there's a previous page, if so, then we need to split the text from the last line of that page
        if(y == 0 && textDocCtrl->getPageExists(pageNum - 1)){
            //The calculation done to figure out where to split the row is (length of text on line) - (length of textToSplit), which gives the location on the line that was originally on its own line
            //for example, if the lines are initially:
            //hello
            //yellow
            //and we press backspace on line two, we get:
            //helloyellow
            //then when we undo, we need to do (length of text on line (11)) - (length of textToSplit(6)) = 5, which is the first letter of "yellow"
            return textDocCtrl->splitIntoNewRow(textDocCtrl->getNumCols(textDocCtrl->getNumRowsInPage(pageNum - 1) - 1, pageNum - 1) - textToSplitLen, textDocCtrl->getNumRowsInPage(pageNum - 1) - 1, pageNum - 1);
        }else{
            //If we aren't on the first line, then we split the previous row using the same calculation as before
            return textDocCtrl->splitIntoNewRow(textDocCtrl->getNumCols(y - 1, pageNum) - textToSplitLen, y - 1, pageNum);
        }
    }
    return -1;
}

ECReplaceTextCommand :: ECReplaceTextCommand(ECTextDocCtrl* docIn, ECReplace* replaceDocIn, std::vector<std::vector<int>> locationsIn, std::string textToRemoveIn, std::string textToReplaceIn) : textDocCtrl(docIn), replaceDoc(replaceDocIn), locations(locationsIn), textToRemove(textToRemoveIn), textToReplace(textToReplaceIn) {
    pageNum = textDocCtrl->getCurrentIndex();
    
}

int ECReplaceTextCommand :: execute(){
    return replaceDoc->replaceText(textToRemove, textToReplace, true, (int)textToRemove.length(), (int)textToReplace.length(), locations, pageNum);
}

int ECReplaceTextCommand :: unexecute(){
    return replaceDoc->replaceText(textToReplace, textToRemove, false, (int)textToReplace.length(), (int)textToRemove.length(), locations, pageNum);
}