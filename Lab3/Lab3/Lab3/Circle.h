#pragma once
#include "Ellipse.h"
namespace Shapes {
	class Circle : public Shape
	{
	public:
		Circle(Point start, COLORREF Color);
		Circle(Point start, Point end, COLORREF Color);
		void Draw(Gdiplus::Graphics& g) override;
		void DrawHidden(Gdiplus::Graphics& g) override;
		void DrawPreview(Gdiplus::Graphics& g) override;
		void DrawOutline(Gdiplus::Graphics& g) override;
		bool HitTest(Point localPos) override;
		virtual Point GetGeometricalCenter() override;
		virtual Shape* Clone() override;
	};
}


