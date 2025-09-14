#pragma once
#include "ICommand.h"
#include "Shape.h"
class RotateCommand : public ICommand
{
	Shapes::Shape* shape;
	double before, after;
public:
	RotateCommand(Shapes::Shape* s, double from, double to)
		: shape(s), before(from), after(to) {}

	void Redo() override;

	void Undo() override;
};

