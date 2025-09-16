#include "ShowHideCommand.h"

void ShowHideCommand::Redo()
{
	if (shape)
		shape->isHidden = after;
}
void ShowHideCommand::Undo()
{
	if (shape)
		shape->isHidden = before;
}

