#ifndef ECCOMMANDS_H
#define ECCOMMANDS_H

//Had to define ECCommands before the includes due to an error
class ECCommands;

#include "ECTextViewImp.h"
#include "ECReplace.h"
#include "ECSearchDoc.h"
#include "ECTextDocCtrl.h"
#include <string>

//The commands design pattern
//Each class holds how to execute and unexecute the command

//The abstract ECCommands class that all commands inherit from
class ECCommands{
public:
    virtual ~ECCommands();
    virtual int execute() = 0;
    virtual int unexecute() = 0;

};

//Class for inserting a single character into the document
class ECInsertTextCommand : public ECCommands{
public:
    ECInsertTextCommand(ECTextDocCtrl* doc, int xIn, int yIn, const char inputIn);
    virtual int execute();
    virtual int unexecute();

private:
    ECTextDocCtrl* textDocCtrl;     //The document controller object
    int x;                          //The x location that the character was inserted
    int y;                          //The y location that the character was inserted
    char input;                     //The actual character that was inserted
    int pageNum;                    //The page where the character was located
};

//Class for remvoing a single character from the document
class ECRemoveTextCommand : public ECCommands{
public:
    ECRemoveTextCommand(ECTextDocCtrl* doc, int xIn, int yIn);
    virtual int execute();
    virtual int unexecute();

private:
    ECTextDocCtrl* textDocCtrl;     //See ECInsertTextCommand for variable explanations
    int x;
    int y;
    char input;
    int pageNum;
};

//Class that inserts a new row into the document
class ECInsertRowCommand : public ECCommands{
public:
    ECInsertRowCommand(ECTextDocCtrl* doc, int xIn, int yIn);
    virtual int execute();
    virtual int unexecute();

private:
    ECTextDocCtrl* textDocCtrl;
    int x;
    int y;
    int pageNum;
};

//Class the removes a row from the document
class ECRemoveRowCommand : public ECCommands{
public:
    ECRemoveRowCommand(ECTextDocCtrl* doc, int xIn, int yIn);
    virtual int execute();
    virtual int unexecute();

private:
    ECTextDocCtrl* textDocCtrl;
    int x;
    int y;
    int pageNum;
    std::string textToSplit;        //The text that was move from the row that was removed.  Used when unexecuting to move the correct text back onto the row
};

//Class that replaces certain text with new text
class ECReplaceTextCommand : public ECCommands{
public:
    ECReplaceTextCommand(ECTextDocCtrl* docIn, ECReplace* replaceDocIn, std::vector<std::vector<int>> locationsIn, std::string textToRemoveIn, std::string textToReplaceIn);
    virtual int execute();
    virtual int unexecute();

private:
    ECTextDocCtrl* textDocCtrl;
    ECReplace* replaceDoc;
    // textViewImp;
    std::vector<std::vector<int>> locations;    //A 2D vector of integers to hold the locations where the document was changed.  Essentially a vector of (x,y) coordinates
    int pageNum;                                //The page at where the replace occurred
    std::string textToRemove;                   //The text that was removed, aka what was originally in the document
    std::string textToReplace;                  //The text that was put in place, aka what is in the document after hitting enter
};

#endif