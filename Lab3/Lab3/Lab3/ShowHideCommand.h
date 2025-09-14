#pragma once
#include "ICommand.h"
#include "Shape.h"
class ShowHideCommand : public ICommand
{
	Shapes::Shape* shape;
	bool before, after;
public:
	ShowHideCommand(Shapes::Shape* s, bool from, bool to)
		: shape(s), before(from), after(to) {}

	void Redo() override;

	void Undo() override;
};

