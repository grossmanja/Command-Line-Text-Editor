#ifndef ECSEARCHDOC_H
#define ECSEARCHDOC_H

class ECSearchDoc;

#include "ECTextDocCtrl.h"
#include "ECStatusRow.h"
#include "ECKeyObserver.h"

//Class that searches the document for instances of a chosen word/phrase
class ECSearchDoc{
public:
    ECSearchDoc(ECTextDocCtrl* textDocCtrlIn, ECStatusRow* statusRowIn);
    //Command that sets keyObserverHandler
    //Needed because we need to initialize ECSearchDoc before we initialize keyObserverHandler
    void setKeyObserverHandler(ECKeyObserverHandler* keyObserverHandlerIn);
    //Command that changes the status row to say "Edit mode" and clears textToSearch
    void initializeSearch();
    //Command that returns textToSearch
    std::string getTextToSearch();
    //Command to add a letter to textToSearch
    //Used when the user types in a character
    //Updates the status row when this occurs
    void addLetterToTextToSearch(char input);
    //Command to remove a letter from textToSearch
    //Used when the user hits backspace
    //Updates the status row when this occurs
    void removeLetterFromTextToSearch();
    //Command to completely change textToSearch with a new string
    //Used in turnOffSearch to clear textToSearch
    void changeWholeTextToSearch(std::string inputString);

    //Command that returns a vector of (x,y) coordinates [stored as vectors]
    //The coordinates store the position of the first character in textToSearch
    //Returns [[-1,-1]] if no instances of the search term is found on the current page
    //Stores the result in locations
    std::vector<std::vector<int>> findTextLocations();
    //Command that returns locations, does not perform the search itself
    std::vector<std::vector<int>> getTextLocations();
    //Command that turns of Search mode and switches to Edit mode
    void turnOffSearch();
private:
    ECTextDocCtrl* textDocCtrl;
    ECKeyObserverHandler* keyObserverHandler;
    ECStatusRow* statusRow;

    std::string textToSearch;                   //String to search on the page
    std::vector<std::vector<int>> locations;    //the (x,y) coordinates of all the instances of the search term on the page
    //int locationsLen;
};

#endif