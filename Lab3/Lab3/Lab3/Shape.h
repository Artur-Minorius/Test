#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "Point.h"
#include "List.h"
namespace Shapes {
	class Shape {
	protected:
		bool isFilled = false;
		bool isOutlineAlwaysVisible = false;
		int selectedControlPointIndex = -1;
		int controlPointRadius = 3;
		int pointsNeeded = 1;
		int zIndex = 0;
		double rotation = 0;
		double scaleX = 1;
		double scaleY = 1;
		COLORREF color;
		Gdiplus::SolidBrush brush;
		Gdiplus::Pen pen;
		Point origin;
		List<Point> points;
		Point* previewPoint = nullptr;
		List<Point> GatherPurePoints();
		int GetAlpha();
	public:
		bool isHidden = 0;
		Shape(int x, int y, COLORREF color) : origin(x, y), color(color),
			brush(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color))),
			pen(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color)))
		{};

		Shape(Point start, COLORREF color) : origin(start), color(color),
			brush(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color))),
			pen(Gdiplus::Color(255, GetRValue(color), GetGValue(color), GetBValue(color)))
		{};



		void SetZIndex(int zIndex);
		int GetZIndex();

		virtual Point* GetPosition();
		virtual void SetPosition(Point newPosition);
		virtual void ChangePosition(Point delta);
		virtual double GetRotation();
		virtual void SetRotation(double newRotation);
		virtual Point GetScale();
		virtual void SetScale(Point newScale);
		virtual void SetColor(COLORREF newColor) { color = newColor; };
		virtual COLORREF GetColor() { return color; };
		virtual void SetOutlineAlwaysVisible(bool isOutlineAlwaysVisible) { this->isOutlineAlwaysVisible = isOutlineAlwaysVisible; };
		virtual bool GetOutlineAlwaysVisible() { return isOutlineAlwaysVisible; };

		void Hide();
		void Show();

		virtual void Move(Point);
		virtual void Rotate(Point);
		virtual void Scale(Point);
		
		
		
		virtual void Draw(Gdiplus::Graphics& g) = 0;
		virtual void DrawHidden(Gdiplus::Graphics& g) = 0;
		virtual void DrawPreview(Gdiplus::Graphics& g) {}
		virtual void UpdatePreviewPoint(int x, int y);
		virtual void DrawOutline(Gdiplus::Graphics& g);
		virtual void PlacePoints(Gdiplus::Graphics& g);
		virtual bool AddPoint(int ex, int ey);

		virtual bool HitTest(Point localPos) { return false; }


		virtual void SetSelectedPointIndex(int index);
		virtual int GetSelectedPointIndex();

		List<Point> GatherPoints();
		virtual List<Point> GatherControlPoints();
		virtual Point GetGeometricalCenter();

		virtual Shape* Clone() = 0;

		//virtual String ToJson() { return ""; };


	};
}