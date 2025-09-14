#include "Scene.h"
#include "Rectangle.h"


Scene::Scene() {
	// Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}
Scene::~Scene() {
	GdiplusShutdown(gdiplusToken);
}
void Scene::SetHWND(HWND* hwnd) {
	this->hwnd = hwnd;
}
void Scene::DrawAll() {
	if (!hwnd)
		return;
	HDC hdc = GetDC(*hwnd);

	//Double buffering - Yay

	//Get screen size to but in bitmap
	RECT rc;
	GetClientRect(*hwnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	Bitmap backBuffer(width, height, PixelFormat32bppARGB); //GDI format
	Gdiplus::Graphics g(&backBuffer);   // Bitmap graphics object
	g.Clear(Color(255, 0, 0, 0)); // clearing

	//Draw all to the buffer
	objects.ForEach([&](Shapes::Shape* object) {object->Draw(g); });
	controlObjects.ForEach([&](Control* object) {object->Draw(g); });

	//Paint obtained image to the foreground
	Graphics screen(hdc);
	screen.DrawImage(&backBuffer, 0, 0);
	ReleaseDC(*hwnd, hdc);
}

void Scene::HandleMouseEvent(MouseEvent& e)
{
	for (int i = 0; i < controlObjects.size(); i++) {
		if (controlObjects[i]->isHidden)
			continue;
		if (controlObjects[i]->isMouseOver(e)) {
			controlObjects[i]->UnFocus();
			e.SetLocal(&controlObjects[i]->position);
			controlObjects[i]->OnMouse(e);
		}
	}
}

Control* Scene::FindControlAt(MouseEvent& e)
{
	for (int i = 0; i < controlObjects.size(); i++) {
		if (controlObjects[i]->isHidden)
			continue;
		if (controlObjects[i]->isMouseOver(e))
			return controlObjects[i];
	}
	return nullptr;
}

void Scene::AddShape(Shapes::Shape* newObject) {
	objects.push_back(newObject);
}

void Scene::AddControl(Control* newControlObject)
{
	newControlObject->SetParentScene(this);
	controlObjects.push_back(newControlObject);
}

void Scene::Invalidate()
{
	DrawAll();
	controlObjects.ForEach([&](Control* object) {object->OnInvalidate(); });
}
