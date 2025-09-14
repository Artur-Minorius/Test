#include "Label.h"

Label::Label(Shapes::Point position, Shapes::Point dimensions, String text) : Control(position, dimensions)
{
	delete this->text;
	this->text = new String(text);
	cursorHoverType = IDC_ARROW;
}

void Label::Draw(Gdiplus::Graphics& g)
{
	Gdiplus::SolidBrush brush(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
	g.DrawString(text->ToWCharArray(), -1, &font, (Gdiplus::PointF)position, &brush);
}

bool Label::isMouseOver(MouseEvent& e)
{
	return ((e.X() > position.x && e.X() < position.x + dimensions.x) &&
		(e.Y() > position.y && e.Y() < position.y + dimensions.y));
}
