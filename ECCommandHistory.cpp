#include "ECCommandHistory.h"
#include <iostream>
using namespace std;

ECCommandHistory :: ECCommandHistory(){
  cmdListIndex = 0;
}

//Destructor handles deleting any remaining commands in commandList
ECCommandHistory :: ~ECCommandHistory(){
    for(int i = 0; i < (int)commandList.size(); i++){  
        delete commandList[i];        
    }
}

//Undo taken straight from programming assignment 6.1 
bool ECCommandHistory :: Undo(){
    int prevcmdListIndex = cmdListIndex - 1;
    if(prevcmdListIndex >= 0){
        ECCommands* undoCmd = commandList[prevcmdListIndex];
        undoCmd->unexecute();
        cmdListIndex = prevcmdListIndex;
        return true;
    }
    return false; 
}

//Redo taken straight from programming assignment 6.1 
bool ECCommandHistory :: Redo(){
  int nextcmdListIndex = cmdListIndex;
  if(nextcmdListIndex < (int)commandList.size()){
    ECCommands* redoCmd = commandList[nextcmdListIndex];
    redoCmd->execute();
    cmdListIndex++;
    return true;
  }
  return false;
}

//ExecuteCmd taken (almost) straight from programming assignment 6.1 
int ECCommandHistory :: ExecuteCmd(ECCommands* pCmd){
  int returnValue = pCmd->execute();
  int tempListSize = (int)commandList.size();

  //Main addition to ExecuteCmd is that it makes sure that we actually executed the command before doing anything
  //If the command failed (returnValue <= 0), then we don't do anything
  if(cmdListIndex >= -1 && returnValue > 0){
    for(int i = cmdListIndex+1; i <= tempListSize; i++){
      delete commandList.back();
      commandList.pop_back();
    }
  }
  if(returnValue > 0){
    commandList.push_back(pCmd);
    cmdListIndex++;
  }
  return returnValue;  
}