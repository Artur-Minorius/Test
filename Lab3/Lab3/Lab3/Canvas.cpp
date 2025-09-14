#include "Canvas.h"
#include "Rectangle.h"
#include "Extentions.h"
void Canvas::BringToFront(Shapes::Shape* selected)
{
	int maxZ = 0;
	shapes.ForEach([&](Shapes::Shape* shape) {maxZ = max(maxZ, shape->GetZIndex()); });
	selected->SetZIndex(maxZ + 1);
}

void Canvas::SetDrawHidden(bool doDrawHidden)
{
	this->doDrawHidden = doDrawHidden;
}

Canvas::Canvas(Shapes::Point position, Shapes::Point dimensions) :
	Control(position, dimensions),
	buffer(new Gdiplus::Bitmap(dimensions.x, dimensions.y, PixelFormat32bppARGB))
{
	brush = new Gdiplus::SolidBrush(Gdiplus::Color(GetAlpha(), GetRValue(backgroundColor), GetGValue(backgroundColor), GetBValue(backgroundColor)));
}

Canvas::~Canvas() {
	delete buffer;
}
Gdiplus::Graphics* Canvas::GetGraphics() {
	return new Gdiplus::Graphics(buffer);
}

void Canvas::Draw(Gdiplus::Graphics& g) {
	if (isHidden) return;

	//Canvas graphics
	Gdiplus::Rect dest(position.x, position.y, dimensions.x, dimensions.y);
	Gdiplus::Graphics canvasGraphics(buffer);
	canvasGraphics.Clear(Gdiplus::Color(255, 255, 255, 255)); // clearing
	Extentions::MergeSort(shapes, [&](Shapes::Shape* s1, Shapes::Shape* s2) {return s1->GetZIndex() < s2->GetZIndex(); });
	shapes.ForEach([&](Shapes::Shape* object) {
		if (object->isHidden)
		{
			if (doDrawHidden)
				object->DrawHidden(canvasGraphics);
		}
		else
			object->Draw(canvasGraphics);
		});
	if (previewShape)
		previewShape->DrawPreview(canvasGraphics);
	if (selectedShape)
		selectedShape->DrawOutline(canvasGraphics);
	//Scene graphics
	g.FillRectangle(brush, (int)position.x, (int)position.y, (int)dimensions.x, (int)dimensions.y);
	g.DrawImage(buffer, dest);
}


bool Canvas::isMouseOver(MouseEvent& e) {
	return e.X() >= position.x && e.X() < position.x + dimensions.x &&
		e.Y() >= position.y && e.Y() < position.y + dimensions.y;
}

void Canvas::AddShape(Shapes::Shape* newObject) {
	if (!newObject)
		return;
	shapes.push_back(newObject);
	BringToFront(newObject);
}
Shapes::Shape* Canvas::SelectAt(Shapes::Point localPos) {
	for (int i = (int)shapes.size() - 1; i >= 0; --i) {
		Shapes::Shape* s = shapes[i];
		if (s->HitTest(localPos)) {
			selectedShape = s;
			BringToFront(s);
			s->SetSelectedPointIndex(-1);
			return selectedShape;
		}
	}
	if (selectedShape)
		selectedShape->SetSelectedPointIndex(-1);
	selectedShape = nullptr;
	return nullptr;
}

void Canvas::Unselect()
{
	if (selectedShape)
		selectedShape->SetSelectedPointIndex(-1);
	selectedShape = nullptr;
}

void Canvas::SetShape(ChosenShape t)
{
	currentShape = t;
}

Shapes::Shape* Canvas::GetSelectedShape()
{
	return selectedShape;
}

Canvas::ChosenShape Canvas::GetShape() const
{
	return currentShape;
}

bool Canvas::RemoveShape(Shapes::Shape* shape)
{
	for (int i = 0; i < shapes.size(); i++) {
		if (shapes[i] == shape)
		{
			shapes.remove(i);
			if (shape == selectedShape)
				selectedShape = nullptr;
			return true;
		}
	}

	return false;
}

void Canvas::SetPreviewShape(Shapes::Shape* s)
{
	previewShape = s;
}

void Canvas::FindNearestControlPointIndex(Shapes::Shape* s, Shapes::Point localPos, int threshold) {
	List<Shapes::Point> pts = s->GatherControlPoints();
	int bestIdx = -1;
	int n = (int)pts.size();
	for (int i = 0; i < n; ++i) {
		int distance = Point::Distance(pts[i], localPos);
		if (distance <= threshold) {
			bestIdx = i;
			threshold = distance;
		}
	}
	s->SetSelectedPointIndex(bestIdx);
}


