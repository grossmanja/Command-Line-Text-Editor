#ifndef ECSTATUSROW_H
#define ECSTATUSROW_H

#include "ECTextViewImp.h"

//Class that handles changing and updating the status row
//Used to show which mode you're in
class ECStatusRow{
public:
    ECStatusRow(ECTextViewImp* textViewImpIn);
    //Command to change what the status row is showing
    //Only takes two arugments since we always want the status row to be highlighted
    //Had a weird bug once where my code would crash anytime I tried to change the status row,
    //Never figured out the reason, but recompiling fixed it so ¯\_(ツ)_/¯
    void changeStatusRow(const std::string statusLeft, const std::string statusRight);
    //Command that removes the status row
    //All it does is call ECTextViewImp->ClearStatusRows();
    void deleteStatusRow();
private:
    ECTextViewImp* textViewImp;
};

#endif