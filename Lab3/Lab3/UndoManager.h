#pragma once
#include "Stack.h"
#include "ICommand.h"
#include "Scene.h"
class UndoManager
{
	Stack<ICommand*> undoStack;
	Stack<ICommand*> redoStack;
	Scene* scene = nullptr;

public:
	void SetScene(Scene* scene);
	void AddCommand(ICommand* cmd);
	void Undo();
	void Redo();
};

