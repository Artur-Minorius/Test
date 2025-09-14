#include "MouseEvent.h"

MouseEvent::MouseEvent(int x, int y, bool isDragging, UINT type){
	position.x = x;
	position.y = y;
	this->isDragging = isDragging;
	this->type = type;
}

MouseEvent::MouseEvent(Shapes::Point position, bool isDragging, UINT type) {
	this->position.x = position.x;
	this->position.y = position.y;
	this->isDragging = isDragging;
	this->type = type;
}

List<UINT> MouseEvent::AcceptableEvents() {
	List<UINT> supportedEvents;
	supportedEvents.push_back(WM_MOUSEMOVE);
	supportedEvents.push_back(WM_LBUTTONDOWN);
	supportedEvents.push_back(WM_LBUTTONUP);
	return supportedEvents;
}

void MouseEvent::SetLocal(Shapes::Point* localPosition) {
	this->localPosition.x = this->position.x - localPosition->x;
	this->localPosition.y = this->position.y - localPosition->y;
};