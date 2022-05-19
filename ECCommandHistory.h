#ifndef ECCOMANDHISTORY_H
#define ECCOMANDHISTORY_H

class ECCommandHistory;

#include "ECCommands.h"
#include <map>

//Class that holds which commands were previously used, for undo/redo purposes
class ECCommandHistory{
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    bool Undo();
    bool Redo();
    int ExecuteCmd(ECCommands* pCmd);

private:
    std::vector<ECCommands*> commandList;
    int cmdListIndex;
};

#endif