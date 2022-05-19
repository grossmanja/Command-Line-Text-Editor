#include "ECFileWriting.h"
using namespace std;

ECFileWriting :: ECFileWriting(ECTextDocCtrl* textViewIn) : textDocCtrl(textViewIn){
}

void ECFileWriting :: writeToFile(const string fileName){
    ofstream outfile;
    //Make sure we successfully open the file
    if(!outfile){
        return;
    }

    //Clear the text file or create the text file if it didn't already exist.
    outfile.open(fileName, ofstream::out | ofstream::trunc);
    outfile.close();

    outfile.open(fileName);

    //We keep pageIndex (the second parameter in getRowTextatPage) at 0 to make sure that we're reading at the correct index
    //Since getRowTextatPage does getRealRowNum(), we would either need to update pageIndex each time we reach the end of
    //A page, or we can keep pageIndex the same and just increment over the whole vector
    string tempString;
        for(int i = 0; i < textDocCtrl->getTotalNumRows(); i++){
            tempString = textDocCtrl->getRowTextatPage(i, 0);
            outfile << tempString << endl;
        }
    outfile.close();
}