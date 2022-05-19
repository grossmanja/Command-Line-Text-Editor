#ifndef ECREPLACE_H
#define ECREPLACE_H

class ECReplace;

#include "ECKeyObserver.h"
#include "ECSearchDoc.h"
#include "ECTextDocCtrl.h"

//Class that handles the replacement of text in the document
//Takes the input from ECSearchDoc and uses that as the text to remove
class ECReplace{
public:
    ECReplace(ECTextDocCtrl* textDocCtrlIn, ECStatusRow* statusRowIn);
    //Changes the status row to say "Replace mode"
    //Also clears textToReplace for the user to type in
    void initalizeReplace();
    //Command to set keyObserverHandler
    //Needed because we need to initialize ECReplace before keyObserverHandler
    void setKeyObserverHandler(ECKeyObserverHandler* keyObserverHandlerIn);
    //Command to add a letter to textToReplace
    //Used when the user types in a character
    //Updates the status row when this occurs
    void addLetterToTextToReplace(char input);
    //Command to remove a letter from textToReplace
    //Used when the user hits backspace
    //Updates the status row when this occurs
    void removeLetterFromTextToReplace();
    //Command to completely change textToReplace with a new string
    //Used in turnOffReplace to clear textToReplace
    void changeWholeTextToReplace(std::string inputString);
    //Command that returns textToReplace
    std::string getTextToReplace();
    //Command that executes the replacement of instances of textToRemove with textToReplace at current page index
    //Uses the locations gathered from ECSearchDoc and the search term from ECSearchDoc for textToRemove 
    bool replaceText(std::string toRemove, std::string toReplace, bool sameLocations, int removeLength, int replaceLength, std::vector<std::vector<int>> locations);
    //Command that executes the replacement of instances of textToRemove with textToReplace at a specified page index
    //Uses the locations gathered from ECSearchDoc and the search term from ECSearchDoc for textToRemove 
    bool replaceText(std::string toRemove, std::string toReplace, bool sameLocations, int removeLength, int replaceLength, std::vector<std::vector<int>> locations, int pageIndex);
    //Command to turn off Replace mode and switch to Search mode
    void turnOffReplace();
private:
    ECTextDocCtrl* textDocCtrl;
    ECKeyObserverHandler* keyObserverHandler;
    ECStatusRow* statusRow;
    ECSearchDoc* searchDoc;
    ECCommandHistory* cmdHistory; 

    std::string textToRemove;       //Text that is currently in the document that will be removed
    std::string textToReplace;      //Text that will be inserted into the document as replacement to textToRemove
};

#endif