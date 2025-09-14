#pragma once
#include "Control.h"
#include "String.h"
class Button : public Control
{
public:
	Button(Shapes::Point position, Shapes::Point dimensions);
	Button(Shapes::Point position, Shapes::Point dimensions, String label);
protected:
	void Draw(Gdiplus::Graphics&) override;
	bool isMouseOver(MouseEvent& e) override;
};

