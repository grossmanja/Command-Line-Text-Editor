#include "ECSearchDoc.h"
#include <iostream>
using namespace std;

ECSearchDoc :: ECSearchDoc(ECTextDocCtrl* textDocCtrlIn, ECStatusRow* statusRowIn) : textDocCtrl(textDocCtrlIn), statusRow(statusRowIn) {
    textToSearch = "";
}

void ECSearchDoc :: initializeSearch(){
    statusRow->changeStatusRow("", "Search Mode");
    textToSearch = "";
}

void ECSearchDoc :: setKeyObserverHandler(ECKeyObserverHandler* keyObserverHandlerIn){
    keyObserverHandler = keyObserverHandlerIn;
}

string ECSearchDoc :: getTextToSearch(){
    return textToSearch;
}

void ECSearchDoc :: addLetterToTextToSearch(char input){
    if(textToSearch == ""){
        //Not sure if this is necessary, but was having issues with initializing textToSearch, saw that doing this could help
        string tempString(1, input);
        textToSearch = tempString;
    }else{
        textToSearch += input;
    }
    statusRow->changeStatusRow(textToSearch, "Search Mode");
}

void ECSearchDoc :: removeLetterFromTextToSearch(){
    if((int)textToSearch.length() > 0){
        textToSearch.pop_back();
        statusRow->changeStatusRow(textToSearch, "Search Mode");
    }
}

void ECSearchDoc :: changeWholeTextToSearch(string textToChange){
    textToSearch = textToChange;
    statusRow->changeStatusRow(textToSearch, "Search Mode");
}

vector<vector<int>> ECSearchDoc :: getTextLocations(){
    return locations;
}

vector<vector<int>> ECSearchDoc :: findTextLocations(){
    locations.clear();
    vector<int> tempVector;
    //If the document is empty, or if the text that we're searching is empty, then just return [[-1,-1]]
    if((textDocCtrl->getNumRowsInPage() == 1 && textDocCtrl->getNumCols(0) == 0) || ((int)textToSearch.length() == 0)){
        tempVector.push_back(-1);
        tempVector.push_back(-1);
        locations.push_back(tempVector);
        return locations;
    }

    int rows = textDocCtrl->getNumRowsInPage();
    int cols = textDocCtrl->getNumCols(0);
    int textIndex = 0;
    int currentPage = textDocCtrl->getCurrentIndex();
    string rowText = "";

    for(int i = 0; i < rows; i++){
        //Get the text at each row, and just call string.find()
        //Makes sure that we don't have overlapping text, like finding two instances of aa in aaa
        rowText = textDocCtrl->getRowTextatPage(i, currentPage);
        cols = textDocCtrl->getNumCols(i);
        textIndex = 0;
        //While we're still finding instances of textToSearch in this row of text 
        while(textIndex != (long)string::npos && textIndex < cols){
            textIndex = rowText.find(textToSearch, textIndex);
            if(textIndex == (long)string::npos){
                break;
            }
            //Make a vector of the x,y location and add that to locations,
            tempVector.clear();
            tempVector.push_back(textIndex);
            tempVector.push_back(i); 
            locations.push_back(tempVector);
            textIndex += (int)textToSearch.length();
        }
    }
    //If we searched the whole page and we couldn't find anything, then return [[-1,-1]] 
    if(locations.size() == 0){
        tempVector.push_back(-1);
        tempVector.push_back(-1);
        locations.push_back(tempVector);
    }
    return locations;
}

void ECSearchDoc :: turnOffSearch(){
    textDocCtrl->updateStatusRow();
    keyObserverHandler->changeSearchMode(false);
}