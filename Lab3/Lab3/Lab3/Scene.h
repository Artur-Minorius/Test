#pragma once
#include "List.h"
#include "Shape.h"
#include "Control.h"
#include <gdiplus.h>
#include "MouseEvent.h"
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

class Scene
{
protected:
	ULONG_PTR gdiplusToken; //GDI+ Token, see ~Scene()
	List<Shapes::Shape*> objects;
	List<Control*> controlObjects;
	HWND* hwnd;
	virtual void InitializeComponents() {};
public:
	Scene();
	~Scene();
	void SetHWND(HWND* hwnd);
	void DrawAll();
	void HandleMouseEvent(MouseEvent& e);
	Control* FindControlAt(MouseEvent& e);
	void AddShape(Shapes::Shape*);
	void AddControl(Control*);
	void Invalidate();
};

