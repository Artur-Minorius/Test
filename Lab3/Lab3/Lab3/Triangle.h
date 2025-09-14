#pragma once
#include "Shape.h"
namespace Shapes {
	class Triangle :public Shape
	{
	public:
		Triangle(Point start, COLORREF Color);
		Triangle(Point start, Point secondPoint, Point thirdPoint, COLORREF Color);
		void Draw(Gdiplus::Graphics& g) override;
		void DrawHidden(Gdiplus::Graphics& g) override;
		void DrawPreview(Gdiplus::Graphics& g) override;
		bool HitTest(Point localPos) override;
		virtual Shape* Clone() override;
	};
}

