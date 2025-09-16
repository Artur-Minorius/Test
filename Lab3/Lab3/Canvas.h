#pragma once
#include "Control.h"
#include "Shape.h"
#include "List.h"
#include "Animation.h"
class Canvas :
    public Control
{
private:
    Gdiplus::Bitmap* buffer;   // More double buffering...
    Gdiplus::SolidBrush* brush;
    List<Shapes::Shape*> shapes; //It's own set of shapes to draw
    List<Shapes::Animation*> animations;
    Shapes::Shape* previewShape = nullptr;
    Shapes::Shape* selectedShape = nullptr;    

    bool doDrawHidden = false;

public:
    enum class ChosenShape { None, Rectangle, Ellipse, Circle, Line, Triangle};
    ChosenShape currentShape = ChosenShape::None;
    int controlPointRadius = 6;
public:
    Canvas(Shapes::Point position, Shapes::Point dimensions);
    ~Canvas();

    Gdiplus::Graphics* GetGraphics();
    void Draw(Gdiplus::Graphics& g) override;
    bool isMouseOver(MouseEvent& e) override;
    void AddShape(Shapes::Shape*);
    void AddAnimation(Shapes::Animation*);
    Shapes::Shape* SelectAt(Shapes::Point localPos);
    void Unselect();
    void SetShape(ChosenShape t);
    Shapes::Shape* GetSelectedShape();
    ChosenShape GetShape() const;
    bool RemoveShape(Shapes::Shape* shape);
    void SetPreviewShape(Shapes::Shape* s);
    void FindNearestControlPointIndex(Shapes::Shape* s, Shapes::Point localPos, int threshold);
    void BringToFront(Shapes::Shape* selected);
    void SetDrawHidden(bool);
};

