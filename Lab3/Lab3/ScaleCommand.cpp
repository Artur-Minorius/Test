#include "ScaleCommand.h"

void ScaleCommand::Redo()
{
	ApplyScales(after);
}
void ScaleCommand::Undo()
{
	ApplyScales(before);
}

void ScaleCommand::CaptureScales(Shapes::Shape* s, List<ScaleSnapshot>& list)
{
	if (!s) return;
	if (Shapes::Agglomerate* agg = dynamic_cast<Shapes::Agglomerate*>(s)) {
		auto children = agg->GetChildShapes();
		children->ForEach([&](Shapes::Shape* child) {
			CaptureScales(child, list);
			});

	}
	list.push_back({ s, s->GetScale() });
}

void ScaleCommand::ApplyScales(List<ScaleSnapshot>& list)
{
	list.ForEach([&](ScaleSnapshot snap) {
		if (snap.shape)
			snap.shape->SetScale(snap.scale);
		});
}
