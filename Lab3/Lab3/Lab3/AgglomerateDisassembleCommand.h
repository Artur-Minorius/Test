#pragma once
#include "ICommand.h"
#include "Shape.h"
#include "Agglomerate.h"
class AgglomerateDisassembleCommand : public ICommand {
    Shapes::Agglomerate* sourceAgg;      // Agglomerate being cleared
    List<Shapes::Shape*>* shapes;          // Shapes being removed
    Canvas* canvas;
    Shapes::Agglomerate* parentAgg;

public:
    AgglomerateDisassembleCommand(Shapes::Agglomerate* agg, Canvas* canvas)
        : sourceAgg(agg), canvas(canvas), parentAgg(nullptr)
    {
        shapes = agg->GetChildShapes();
        parentAgg = agg->GetParent();
    }

    virtual void Redo() override;
    virtual void Undo() override;
};
