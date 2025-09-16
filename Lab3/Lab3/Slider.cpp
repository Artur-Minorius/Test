#include "Slider.h"

Slider::Slider(double minValue, double maxValue, double step, Shapes::Point position, Shapes::Point dimensions, int precision) :
	Control(position, dimensions), minValue(minValue), maxValue(maxValue), step(step), precision(precision)
{
	value = round(((maxValue + minValue) / 2) * pow(10, precision)) / pow(10, precision);
}

double Slider::GetValue()
{
	return value;
}

void Slider::SetValue(double v)
{
	value = max(minValue, min(v, maxValue));
}

void Slider::Draw(Gdiplus::Graphics& g)
{
	if (isHidden) return;

	// background
	Gdiplus::SolidBrush bgBrush(Gdiplus::Color(GetAlpha(),
		GetRValue(backgroundColor), GetGValue(backgroundColor), GetBValue(backgroundColor)));
	g.FillRectangle(&bgBrush, (int)position.x, (int)position.y, (int)dimensions.x, (int)dimensions.y);

	// track
	int trackY = position.y + dimensions.y / 2 - barHeight / 2;
	int trackX = position.x + trackPadding;
	int trackW = dimensions.x - 2 * trackPadding;
	Gdiplus::SolidBrush trackBrush(Gdiplus::Color(200, 200, 200));
	g.FillRectangle(&trackBrush, trackX, trackY, trackW, barHeight);

	// Value label

	Gdiplus::SolidBrush textBrush(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
	Shapes::Point textPosition = position + Point(dimensions.x, 0)+ Shapes::Point(0, (dimensions.y - font.GetHeight(&g)) / 2) + Shapes::Point(5, 0);
	g.DrawString(String(value).ToWCharArray(), -1, &font, (Gdiplus::PointF)textPosition, &textBrush);

	// knob position based on value
	double t = (value - minValue) / (maxValue - minValue);
	int knobX = trackX + (int)(t * trackW);
	int knobY = position.y + dimensions.y / 2;

	Gdiplus::SolidBrush knobBrush(Gdiplus::Color(50, 50, 50));
	g.FillEllipse(&knobBrush, knobX - knobRadius, knobY - knobRadius, 2 * knobRadius, 2 * knobRadius);

}
void Slider::UpdateValue(MouseEvent& e) {
	double trackX = position.x + trackPadding;
	double trackW = dimensions.x - 2 * trackPadding;

	double relX = e.X() - trackX;
	relX = max(0, min(relX, trackW));
	double t = relX / trackW;
	//value = minValue + (int)lround(t * (maxValue - minValue));
	value = minValue +  round(t * (maxValue - minValue) * pow(10, precision)) / pow(10, precision);
}
void Slider::OnMouse(MouseEvent& e)
{
	switch (e.Event()) {
	case WM_LBUTTONDOWN:
		Focus();
		UpdateValue(e);
		if (onDown)
		{
			onDown(this, e);
		}
		break;
	case WM_LBUTTONUP:
		if (onClick)
		{
			onClick(this, e);
		}
		break;
	case WM_MOUSEMOVE:
		if (e.IsDragging())
			UpdateValue(e);
		if (onMove)
		{
			onMove(this, e);
		}
		break;
	}
}

bool Slider::isMouseOver(MouseEvent& e)
{
	return ((e.X() > position.x && e.X() < position.x + dimensions.x) &&
		(e.Y() > position.y && e.Y() < position.y + dimensions.y));
}
