#include "MoveCommand.h"

void MoveCommand::Redo() {
	if (shape)
		shape->SetPosition(after);
}

void MoveCommand::Undo() {
	if (shape)
		shape->SetPosition(before);
}
