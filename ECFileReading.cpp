#include "ECFileReading.h"
using namespace std;

ECFileReading :: ECFileReading(ECTextDocCtrl* textViewIn) : textDocCtrl(textViewIn){}

//This function allows me to edit files in Windows and still be able to read them in the editor
//It makes sure that the ends of lines are just the \n newline instead of Windows \r\n
//I sort of understand how it works, but it was very useful
istream& ECFileReading :: safeGetline(istream &is, string &t)
{
    t.clear();

    istream::sentry se(is, true);
    streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

bool ECFileReading :: readFromFile(string fileName, ECTextViewImp* textViewImp){
    //cerr << "Start of readFromFile, reading from: " << fileName << endl;
    std::ifstream ifs(fileName.c_str());
    //Make sure we actually open the file
    if(!ifs){
        return false;
    }
    //Turn off screen refresh, since this function essentially acts as if the person is typing very quickly.
    //If screen refresh was on, you would see the text being entered/pages changing 
    textDocCtrl->switchMyRefresh(false);
    string temp = "";
    int count = 0;
    //While there's still lines of text in the document
    while(!safeGetline(ifs, temp).eof()){
        //Insert the character into the line
        for(int i = 0; i < (int)temp.size(); i++){
            //cerr << "readFromFile temp[i] is " << temp[i] << endl;
            textDocCtrl->insertCharAt(i, count, temp[i]);
        }
        //Once we've entered the text into the line, create a new row
        textDocCtrl->createNewRow(0, count + 1);
        //Check if we need to switch to a new page
        if(count < textViewImp->GetRowNumInView() - 1){
            count++;
        }else{
            textDocCtrl->changeNextPage();
            count = 0;
        }
    }
    //Due to some reason, an extra row is added to the text file if we read more than one line from the file,
    //I know that it's fine, but it bothers me, so we just go and delete the row.
    if(textDocCtrl->getTotalNumRows() > 1){
        textDocCtrl->completelyDeleteRowNoReturn(textDocCtrl->getNumRowsInPage(textDocCtrl->getNumPages() - 1) - 1);
    }
    //Turn on screen refreshing and set the view back to the beginning, and the user is ready to go
    textDocCtrl->switchMyRefresh(true);
    textDocCtrl->changeCurrentPage(0);
    textViewImp->SetCursorX(0);
    textViewImp->SetCursorY(0);
    return true;
}