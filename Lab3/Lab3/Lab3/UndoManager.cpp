#include "UndoManager.h"

void UndoManager::SetScene(Scene* scene){
	this->scene = scene;
}

void UndoManager::AddCommand(ICommand* cmd)
{
	undoStack.push(cmd);
}

void UndoManager::Undo()
{
	if (undoStack.empty()) return;
	ICommand* cmd = undoStack.top();
	undoStack.pop();
	cmd->Undo();
	redoStack.push(cmd);
	if (scene)
		scene->Invalidate();
}

void UndoManager::Redo()
{
	if (redoStack.empty()) return;
	ICommand* cmd = redoStack.top();
	redoStack.pop();
	cmd->Redo();
	undoStack.push(cmd);
	if (scene)
		scene->Invalidate();
}
