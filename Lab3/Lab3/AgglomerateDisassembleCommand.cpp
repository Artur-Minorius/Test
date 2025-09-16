#include "AgglomerateDisassembleCommand.h"

void AgglomerateDisassembleCommand::Redo()
{
	//Simply wont work, because of the agglomerate nonsense
	shapes->ForEach([&](Shapes::Shape* s) {
		sourceAgg->RemoveShape(s);
		if (parentAgg)
			parentAgg->AddShape(s);
		else if (canvas)
			canvas->AddShape(s);
		});
}

void AgglomerateDisassembleCommand::Undo() {
	shapes->ForEach([&](Shapes::Shape* s) {
		if (parentAgg)
			parentAgg->RemoveShape(s);
		else if (canvas)
			canvas->RemoveShape(s);

		sourceAgg->AddShape(s);
		});
}