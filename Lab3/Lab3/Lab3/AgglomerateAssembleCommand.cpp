#include "AgglomerateAssembleCommand.h"

void AgglomerateAssembleCommand::Redo()
{
	shapes.ForEach([&](Shapes::Shape* s) {
		if (parentAgg)
			parentAgg->RemoveShape(s);
		else if (canvas)
			canvas->RemoveShape(s);

		targetAgg->AddShape(s); });
	
}
void AgglomerateAssembleCommand::Undo() {
	shapes.ForEach([&](Shapes::Shape* s) {
		targetAgg->RemoveShape(s);

		if (parentAgg)
			parentAgg->AddShape(s);
		else if (canvas)
			canvas->AddShape(s);
		});
	canvas->RemoveShape(targetAgg);
}