#pragma once
#include "Point.h"
#include "List.h"
#include "Windows.h"
#include "Point.h"
class MouseEvent {
	Shapes::Point position;
	Shapes::Point localPosition;
	bool isDragging;
	UINT type; //WM_MOUSEMOVE WM_LBUTTONDOWN WM_LBUTTONUP...
public:
	MouseEvent();
	MouseEvent(int x, int y, bool isDragging, UINT type);
	MouseEvent(Shapes::Point position, bool isDragging, UINT type);
	static List<UINT> AcceptableEvents();
	int X() { return position.x; };
	int Y() { return position.y; };
	int LocalX() { return localPosition.x; };
	int LocalY() { return localPosition.y; };
	bool IsDragging() { return isDragging; };
	Shapes::Point GetGlobalPosition() { return position; }
	Shapes::Point GetLocalPosition() { return localPosition; }
	void SetLocal(Shapes::Point* localPosition);
	UINT Event() { return type; };
};