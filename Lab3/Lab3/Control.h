#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "String.h"
#include "MouseEvent.h"
#include "Point.h"

using Shapes::Point;

class Scene;

class Control
{
private:
	using MouseCallback = void(*)(Control*, MouseEvent&);
	using Callback = void(*)(Control*);
protected:
	MouseCallback onClick = nullptr;
	MouseCallback onDown = nullptr;
	MouseCallback onMove = nullptr;
	Callback onInvalidate = nullptr;
	Scene* parentScene = nullptr;
	bool isFocused = false;

	LPWSTR cursorHoverType = IDC_HAND;

public:
	Shapes::Point position;
	Shapes::Point dimensions;
	bool isHidden = 0;

	Gdiplus::FontFamily fontFamily;
	Gdiplus::Font font;

	String* text;
	COLORREF color = RGB(0, 0, 0);
	COLORREF backgroundColor = RGB(255, 255, 255);

	Control(Shapes::Point position, Shapes::Point dimensions) :
		text(new String("")),
		position(position), dimensions(dimensions),
		fontFamily(L"Arial"),
		font(&fontFamily, 16, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel)
	{};
	virtual void Draw(Gdiplus::Graphics&) = 0;
	
	void SetOnMouseDown(MouseCallback);
	void SetOnMouseMove(MouseCallback);
	void SetOnClick(MouseCallback); //Or mouse up
	void SetOnInvalidate(Callback);
	virtual LPWSTR GetHoverCursor() const { return cursorHoverType; }
	virtual void OnMouse(MouseEvent& e); // All events
	virtual void OnInvalidate();
	void Focus();
	void UnFocus();
	void SetColor(COLORREF);
	void SetBackgroundColor(COLORREF);
	virtual void SetLabel(String);
	int GetAlpha();
	virtual bool isMouseOver(MouseEvent& e) = 0;
	void SetParentScene(Scene* scene) { parentScene = scene; }
	Scene* GetParentScene() const { return parentScene; }
};

