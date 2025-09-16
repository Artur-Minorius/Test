#include "Control.h"

void Control::SetOnClick(MouseCallback handler) {
	onClick = handler;
}
void Control::SetOnInvalidate(Callback handler)
{
    onInvalidate = handler;
}

void Control::SetOnMouseDown(MouseCallback handler) {
	onDown = handler;
}
void Control::SetOnMouseMove(MouseCallback handler) {
	onMove = handler;
}

void Control::SetColor(COLORREF backgroundColor) {
	this->color = COLORREF(backgroundColor);
}
void Control::SetBackgroundColor(COLORREF backgroundColor) {
	this->backgroundColor = COLORREF(backgroundColor);
}
void Control::SetLabel(String label) {
    delete this->text;
	this->text = new String(label);
}

int Control::GetAlpha()
{
	return isHidden ? 0 : 255;
}

void Control::OnMouse(MouseEvent& e) { 
    switch (e.Event()) {
    case WM_LBUTTONDOWN:
        Focus();
        if (onDown)
            onDown(this, e);
        break;
    case WM_LBUTTONUP:
        if (onClick)
            onClick(this, e);
        break;
    case WM_MOUSEMOVE:
        if (onMove)
            onMove(this, e);
        break;
    }
}

void Control::OnInvalidate()
{
    if (onInvalidate)
        onInvalidate(this);
}

void Control::Focus()
{
    isFocused = true;
}

void Control::UnFocus()
{
    isFocused = false;
}
