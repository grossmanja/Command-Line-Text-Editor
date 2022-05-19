#ifndef ECTEXTDOC_H
#define ECTEXTDOC_H
//#include "ECAllTextViews.h"
#include "ECTextViewImp.h"
#include "ECRowOfText.h"
//#include "ECKeyObserver.h"
#include <string.h>

//The model in the MVC design pattern, this class holds all of the RowsOfText objects for each row of text
class ECTextDoc{
public:
    ECTextDoc(int rowsInViewIn);
    virtual ~ECTextDoc();
    
    //Command to get character at coordinates x, y value on the specified page
    char getCharAt(int x, int y, int pageIndex) const;
    //Command to get the number of rows in the whole document
    int getTotalNumRows() const; 
    //Command to get the number of rows used in a specific page
    int getNumRowsInPage(int pageIndex) const;
    //Get the number of columns used on a specific page
    int getNumCols(int y, int pageIndex) const;
    //Command to check if page exists
    bool getPageExists(int pageIndex) const;
    //Command to get the total number of pages in the document
    int getNumPages() const;
    //Command to get the entire row of text on a specified page
    std::string getRowText(int row, int pageIndex) const;
    //Command to insert a character at an x,y location on a specified page
    bool insertCharAt(int x, int y, int pageIndex, const char input);
    //Command to delete one character at an x,y location on a specified page
    int deleteCharAt(int x, int y, int pageIndex);

    //Command to delete the ECRowOfText object at index y on page pageIndex.  Will move any characters on the deleted row to the previous row 
    int deleteRow(int y, int pageIndex);
    
    //Command to create a new ECRowOfText and insert it into documentText at row Y
    int createNewRow(int y, int pageIndex);
    
    //Command to split the text at row y into the next line starting at position x
    bool splitIntoNewRow(int x, int y, int pageIndex);
    //Command to remove the text at row y on page pageIndex starting at position x, returns the removed text
    std::string splitRow(int x, int y, int pageIndex);
    //Command to delete entire row without moving the text to another row, doesn't return anything
    //Used when the row is empty
    void completelyDeleteRowNoReturn(int y, int pageIndex);
private:
    //Command to get the actual index in documentText that the desired row is located at.
    int getRealRowNum(int y, int pageIndex) const;
    //vector of ECRowOfText pointers to hold the rows of text
    //This holds the entire document, even if there's multiple pages  
    std::vector<ECRowOfText*> documentText;
    //The number of rows/elements in documentText
    int numRows;
    //The number of rows in the view, obtained from textViewImp
    int rowsInView;

};
#endif