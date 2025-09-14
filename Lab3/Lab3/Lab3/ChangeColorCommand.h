#pragma once
#include "ICommand.h"
#include "Shape.h"
class ChangeColorCommand : public ICommand
{
	Shapes::Shape* shape;
	COLORREF before, after;
public:
	ChangeColorCommand(Shapes::Shape* s, COLORREF from, COLORREF to)
		: shape(s), before(from), after(to) {}

	void Redo() override;

	void Undo() override;
};

