#include "Button.h"

Button::Button(Shapes::Point position, Shapes::Point dimensions) : Control(position, dimensions)
{
	this->SetLabel("Button");
}

Button::Button(Shapes::Point position, Shapes::Point dimensions, String label) : Control(position, dimensions)
{
	this->SetLabel(label);
}

void Button::Draw(Gdiplus::Graphics& g)
{
	Gdiplus::SolidBrush fillBrush(Gdiplus::Color(GetAlpha(), GetRValue(backgroundColor), GetGValue(backgroundColor), GetBValue(backgroundColor)));
	g.FillRectangle(&fillBrush, (int)position.x, (int)position.y, (int)dimensions.x, (int)dimensions.y);

	Gdiplus::PointF textPosition = (Gdiplus::PointF)position + Gdiplus::PointF(0, (dimensions.y - font.GetHeight(&g)) / 2); //Veritcal align

	Gdiplus::SolidBrush textBrush(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
	g.DrawString(text->ToWCharArray(), -1, &font, textPosition, &textBrush);
}

bool Button::isMouseOver(MouseEvent& e)
{
	return ((e.X() > position.x && e.X() < position.x + dimensions.x) &&
		(e.Y() > position.y && e.Y() < position.y + dimensions.y));
}

void Button::Click(MouseEvent& e)
{
	if (onClick)
		onClick(this, e);
}
