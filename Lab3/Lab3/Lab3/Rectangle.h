#pragma once
#include "Shape.h"
#include "gdiplus.h"
namespace Shapes {
	class Rectangle : public Shape
	{
	public:
		Rectangle(Point start, COLORREF Color);
		Rectangle(Point start, Point end, COLORREF Color);
		void Draw(Gdiplus::Graphics& g) override;
		void DrawHidden(Gdiplus::Graphics& g) override;
		void DrawPreview(Gdiplus::Graphics& g) override;
		bool AddPoint(int ex, int ey) override;
		bool HitTest(Point localPos) override;
		virtual Shape* Clone() override;
	};
}

