#pragma once
#include "Control.h"
class Label : public Control
{
	bool isMultiLineComputed = false;
	List<String*> multiLine;
	void WrapText(String* text, Gdiplus::Font& font, Gdiplus::Graphics& g, float maxWidth);
public:
	Label(Shapes::Point position, Shapes::Point dimensions, String text);
	void Draw(Gdiplus::Graphics&) override;
	bool isMouseOver(MouseEvent& e) override;
	void SetLabel(String) override;
};

