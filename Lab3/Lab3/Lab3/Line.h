#pragma once
#include "Shape.h"
namespace Shapes {
	class Line : public Shape //Line is a square but with less math
	{
	public:
		Line(Point start, COLORREF color);
		Line(Point start, Point end, COLORREF color);
		void Draw(Gdiplus::Graphics& g) override;
		void DrawHidden(Gdiplus::Graphics& g) override;
		void DrawPreview(Gdiplus::Graphics& g) override;
		bool HitTest(Point localPos) override;
		virtual void DrawOutline(Gdiplus::Graphics& g);
		virtual Shape* Clone() override;
	};
}


