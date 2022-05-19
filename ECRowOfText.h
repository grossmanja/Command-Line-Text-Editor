#ifndef ECROWOFTEXT_H
#define ECROWOFTEXT_H
#include <string>
#include <vector>
//This class holds the string for a row of text
class ECRowOfText{
public:
    ECRowOfText();
    int getRowLen() const;
    char getCharAt(int pos) const;          // get char at current position
    void insertCharAt(int pos, char ch);    // insert a single char at position
    std::string getWholeRow() const;        // returns the whole string
    void removeCharAt(int pos);             // erase a single char at position
    
private:
    //The vector of chars that holds the text
    std::vector<char> listChars;
    //the size of listChars
    int size;
};
#endif