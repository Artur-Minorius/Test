#pragma once
#include "ICommand.h"
#include "Shape.h"
#include "Canvas.h"
class CreationCommand : public ICommand
{
	Shapes::Shape* shape;
	Canvas* canvas;
public:
	CreationCommand(Shapes::Shape* s, Canvas* canvas)
		: shape(s), canvas(canvas){}

	void Redo() override;

	void Undo() override;

};

