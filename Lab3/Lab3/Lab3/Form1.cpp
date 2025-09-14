#include "Form1.h"
#include "TerminalWindow.h"

#include "Button.h"
#include "Slider.h"
#include "Canvas.h"
#include "Label.h"

#include "Point.h"
#include "MouseEvent.h"

#include "Rectangle.h"
#include "Ellipse.h"
#include "Circle.h"
#include "Line.h"
#include "Triangle.h"
#include "Agglomerate.h"


#include "UndoManager.h"
#include "MoveCommand.h"
#include "ScaleCommand.h"
#include "RotateCommand.h"
#include "CreationCommand.h"
#include "ChangeColorCommand.h"
#include "ShowHideCommand.h"
#include "AgglomerateAssembleCommand.h"
#include "AgglomerateDisassembleCommand.h"

void ShapesButtonClick(Control* sender, MouseEvent& e);

void ToolsButtonClick(Control* sender, MouseEvent& e);

void OnCanvasClick(Control* sender, MouseEvent& e);
void OnCanvasMouseDown(Control* sender, MouseEvent& e);
void OnCanvasMouseMove(Control* sender, MouseEvent& e);

void RedoButtonClick(Control* sender, MouseEvent& e);
void UndoButtonClick(Control* sender, MouseEvent& e);

void RedSliderMouseMove(Control* sender, MouseEvent& e);
void GreenSliderMouseMove(Control* sender, MouseEvent& e);
void BlueSliderMouseMove(Control* sender, MouseEvent& e);

void SetColorApplyButtonClick(Control* sender, MouseEvent& e);
void ShowHideToggleButtonClick(Control* sender, MouseEvent& e);
void ShowHideToggleInvalidate(Control* sender);

void CreateAgglomerateButtonClick(Control* sender, MouseEvent& e);
void CreateAgglomerateButtonInvalidate(Control* sender);

void ShowHideToggleInvalidate(Control* sender);

void SetPreviewShape(Shapes::Point);

void Reset(bool doResetSelectedShape = false);

template <typename F, typename CmdF>
void BaseSelectedShapeLogic(MouseEvent&, F, CmdF);

List<String*> labelsShapes;
int currShape = 1; //Access to current shape
//More C# WinFormApp code
Canvas* canvas; // Global canvas object

Shapes::Shape* previewShape;
Shapes::Shape* selectedShape;
List<Shapes::Shape*> newAgglomerateShapes;

enum Tools { Create, Move, Rotate, Scale, SetColor, HideShow, CreateAgglomerate };

Tools currTool = Create;

//Button* toolsButton;
Button* createShapeButton;
Button* setColorApplyButton;
Button* hideShowToggleButton;
Button* createAgglomerateButton;

Label* currToolLabel;

UndoManager undoManager;
//State tracing
Shapes::Point moveStart;
double rotationStart;
Shapes::Point scaleStart;
ScaleCommand* scaleCmd;
Shapes::Shape* colorPreview;

bool isAgglomeratesWorkInWeirdWay = true;

#pragma region Setups for Tools Buttons
void SetupCreate(Control*);
void SetupEmpty(Control*);
void SetupSetColor(Control*);
void SetupHideShow(Control*);
void SetupAgglomerate(Control*);
#pragma endregion


struct ToolButton {
	using SpecialFunction = void(*)(Control*);
	Button* button;
	Tools tool;
	SpecialFunction specialFunction;
	bool doResetSelectedShape = false;
};

List<ToolButton> toolButtons;

void Form1::InitializeComponents() {

	undoManager.SetScene(this);
	Shapes::Point toolsStartPos(610, 170);
	Shapes::Point baseSize(150, 20);
	int indx = 0;
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Create"), Create, SetupCreate, true });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Move"), Move, SetupEmpty });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Rotate"), Rotate, SetupEmpty });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Scale"), Scale, SetupEmpty });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Set color"), SetColor, SetupSetColor });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Create agglomerate"), CreateAgglomerate, SetupAgglomerate });
	toolButtons.ForEach([&](ToolButton tB) {
		tB.button->SetOnClick(ToolsButtonClick);
		AddControl(tB.button);
		});
	currToolLabel = new Label(toolsStartPos + Shapes::Point(0, 30 * indx), baseSize, "Create");

#pragma region Buttom Right Corner Buttons

	Shapes::Point pos(610, 500);

	//Allows creation of different shapes
	labelsShapes.push_back(new String("Rectangle"));
	labelsShapes.push_back(new String("Ellipse"));
	labelsShapes.push_back(new String("Circle"));
	labelsShapes.push_back(new String("Line"));
	labelsShapes.push_back(new String("Triangle"));
	createShapeButton = new Button(pos, Shapes::Point(150, 20), "Rectangle");
	createShapeButton->SetOnClick(ShapesButtonClick);
	AddControl(createShapeButton);

	//Set color apply button
	setColorApplyButton = new Button(pos, Shapes::Point(150, 20), "Apply");
	setColorApplyButton->isHidden = true;
	setColorApplyButton->SetOnClick(SetColorApplyButtonClick);
	AddControl(setColorApplyButton);

	//Agglomerate start/end creation
	createAgglomerateButton = new Button(pos, Shapes::Point(150, 20), "Stop creating");
	createAgglomerateButton->isHidden = true;
	createAgglomerateButton->SetOnClick(CreateAgglomerateButtonClick);
	createAgglomerateButton->SetOnInvalidate(CreateAgglomerateButtonInvalidate);
	AddControl(createAgglomerateButton);

	//  Hide/show button
	hideShowToggleButton = new Button(pos, Shapes::Point(150, 20), "Select shape");
	hideShowToggleButton->isHidden = true;
	hideShowToggleButton->SetOnClick(ShowHideToggleButtonClick);
	hideShowToggleButton->SetOnInvalidate(ShowHideToggleInvalidate);
	AddControl(hideShowToggleButton);
#pragma endregion

#pragma region Canvas
	//Painting area
	canvas = new Canvas(Shapes::Point(50, 50), Shapes::Point(550, 450));
	canvas->backgroundColor = RGB(255, 255, 255);
	canvas->SetShape(Canvas::ChosenShape::Rectangle); // Default
	canvas->SetOnClick(OnCanvasClick);
	canvas->SetOnMouseDown(OnCanvasMouseDown);
	canvas->SetOnMouseMove(OnCanvasMouseMove);
	AddControl(canvas);
#pragma endregion

#pragma region Undo/Redo Buttons
	//Undo/redo buttons
	Button* undoButton = new Button(Shapes::Point(50, 20), Shapes::Point(70, 20), "Undo");
	undoButton->SetOnClick(UndoButtonClick);
	AddControl(undoButton);
	Button* redoButton = new Button(Shapes::Point(130, 20), Shapes::Point(70, 20), "Redo");
	redoButton->SetOnClick(RedoButtonClick);
	AddControl(redoButton);
#pragma endregion

#pragma region ColorPreview
	Shapes::Point colorPreviewStart(610, 50);
	//RGB slider
	Slider* redValSlider = new Slider(0, 255, 1, colorPreviewStart + Shapes::Point(0, 30), Shapes::Point(120, 20));
	redValSlider->SetColor(RGB(255, 255, 255));
	redValSlider->SetOnMouseMove(RedSliderMouseMove);
	AddControl(redValSlider);

	Slider* greenValSlider = new Slider(0, 255, 1, colorPreviewStart + Shapes::Point(0, 60), Shapes::Point(120, 20));
	greenValSlider->SetColor(RGB(255, 255, 255));
	greenValSlider->SetOnMouseMove(GreenSliderMouseMove);
	AddControl(greenValSlider);

	Slider* blueValSlider = new Slider(0, 255, 1, colorPreviewStart + Shapes::Point(0, 90), Shapes::Point(120, 20));
	blueValSlider->SetColor(RGB(255, 255, 255));
	blueValSlider->SetOnMouseMove(BlueSliderMouseMove);
	AddControl(blueValSlider);

	colorPreview = new Shapes::Rectangle(colorPreviewStart, colorPreviewStart + Shapes::Point(120, 20), //Size
		RGB(redValSlider->GetValue(), greenValSlider->GetValue(), blueValSlider->GetValue()));
	AddShape(colorPreview);
#pragma endregion
}


void ShapesButtonClick(Control* sender, MouseEvent& e) {
	if (currShape >= labelsShapes.size()) {
		currShape = 0;
	}
	Button* button = dynamic_cast<Button*>(sender);
	if (!button)
		return;
	//Switch here expects String not const char*
	if (*labelsShapes[currShape] == "Rectangle")
		canvas->SetShape(Canvas::ChosenShape::Rectangle);
	else if (*labelsShapes[currShape] == "Ellipse")
		canvas->SetShape(Canvas::ChosenShape::Ellipse);
	else if (*labelsShapes[currShape] == "Circle")
		canvas->SetShape(Canvas::ChosenShape::Circle);
	else if (*labelsShapes[currShape] == "Line")
		canvas->SetShape(Canvas::ChosenShape::Line);
	else if (*labelsShapes[currShape] == "Triangle")
		canvas->SetShape(Canvas::ChosenShape::Triangle);
	button->SetLabel(*labelsShapes[currShape++]);
}

void OnCanvasClick(Control* sender, MouseEvent& e) {
	switch (currTool) {
	case Create:
		if (!previewShape)
			SetPreviewShape(e.GetLocalPosition());
		else
			if (previewShape) {
				if (previewShape->AddPoint(e.LocalX(), e.LocalY()))
				{
					if (!isAgglomeratesWorkInWeirdWay)
						undoManager.AddCommand(new CreationCommand(previewShape, canvas));
					canvas->AddShape(previewShape);
					canvas->SetPreviewShape(nullptr);
					previewShape = nullptr;
				}
			}
		break;
	case Move:
		BaseSelectedShapeLogic(e,
			[&]() {selectedShape->Move(e.GetLocalPosition()); },
			[&]() {return new MoveCommand(selectedShape, moveStart, selectedShape->GetGeometricalCenter()); });
		break;
	case Rotate:
		BaseSelectedShapeLogic(e,
			[&]() {selectedShape->Rotate(e.GetLocalPosition()); },
			[&]() {return new RotateCommand(selectedShape, rotationStart, selectedShape->GetRotation()); });
		break;
	case Scale:
		BaseSelectedShapeLogic(e,
			[&]() {selectedShape->Scale(e.GetLocalPosition()); },
			[&]() {
				scaleCmd->CaptureAfter(selectedShape);
				return scaleCmd;
			}); //It was easy until we're here

		break;
	case SetColor:
		selectedShape = canvas->SelectAt(e.GetLocalPosition());
		break;
	case HideShow:
		selectedShape = canvas->SelectAt(e.GetLocalPosition());
		if (!selectedShape)
			hideShowToggleButton->SetLabel("Select shape");
		else
			hideShowToggleButton->SetLabel(selectedShape->isHidden ? "Show" : "Hide");
		break;
	case CreateAgglomerate:
		selectedShape = canvas->SelectAt(e.GetLocalPosition());
		if (selectedShape)
		{
			//toolsButton->isHidden = true;
			if (!newAgglomerateShapes.contains(selectedShape))
			{
				selectedShape->SetOutlineAlwaysVisible(true);
				newAgglomerateShapes.push_back(selectedShape);
			}
			else {
				selectedShape->SetOutlineAlwaysVisible(false);

				for (int i = 0; i < newAgglomerateShapes.size(); i++)
					if (newAgglomerateShapes[i] == selectedShape)
					{
						newAgglomerateShapes.remove(i);
						break;
					}
				selectedShape = nullptr;
				canvas->Unselect();
				/*if (newAgglomerateShapes.size() == 0)
					toolsButton->isHidden = false;*/
			}
		}
		break;
	}

}

void SetPreviewShape(Shapes::Point pos) {
	switch (canvas->currentShape) {
	case Canvas::ChosenShape::Rectangle: {
		previewShape = new Shapes::Rectangle(pos, colorPreview->GetColor());
		canvas->SetPreviewShape(previewShape);
		break;
	}
	case Canvas::ChosenShape::Ellipse: {
		previewShape = new Shapes::Ellipse(pos, colorPreview->GetColor());
		canvas->SetPreviewShape(previewShape);
		break;
	}
	case Canvas::ChosenShape::Circle: {
		previewShape = new Shapes::Circle(pos, colorPreview->GetColor());
		canvas->SetPreviewShape(previewShape);
		break;
	}
	case Canvas::ChosenShape::Line: {
		previewShape = new Shapes::Line(pos, colorPreview->GetColor());
		canvas->SetPreviewShape(previewShape);
		break;
	}
	case Canvas::ChosenShape::Triangle: {
		previewShape = new Shapes::Triangle(pos, colorPreview->GetColor());
		canvas->SetPreviewShape(previewShape);
		break;
	}
	}
}

void Reset(bool doResetSelectedShape)
{
	canvas->SetDrawHidden(false);
	createShapeButton->isHidden = true;
	setColorApplyButton->isHidden = true;
	hideShowToggleButton->isHidden = true;
	createAgglomerateButton->isHidden = true;
	newAgglomerateShapes.clear();
	canvas->SetPreviewShape(nullptr);
	previewShape = nullptr;
	if (doResetSelectedShape)
	{
		selectedShape = nullptr;
		canvas->Unselect();
	}
}

void OnCanvasMouseDown(Control* sender, MouseEvent& e)
{

}

void OnCanvasMouseMove(Control* sender, MouseEvent& e)
{

	switch (currTool) {
	case Create:
		if (previewShape) {
			previewShape->UpdatePreviewPoint(e.LocalX(), e.LocalY());
		}
		break;
	case Move:
		createShapeButton->isHidden = true;
		if (selectedShape)
		{
			selectedShape->Move(e.GetLocalPosition());
		}
		break;
	case Rotate:
		createShapeButton->isHidden = true;
		if (selectedShape)
		{
			selectedShape->Rotate(e.GetLocalPosition());
		}
		break;
	case Scale:
		createShapeButton->isHidden = true;
		if (selectedShape)
		{
			selectedShape->Scale(e.GetLocalPosition());
		}
		break;
	}
}


void ToolsButtonClick(Control* sender, MouseEvent& e) {
	Button* b = dynamic_cast<Button*>(sender);
	if (!b)
		return;
	int tool = -1;
	for (int i = 0; i < toolButtons.size(); i++) {
		if (toolButtons[i].button == b) {
			tool = i;
			toolButtons[i].button->backgroundColor = RGB(128, 128, 128);
		}
		else {
			toolButtons[i].button->backgroundColor = RGB(255, 255, 255);
		}
	}
	if (tool < 0)
		return;
	ToolButton& t = toolButtons[tool];
	Reset(t.doResetSelectedShape);
	if (t.specialFunction)
		t.specialFunction(sender);
	currTool = t.tool;
	currToolLabel->SetLabel(*b->text);
}

template <typename F, typename CmdF>
void BaseSelectedShapeLogic(MouseEvent& e, F action, CmdF commandCreation) {
	if (!selectedShape)
		selectedShape = canvas->SelectAt(e.GetLocalPosition());
	else
		if (selectedShape->GetSelectedPointIndex() == -1)
		{
			canvas->FindNearestControlPointIndex(selectedShape, e.GetLocalPosition(), canvas->controlPointRadius);
			if (selectedShape->GetSelectedPointIndex() == -1)
				selectedShape = canvas->SelectAt(e.GetLocalPosition());
			else
			{
				moveStart = selectedShape->GetGeometricalCenter();
				rotationStart = selectedShape->GetRotation();
				scaleStart = selectedShape->GetScale();

				delete scaleCmd;
				scaleCmd = new ScaleCommand(selectedShape);

				action();
			}
		}
		else
		{
			action();
			undoManager.AddCommand(commandCreation());

			canvas->Unselect();
			selectedShape = nullptr;
		}
}

void RedoButtonClick(Control* sender, MouseEvent& e) {
	undoManager.Redo();
}
void UndoButtonClick(Control* sender, MouseEvent& e) {
	undoManager.Undo();
}
void RedSliderMouseMove(Control* sender, MouseEvent& e) {
	Slider* slider = dynamic_cast<Slider*>(sender);
	if (!slider)
		return;
	COLORREF currColor = colorPreview->GetColor();
	colorPreview->SetColor(RGB(slider->GetValue(), GetGValue(currColor), GetBValue(currColor)));
}
void GreenSliderMouseMove(Control* sender, MouseEvent& e) {
	Slider* slider = dynamic_cast<Slider*>(sender);
	if (!slider)
		return;
	COLORREF currColor = colorPreview->GetColor();
	colorPreview->SetColor(RGB(GetRValue(currColor), slider->GetValue(), GetBValue(currColor)));
}
void BlueSliderMouseMove(Control* sender, MouseEvent& e) {
	Slider* slider = dynamic_cast<Slider*>(sender);
	if (!slider)
		return;
	COLORREF currColor = colorPreview->GetColor();
	colorPreview->SetColor(RGB(GetRValue(currColor), GetGValue(currColor), slider->GetValue()));
}

void SetColorApplyButtonClick(Control* sender, MouseEvent& e) {
	if (selectedShape)
	{
		undoManager.AddCommand(new ChangeColorCommand(selectedShape, selectedShape->GetColor(), colorPreview->GetColor()));
		selectedShape->SetColor(colorPreview->GetColor());
	}
}
void ShowHideToggleButtonClick(Control* sender, MouseEvent& e) {
	Button* hideShow = dynamic_cast<Button*>(sender);
	if (!hideShow)
		return;
	if (selectedShape)
	{
		undoManager.AddCommand(new ShowHideCommand(selectedShape, selectedShape->isHidden, !selectedShape->isHidden));
		selectedShape->isHidden = !selectedShape->isHidden;
		hideShow->SetLabel(selectedShape->isHidden ? "Show" : "Hide");
	}
}

void ShowHideToggleInvalidate(Control* sender)
{
	Button* hideShow = dynamic_cast<Button*>(sender);
	if (canvas->GetSelectedShape())
		hideShow->SetLabel(selectedShape->isHidden ? "Show" : "Hide");
	else
		hideShow->SetLabel("Select shape");
}

void CreateAgglomerateButtonClick(Control* sender, MouseEvent& e)
{
	selectedShape = nullptr;

	Shapes::Agglomerate* newAgglomerate = new Shapes::Agglomerate(nullptr, canvas, colorPreview->GetColor());
	undoManager.AddCommand(new AgglomerateAssembleCommand(newAgglomerate, newAgglomerateShapes, canvas));
	newAgglomerateShapes.ForEach([&](Shapes::Shape* s) {
		s->SetOutlineAlwaysVisible(false);
		newAgglomerate->AddShape(s);
		});
	canvas->AddShape(newAgglomerate);
	newAgglomerateShapes.clear();
	//toolsButton->isHidden = false;
}

void CreateAgglomerateButtonInvalidate(Control* sender)
{

}

#pragma region Setups for Tools Buttons
void SetupCreate(Control*) {
	createShapeButton->isHidden = false;
}
void SetupEmpty(Control*) {}
void SetupSetColor(Control*) {
	setColorApplyButton->isHidden = false;
}
void SetupHideShow(Control*) {
	hideShowToggleButton->isHidden = false;
	if (!selectedShape)
		hideShowToggleButton->SetLabel("Select shape");
	else
		hideShowToggleButton->SetLabel(selectedShape->isHidden ? "Show" : "Hide");
	canvas->SetDrawHidden(true);
}
void SetupAgglomerate(Control*) {
	createAgglomerateButton->isHidden = false;
}
#pragma endregion
