#pragma once
#include "Shape.h"
#include "List.h"
namespace Shapes {
	class Animation: Shape
	{
		bool playing = false;
		bool isEnded = false;

		int currentSegment = 0;

		double duration; //duration in seconds
		double speed; //px/s
		double t = 0.0; //progress in current segment

		List<Shapes::Point> points;
		Shapes::Shape* shape;
		Shapes::Point* previewPoint = nullptr;
		double lastUpdate;
	public:
		Animation(Shapes::Shape* shape, double duration, double speed, COLORREF color);
		void AddPoint(Shapes::Point p);
		void Start();
		void Update();
		bool IsPlaying();
		bool IsEnded();
		virtual void Draw(Gdiplus::Graphics& g);
		virtual void DrawPreview(Gdiplus::Graphics& g);
		virtual void UpdatePreviewPoint(int x, int y);
		virtual bool HitTest(Shapes::Point localPos);
		virtual void PlacePoints(Gdiplus::Graphics& g);
		virtual void SetSelectedPointIndex(int index);
		virtual int GetSelectedPointIndex();
		void EndCreating();
		List<Shapes::Point> GetPoints();
		void DrawHidden(Gdiplus::Graphics& g){}
		Shape* Clone() { return nullptr; }
	};
}


