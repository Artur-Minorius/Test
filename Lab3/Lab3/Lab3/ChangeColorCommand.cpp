#include "ChangeColorCommand.h"

void ChangeColorCommand::Redo()
{
	if (shape)
	shape->SetColor(after);
}
void ChangeColorCommand::Undo()
{
	if (shape)
	shape->SetColor(before);
}
