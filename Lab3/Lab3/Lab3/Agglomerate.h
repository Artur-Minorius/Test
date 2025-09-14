#pragma once
#include "Shape.h"
#include "List.h"
#include "Canvas.h"
namespace Shapes {
	class Agglomerate : public Shape
	{
	private:
		Agglomerate* parentAgglomerate = nullptr;
		List<Shape*> agglomerateObjects;
		Canvas* canvas;
	public:
		Agglomerate(Shape* initial, Canvas* canvas, COLORREF color);
		~Agglomerate();
		//Make copy, delete object from canvas
		void AddShape(Shape* shape);
		//Deletes shapes
		void RemoveShape(Shape* shape);
		void Clear();
		//Removes shapes from Agglomerate object list and put them into Canvas
		void Detach(Shape* shape);
		void DetachAll();

		void SetParent(Agglomerate* parent);
		Agglomerate* GetParent();
		
		virtual void Move(Point) override;
		virtual void Scale(Point) override;

		virtual void Draw(Gdiplus::Graphics& g) override;
		virtual void DrawPreview(Gdiplus::Graphics& g) override;
		virtual void DrawHidden(Gdiplus::Graphics& g) override;
		virtual void DrawOutline(Gdiplus::Graphics& g) override;
		virtual void PlacePoints(Gdiplus::Graphics& g) override;
		virtual bool HitTest(Point localPos) override;

		virtual Point* GetPosition() override;
		virtual void SetPosition(Point newPosition) override;
		void SetRotation(double newRotation) override {};
		virtual void SetScale(Point newScale) override;
		virtual void SetColor(COLORREF newColor) override;

		virtual List<Point> GatherControlPoints() override;
		virtual Point GetGeometricalCenter() override;
		virtual Shape* Clone() override;

		int ControlPointOwner();
		List<Shape*>* GetChildShapes();
	};

}