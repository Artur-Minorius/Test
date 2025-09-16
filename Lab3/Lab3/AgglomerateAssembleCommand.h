#pragma once
#include "ICommand.h"
#include "Shape.h"
#include "Agglomerate.h"
class AgglomerateAssembleCommand : public ICommand
{
    Shapes::Agglomerate* targetAgg;
    List<Shapes::Shape*> shapes;
    Canvas* canvas; //top level case
    Shapes::Agglomerate* parentAgg;
public:
    AgglomerateAssembleCommand(Shapes::Agglomerate* agg, const List<Shapes::Shape*>& shapesToAdd, Canvas* canvas)
        : targetAgg(agg), shapes(shapesToAdd), canvas(canvas), parentAgg(nullptr) {}

    // If shapes had a parent agglomerate, set it
    void SetParentAgglomerate(Shapes::Agglomerate* parent) { parentAgg = parent; }
    virtual void Redo() override;
    virtual void Undo() override;
};

