#pragma once
#include "Shape.h"
namespace Shapes {
	class Ellipse : public Shape
	{
	public:
		Ellipse(Point start, COLORREF Color);
		Ellipse(Point start, Point end, COLORREF Color);
		void Draw(Gdiplus::Graphics& g) override;
		void DrawHidden(Gdiplus::Graphics& g) override;
		void DrawPreview(Gdiplus::Graphics& g) override;
		bool HitTest(Point localPos) override;
		virtual void DrawOutline(Gdiplus::Graphics& g);
		virtual Shape* Clone() override;
	};
}

