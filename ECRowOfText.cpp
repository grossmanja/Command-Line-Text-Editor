#include "ECRowOfText.h"
#include <iostream>
using namespace std;

//Took most of this code from Programming Assignment 6.1
//Would've it been easier to just use a string for each row instead of a whole object?  Maybe, not sure.

ECRowOfText :: ECRowOfText(){size = 0;}

int ECRowOfText :: getRowLen() const{
    return size;
}

char ECRowOfText :: getCharAt(int pos) const{
    return listChars[pos];
}

void ECRowOfText :: insertCharAt(int pos, char input){
    listChars.insert(listChars.begin() + pos, input);
    size++;
}

string ECRowOfText :: getWholeRow() const{
    if(size == 0){
        return "";
    } 
    
    string row = "";
    for(int i = 0; i < size; i++){
        row += listChars[i];
    }
    return row;
}

void ECRowOfText :: removeCharAt(int pos){
    if(pos < size && pos >= 0){
        cerr << "Removing character " << listChars[pos] << endl;
        listChars.erase(listChars.begin() + pos);
        size--;
    } 
}