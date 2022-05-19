#ifndef ECFILEREADING_H
#define ECFILEREADING_H
#include <iostream>
#include <fstream>
#include "ECTextDocCtrl.h"

//Class for reading from a file when starting the editor
class ECFileReading{
public:
    ECFileReading(ECTextDocCtrl* textViewIn);
    //Command to get the lines of text from the file
    //Strips /r/n newlines from Windows text editors for easier debugging
    //Based off of code from StackOverflow:
    //https://stackoverflow.com/a/6089413
    std::istream& safeGetline(std::istream &is, std::string &t);
    //Command that takes the lines from safeGetLine and inputs it into ECTextDoc
    bool readFromFile(std::string fileName, ECTextViewImp* textViewImp);
private:
    ECTextDocCtrl* textDocCtrl;
};

#endif