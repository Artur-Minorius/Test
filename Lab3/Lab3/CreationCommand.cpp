#include "CreationCommand.h"

void CreationCommand::Redo()
{
	if (shape)
		canvas->AddShape(shape);
}
void CreationCommand::Undo()
{
	if (shape)
		canvas->RemoveShape(shape);
}

