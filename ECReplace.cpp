#include "ECReplace.h"
#include <iostream>
using namespace std;

ECReplace :: ECReplace(ECTextDocCtrl* textDocCtrlIn, ECStatusRow* statusRowIn) : textDocCtrl(textDocCtrlIn), statusRow(statusRowIn) { 
    textToReplace = "";
    textToRemove = "";
}

void ECReplace :: initalizeReplace(){
    statusRow->changeStatusRow("", "Replace Mode");
    textToReplace = "";
}

void ECReplace :: setKeyObserverHandler(ECKeyObserverHandler* keyObserverHandlerIn){
    keyObserverHandler = keyObserverHandlerIn;
}

void ECReplace :: addLetterToTextToReplace(char input){
    if(textToReplace == ""){
        //Not sure if this is necessary, but was having issues with initializing textToReplace, saw that doing this could help
        string tempString(1, input);
        textToReplace = tempString;
    }else{
        textToReplace += input;
    }
    statusRow->changeStatusRow(textToReplace, "Replace Mode");
}

void ECReplace :: removeLetterFromTextToReplace(){
    if((int)textToReplace.length() > 0){
        textToReplace.pop_back();
        statusRow->changeStatusRow(textToReplace, "Replace Mode");
    }
}

void ECReplace :: changeWholeTextToReplace(string stringInput){
    textToReplace = stringInput;
}

string ECReplace :: getTextToReplace(){
    return textToReplace;
}

void ECReplace :: turnOffReplace(){
    statusRow->changeStatusRow("", "Search Mode");
    changeWholeTextToReplace("");
    keyObserverHandler->changeReplaceMode(false);
}

bool ECReplace :: replaceText(string toRemove, string toReplace, bool sameLocations, int removeLength, int replaceLength, vector<vector<int>> locations){
    bool returnValue = replaceText(toRemove, toReplace, sameLocations, removeLength, replaceLength, locations, textDocCtrl->getCurrentIndex());
    textDocCtrl->myRefresh();
    return returnValue;
}

//Replace text works by first inserting the text that we are replacing with, starting at the index of the word that we're replacing, then deleting the old word
//So, if the text is:
//red red
//And we want to replace "red" with "blue", we would replace like this:
//bluered red
//blue red
//blue bluered
//blue blue
bool ECReplace :: replaceText(string toRemove, string toReplace, bool sameLocations, int removeLength, int replaceLength, vector<vector<int>> locations, int pageIndex){

    //If there's no locations of things to replace, then just return
    if(locations[0][0] == -1) return false;
    //Find the difference in length between what we're removing and what we're replacing it with
    int differenceInLength = replaceLength - removeLength;
    //Need to keep track of how many things we replaced in the same row, if the text we're replacing is a different length than the text we're removing
    //Then the locations of text in locations will be offset, so we need to account for that
    //For example, if we replace red with blue, them the locations return for this row
    //red red
    //will be [(0,0),(4,0)], but once we replace the first red with blue, then the start of the second red will move to (5,0), so that's what counterInSameRow is account for
    int counterInSameRow = 0;
    int prevRow = -1;

    for(int i = 0; i < (int)locations.size(); i++){
        //Get x,y locations, and check if we're on the same row as the previous iteration
        //If we are, then increment counterInSameRow, otherwise restart the counter
        int x = locations[i][0];
        int y = locations[i][1];
        if(prevRow == y){
            counterInSameRow++;
        }else{
            counterInSameRow = 0;
        }

        //Since we use the same vector for when we undo and redo this change, we need to account for the fact that when undoing this replace, we don't need to account for offsets
        //Going off the previous example, after the replace, our line is now
        //blue blue
        //But, locations is still [(0,0),(4,0)]
        //So, when we undo, we don't need to account for the difference in length or anything.  That's what the sameLocations boolean is for
        //If we're undoing the change, sameLocations will be false, and we can just insert like normal
        //Otherwise, we have to account for the different in length like in the previous red blue example, which is the else in this statement.
        for(int j = 0; j < replaceLength; j++){
            if(!sameLocations){
                textDocCtrl->insertCharAt(x + j, y, toReplace[j], pageIndex);
            }else{
                textDocCtrl->insertCharAt(x + j + (counterInSameRow * differenceInLength), y, toReplace[j], pageIndex);
            }
            
        }

        //Same idea as before with sameLocations, we don't need to account for the difference of length or how many times it occurs, just remove the old text
        //If not, then account for the difference in length in the different text and if we've done multiple changes in this row
        for(int j = 0; j < removeLength; j++){
            if(!sameLocations){
                textDocCtrl->deleteCharAt(x + replaceLength, y, pageIndex);
            }else{
                
                textDocCtrl->deleteCharAt(x + replaceLength + (counterInSameRow * differenceInLength), y, pageIndex);
            }
        }
        //Set prevRow to our current row before the next iteration 
        prevRow = y;
    }
    textDocCtrl->myRefresh();
    return true;
}