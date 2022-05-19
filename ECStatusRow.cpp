#include "ECStatusRow.h"
#include <iostream>
using namespace std;

ECStatusRow :: ECStatusRow(ECTextViewImp* textViewImpIn) : textViewImp(textViewImpIn){
}

void ECStatusRow :: changeStatusRow(const string statusLeft, const string statusRight){
    textViewImp->ClearStatusRows();
    textViewImp->AddStatusRow(statusLeft, statusRight, true);
}

void ECStatusRow :: deleteStatusRow(){
    textViewImp->ClearStatusRows();
}