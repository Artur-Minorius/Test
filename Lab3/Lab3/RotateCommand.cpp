#include "RotateCommand.h"

void RotateCommand::Redo()
{
	if (shape)
		shape->SetRotation(after);
}

void RotateCommand::Undo()
{
	if (shape)
		shape->SetRotation(before);
}

