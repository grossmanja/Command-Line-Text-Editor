#ifndef ECTEXTDOCCTRL_H
#define ECTEXTDOCCTRL_H
#include "ECTextViewImp.h"
#include "ECTextDoc.h"
#include "ECStatusRow.h"

//The class is split into 2 .cpp files, ECTextDocCtrl and ECTextDocCtrlPassthrough

//The Controller in the MVC design pattern
//The main interface for ECTextDoc and ECTextViewImp, although many classes interact with ECTextViewImp directly
//Unfortunately, I did not have time to decouple the other classes with ECTextViewImp
class ECTextDocCtrl{
public:
    ECTextDocCtrl(ECTextDoc* textDocIn, ECTextViewImp* textViewImpIn, ECStatusRow* statusRowIn);
    virtual ~ECTextDocCtrl(){}
    
    
    //Command to get the current page index
    int getCurrentIndex() const;
    //Command that returns true if that page exists (there's enough rows to go to that page)
    bool getPageExists(int y) const;
    //Command to get the string of text at a specified row at a specified page index
    std::string getRowTextatPage(int row, int currentIndex) const;
    //Command to update that status row with the current page index and current page count
    bool updateStatusRow();
    //Command to change the index to the next page (index + 1)
    bool changeNextPage();
    //Command to change the index to the previous page (index - 1)
    bool changePreviousPage();
    //Command to change index to specified page
    bool changeCurrentPage(int y);
    //Command to create a new ECRowOfText and insert it into the document at row y
    int createNewRow(int x, int y);
    //Command to create a new ECRowOfText and insert it into a specified page at row y
    int createNewRow(int x, int y, int pageIndex);
    //Command to split the row into a new line at the cursor
    bool splitIntoNewRow(int y, int pageIndex);
    //Command to split the row into a new line at given x location
    bool splitIntoNewRow(int x, int y, int pageIndex);
    //Command to remove the text
    std::string splitRow(int x, int y);
    //Command to create a new page after the current currentIndex
    bool createNewPage();
    //Command to create a new page at a specific idnex
    bool createNewPage(int indexNew);
    //My implementation of ECTextViewImp's Refresh() function
    void myRefresh();
    //Command to enable or disable myRefresh().  Disables myRefresh  when reading from the intitial text file and re-enables when done.
    void switchMyRefresh(bool state);

    //*These functions are passed to textDoc without anything done by ECTextDocCtrl

    //Command to get the number of pages in the document
    int getNumPages() const;
    //Command to get char at x,y location on current page
    char getCharAt(int x, int y) const;
    //Get the number of rows used for the entire document
    int getTotalNumRows() const;
    //Get the number of rows on specified page
    int getNumRowsInPage(int currentIndex) const;
    //Get the number of rows on current page
    int getNumRowsInPage() const;
    //Get the number of columns in a row on specified page
    int getNumCols(int y) const;
    //Get the number of columns in a row on specified page
    int getNumCols(int y, int pageIndex) const;
    //Command to insert one character at the cursor location
    bool insertCharAtCursor(const char input);
    //Command to insert one character at given x,y location on current page
    bool insertCharAt(int x, int y, const char input);
    //Command to insert one character at given x,y location on a specified page currentIndex
    bool insertCharAt(int x, int y, const char input, int pageIndex);
    //Command to delete one character at x,y location on current page
    int deleteCharAt(int x, int y);
    //Command to delete one character at x,y location at specfic page currentIndex
    int deleteCharAt(int x, int y, int pageIndex);
    //Command to delete an entire row of text on current page
    int deleteRow(int y);
    //Command to delete an entire row of text on a specified page
    int deleteRow(int y, int pageIndex);
    //Command to completely delete a row of text with no return
    void completelyDeleteRowNoReturn(int y);


private:

    ECTextDoc* textDoc;         //The model in the MVC design pattern
    int currentIndex;           //The current page index
    int rowsInView;             //The number of rows in the view, obtained from ECTextViewImp
    bool myRefreshState;        //Variable that controls whether myRefresh is enabled or disabled
    ECTextViewImp* textViewImp; //The view in the MVC design pattern
    ECStatusRow* statusRow;     //The object that controls the status row
};
#endif