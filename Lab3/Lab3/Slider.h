#pragma once
#include "Control.h"
class Slider :public Control
{
	double minValue, maxValue;
	double step;
	int precision;
	double value;

	int barHeight = 6;
	int knobRadius = 8;
	int trackPadding = 8;

	void UpdateValue(MouseEvent& e);
public:
	Slider(double min, double max, double step, Shapes::Point position, Shapes::Point dimensions, int precision = 0);
	double GetValue();
	void SetValue(double v);
	void Draw(Gdiplus::Graphics& g) override;
	virtual void OnMouse(MouseEvent& e) override;
	bool isMouseOver(MouseEvent& e) override;
};

