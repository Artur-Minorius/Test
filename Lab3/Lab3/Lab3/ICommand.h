#pragma once

//First interface
class ICommand {
public:
    virtual void Redo() = 0;
    virtual void Undo() = 0;
    virtual ~ICommand() {}
};
