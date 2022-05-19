#ifndef ECFILEWRITING_H
#define ECFILEWRITING_H
#include "ECTextDocCtrl.h"
#include <iostream>
#include <fstream>

//Class that writes the text from ECTextDoc and puts it into a text file
class ECFileWriting{
public:
    ECFileWriting(ECTextDocCtrl* textViewIn);
    //Command to write the text from textDocCtrl and puts it into the specified text file line by line.
    void writeToFile(const std::string fileName);
private:
    ECTextDocCtrl* textDocCtrl;
};


#endif