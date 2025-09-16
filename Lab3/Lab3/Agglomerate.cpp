#include "Agglomerate.h"
#include "Extentions.h"
namespace Shapes {
	Agglomerate::Agglomerate(Shape* initial, Canvas* canvas, COLORREF Color) : Shape(initial ? initial->GetPosition() : Point(0, 0), Color), canvas(canvas) {
		if (initial)
			agglomerateObjects.push_back(initial);
	}

	Agglomerate::~Agglomerate()
	{
		for (int i = 0; i < agglomerateObjects.size(); i++) {
			//delete agglomerateObjects[i];
			agglomerateObjects[i] = nullptr;
		}
		agglomerateObjects.clear();
	}

	void Agglomerate::Draw(Gdiplus::Graphics& g) {
		if(!isHidden)
		{
			Extentions::MergeSort(agglomerateObjects, [&](Shapes::Shape* s1, Shapes::Shape* s2) {return s1->GetZIndex() < s2->GetZIndex(); });
			agglomerateObjects.ForEach([&](Shape* child) {child->Draw(g); });
		}
	}

	void Agglomerate::DrawPreview(Gdiplus::Graphics& g)
	{
		Draw(g);
	}

	void Agglomerate::DrawHidden(Gdiplus::Graphics& g)
	{
		agglomerateObjects.ForEach([&](Shape* child) {
			child->DrawHidden(g);
			});
	}

	void Agglomerate::DrawOutline(Gdiplus::Graphics& g)
	{
		agglomerateObjects.ForEach([&](Shape* child) {
			child->DrawOutline(g);
			});
	}

	void Agglomerate::PlacePoints(Gdiplus::Graphics& g)
	{
		agglomerateObjects.ForEach([&](Shape* child) {
			child->PlacePoints(g);
			});
	}

	void Agglomerate::AddShape(Shape* shape)
	{
		Shape* copy = shape->Clone();
		copy->SetColor(color);
		Agglomerate* agg = dynamic_cast<Agglomerate*>(copy);
		if (agg) {
			agg->SetParent(this);
		}

		agglomerateObjects.push_back(copy);

		if (canvas) {
			canvas->RemoveShape(shape);
			//delete shape;
			//shape = nullptr;
		}
	}

	void Agglomerate::RemoveShape(Shape* shape)
	{
		for (int i = 0; i < agglomerateObjects.size(); i++)
			if (agglomerateObjects[i] == shape)
			{
				agglomerateObjects.remove(i);
				return;
			}
	}

	void Agglomerate::Clear()
	{
		for (int i = 0; i < agglomerateObjects.size(); i++) {
			//delete agglomerateObjects[i];
			agglomerateObjects[i] = nullptr;
		}
		agglomerateObjects.clear();

		if (parentAgglomerate) {
			parentAgglomerate->RemoveShape(this);
		}
		else if (canvas) {
			canvas->RemoveShape(this);
		}

		delete this;
	}

	void Agglomerate::Detach(Shape* shape)
	{
		Shape* newShape = shape->Clone();
		if (parentAgglomerate) {
			parentAgglomerate->AddShape(newShape);
		}
		else if (canvas) {
			canvas->AddShape(newShape);
		}
		RemoveShape(shape);
	}

	void Agglomerate::DetachAll()
	{
		if (parentAgglomerate) {
			agglomerateObjects.ForEach([&](Shape* s) {
				Shape* newShape = s->Clone();
				parentAgglomerate->AddShape(newShape);
				});

		}
		else if (canvas) {
			agglomerateObjects.ForEach([&](Shape* s) {
				Shape* newShape = s->Clone();
				canvas->AddShape(newShape); });
		}
		Clear();
	}

	void Agglomerate::SetParent(Agglomerate* parent)
	{
		parentAgglomerate = parent;
	}

	Agglomerate* Agglomerate::GetParent()
	{
		return parentAgglomerate;
	}

	void Agglomerate::Move(Point mousePosition)
	{
		if (agglomerateObjects.size() == 0)
			return;

		if (selectedControlPointIndex < 0)
			return;

		auto pts = GatherControlPoints();
		Point delta = mousePosition - pts[selectedControlPointIndex];
		agglomerateObjects.ForEach([&](Shape* s) {
			s->SetPosition(s->GetGeometricalCenter() + delta);
			});

	}

	int Agglomerate::ControlPointOwner() {
		int t = selectedControlPointIndex + 1;
		for (int i = 0; i < agglomerateObjects.size(); i++) {
			int controlPointsAmountOfShape = agglomerateObjects[i]->GatherControlPoints().size();
			t -= controlPointsAmountOfShape;
			if (t <= 0)
				return i;
		}
	}

	List<Shape*>* Agglomerate::GetChildShapes()
	{
		return &agglomerateObjects;
	}

	void Agglomerate::Scale(Point mousePosition)
	{
		if (selectedControlPointIndex < 0)
			return;
		auto pts = GatherControlPoints();
		auto selectedPoint = pts[selectedControlPointIndex];
		Shape* refShape = agglomerateObjects[ControlPointOwner()];
		Point center = refShape->GetGeometricalCenter();
		double rotation = refShape->GetRotation();

		Point oldVec = selectedPoint.Rotate(center, -rotation) - center;
		Point newVec = mousePosition.Rotate(center, -rotation) - center;

		const double THRESHOLD = 1.0;

		double factorX = newVec.x / oldVec.x;
		double factorY = newVec.y / oldVec.y;
		const double MIN_SCALE_FACTOR = 0.05;
		const double MAX_SCALE_FACTOR = 20.0;
		factorX = max(MIN_SCALE_FACTOR, min(abs(factorX), MAX_SCALE_FACTOR));
		factorY = max(MIN_SCALE_FACTOR, min(abs(factorY), MAX_SCALE_FACTOR));
		agglomerateObjects.ForEach([&](Shape* child) {
			Point scale = child->GetScale();
			scale.x *= factorX;
			scale.y *= factorY;
			child->SetScale(scale);
			});
	}

	bool Agglomerate::HitTest(Point localPos)
	{
		for (int i = 0; i < agglomerateObjects.size(); i++)
			if (agglomerateObjects[i]->HitTest(localPos))
				return true;
		return false;
	}

	Point Agglomerate::GetPosition()
	{
		if (agglomerateObjects.size() <= 0)
			return Point(-1, -1);
		return agglomerateObjects[0]->GetPosition();
	}

	void Agglomerate::SetPosition(Point newPosition)
	{
		Point center = GetGeometricalCenter();
		Point delta = newPosition - center;
		agglomerateObjects.ForEach([&](Shape* s) {
			s->ChangePosition(delta);
			});
	}

	void Agglomerate::SetScale(Point newScale)
	{
		agglomerateObjects.ForEach([&](Shape* child) {
			Point scale = child->GetScale();
			scale.x *= newScale.x;
			scale.y *= newScale.y;
			child->SetScale(scale);
			});
	}

	void Agglomerate::SetColor(COLORREF newColor)
	{
		agglomerateObjects.ForEach([&](Shape* child) {child->SetColor(newColor); });
	}

	Shape* Agglomerate::Clone() {
		Agglomerate* copy = new Agglomerate(nullptr, canvas, color);
		for (int i = 0; i < agglomerateObjects.size(); i++) {
			Shape* childCopy = agglomerateObjects[i]->Clone();
			copy->agglomerateObjects.push_back(childCopy);
			if (Agglomerate* agg = dynamic_cast<Agglomerate*>(childCopy)) {
				agg->SetParent(copy);
			}
		}
		return copy;
	}
	List<Point> Agglomerate::GatherControlPoints()
	{
		List<Point> allPoints;
		agglomerateObjects.ForEach([&](Shape* s) {
			s->GatherControlPoints().ForEach([&](Point p) {
				allPoints.push_back(p);
				});
			});
		return allPoints;
	}
	Point Agglomerate::GetGeometricalCenter() {
		if (agglomerateObjects.size() == 0)
			return Point(0, 0);

		double sx = 0, sy = 0;
		agglomerateObjects.ForEach([&](Shape* s) {
			Point center = s->GetGeometricalCenter();
			sx += center.x;
			sy += center.y;
			});
		return Point(sx / agglomerateObjects.size(), sy / agglomerateObjects.size());
	}


}