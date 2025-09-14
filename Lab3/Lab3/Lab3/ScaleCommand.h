#pragma once
#include "ICommand.h"
#include "Shape.h"
#include "Agglomerate.h"
class ScaleCommand : public ICommand
{
	struct ScaleSnapshot {
		Shapes::Shape* shape;
		Shapes::Point scale;
	};

	List<ScaleSnapshot> before;
	List<ScaleSnapshot> after;
public:
	ScaleCommand(Shapes::Shape* s) {
		CaptureScales(s, before);
	}
	void CaptureAfter(Shapes::Shape* s) {
		CaptureScales(s, after);
	}
	void Redo() override;

	void Undo() override;

private:
	void CaptureScales(Shapes::Shape* s, List<ScaleSnapshot>& list);

	// Apply stored scales
	void ApplyScales(List<ScaleSnapshot>& list);
};

