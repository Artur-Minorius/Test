#pragma once
#include "ICommand.h"
#include "Shape.h"
class MoveCommand :public ICommand
{
	Shapes::Shape* shape;
	Shapes::Point before, after;
public:
	MoveCommand(Shapes::Shape* s, Shapes::Point from, Shapes::Point to)
		: shape(s), before(from), after(to) {}

	void Redo() override;

	void Undo() override;
};

