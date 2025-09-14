#pragma once
#include "Control.h"
class Label : public Control
{
public:
	Label(Shapes::Point position, Shapes::Point dimensions, String text);
	void Draw(Gdiplus::Graphics&) override;
	bool isMouseOver(MouseEvent& e) override;
};

