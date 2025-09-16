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

#include "Animation.h"

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
void CreateAnimationButtonClick(Control* sender, MouseEvent& e);
void ShowHideToggleInvalidate(Control* sender);

void CreateAgglomerateButtonClick(Control* sender, MouseEvent& e);

void ShowHideToggleInvalidate(Control* sender);

void SetPreviewShape(Shapes::Point);

void Reset(bool doResetSelectedShape = false);

template <typename F, typename CmdF>
void BaseSelectedShapeLogic(MouseEvent&, F, CmdF);

void CreateAnimationLogic(MouseEvent& e);
List<String*> labelsShapes;
int currShape = 1; //Access to current shape
//More C# WinFormApp code
Canvas* canvas; // Global canvas object

Shapes::Shape* previewShape;
Shapes::Shape* selectedShape;
List<Shapes::Shape*> newAgglomerateShapes;
Shapes::Animation* currAnimation;
enum Tools { Create, Move, Rotate, Scale, SetColor, HideShow, CreateAgglomerate, Delete, CreateAnimation };

Tools currTool = Create;

//Button* toolsButton;
Button* createShapeButton;
Button* setColorApplyButton;
Button* hideShowToggleButton;
Button* createAgglomerateButton;
Button* createAnimationButton;

Label* currToolHelpLabel;

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
void SetupAnimation(Control*);

String createHelp = "Click on canvas to place start point, when enough points were placed, shape will auto fill. You can cycle through different shapes using button in the right bottom corner";
String moveHelp = "Click on shape you'd like to move, click on appeared control points to move shape";
String rotateHelp = "Click on shape you'd like to rotate, click on appeared control points to rotate shape (around center)";
String scaleHelp = "Click on shape you'd like to scale, click on appeared control points to scale shape";
String setColorHelp = "Click on shape whose color you'd like to change, select color using RGB bars in the top right corner and click \"Apply\"";
String showHideHelp = "Click on shape you'd like to show or hide, click on appeared button in the right bottom corner to show or hide selected shape";
String deleteHelp = "Click on shape you'd like to delete (you cannot recover it)";
String createAgglomerateHelp = "Click on shapes you'd like to make agglomerate from. Click \"Submit\" button to create agglomerate";
String createAnimationHelp = "Click on shape you'd like to create animation for. Place control points for the animation, by clicking on canvas. Click \"Submit\" button to end animation creation";
#pragma endregion


struct ToolButton {
	using SpecialFunction = void(*)(Control*);
	Button* button;
	Tools tool;
	SpecialFunction specialFunction;
	String* help;
	bool doResetSelectedShape = false;

};

List<ToolButton> toolButtons;

void Form1::InitializeComponents() {

	undoManager.SetScene(this);
	Shapes::Point toolsStartPos(630, 170);
	Shapes::Point baseSize(150, 20);
	int indx = 0;
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Create"), Create, SetupCreate, &createHelp, true });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Move"), Move, SetupEmpty, &moveHelp });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Rotate"), Rotate, SetupEmpty, &rotateHelp });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Scale"), Scale, SetupEmpty, &scaleHelp });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Set color"), SetColor, SetupSetColor, &setColorHelp });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Show/Hide"), HideShow, SetupHideShow, &showHideHelp });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Delete"), Delete, SetupEmpty, &deleteHelp, true });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Create agglomerate"), CreateAgglomerate, SetupAgglomerate, &createAgglomerateHelp });
	toolButtons.push_back({ new Button(toolsStartPos + Shapes::Point(0, 30 * indx++), baseSize, "Create animation"), CreateAnimation, SetupAnimation, &createAnimationHelp });
	toolButtons.ForEach([&](ToolButton tB) {
		tB.button->SetOnClick(ToolsButtonClick);
		AddControl(tB.button);
		});
	currToolHelpLabel = new Label(Shapes::Point(50, 520), Shapes::Point(650, 30), *toolButtons[0].help);
	currToolHelpLabel->SetColor(RGB(255, 255, 255));
	AddControl(currToolHelpLabel);

#pragma region Buttom Right Corner Buttons

	Shapes::Point pos(630, 480);

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
	createAgglomerateButton = new Button(pos, Shapes::Point(150, 20), "Submit");
	createAgglomerateButton->isHidden = true;
	createAgglomerateButton->SetOnClick(CreateAgglomerateButtonClick);
	AddControl(createAgglomerateButton);

	//  Hide/show button
	hideShowToggleButton = new Button(pos, Shapes::Point(150, 20), "Select shape");
	hideShowToggleButton->isHidden = true;
	hideShowToggleButton->SetOnClick(ShowHideToggleButtonClick);
	hideShowToggleButton->SetOnInvalidate(ShowHideToggleInvalidate);
	AddControl(hideShowToggleButton);

	createAnimationButton = new Button(pos, Shapes::Point(150, 20), "Submit");
	createAnimationButton->isHidden = true;
	createAnimationButton->SetOnClick(CreateAnimationButtonClick);
	AddControl(createAnimationButton);
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
	Shapes::Point colorPreviewStart(630, 50);
	//RGB slider
	//R
	Label* rLabel = new Label(colorPreviewStart + Shapes::Point(-20, 30), Shapes::Point(20, 20), "R");
	rLabel->SetColor(RGB(255, 255, 255));
	AddControl(rLabel);
	Slider* redValSlider = new Slider(0, 255, 1, colorPreviewStart + Shapes::Point(0, 30), Shapes::Point(120, 20));
	redValSlider->SetColor(RGB(255, 255, 255));
	redValSlider->SetOnMouseMove(RedSliderMouseMove);
	AddControl(redValSlider);
	//G
	Label* gLabel = new Label(colorPreviewStart + Shapes::Point(-20, 60), Shapes::Point(20, 20), "G");
	gLabel->SetColor(RGB(255, 255, 255));
	AddControl(gLabel);
	Slider* greenValSlider = new Slider(0, 255, 1, colorPreviewStart + Shapes::Point(0, 60), Shapes::Point(120, 20));
	greenValSlider->SetColor(RGB(255, 255, 255));
	greenValSlider->SetOnMouseMove(GreenSliderMouseMove);
	AddControl(greenValSlider);
	//B
	Label* bLabel = new Label(colorPreviewStart + Shapes::Point(-20, 90), Shapes::Point(20, 20), "B");
	bLabel->SetColor(RGB(255, 255, 255));
	AddControl(bLabel);
	Slider* blueValSlider = new Slider(0, 255, 1, colorPreviewStart + Shapes::Point(0, 90), Shapes::Point(120, 20));
	blueValSlider->SetColor(RGB(255, 255, 255));
	blueValSlider->SetOnMouseMove(BlueSliderMouseMove);
	AddControl(blueValSlider);

	colorPreview = new Shapes::Rectangle(colorPreviewStart, colorPreviewStart + Shapes::Point(120, 20), //Size
		RGB(redValSlider->GetValue(), greenValSlider->GetValue(), blueValSlider->GetValue()));
	AddShape(colorPreview);
#pragma endregion

	auto tempME = MouseEvent(-1, -1, 0, -1);
	toolButtons[0].button->Click(tempME);
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
			});

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
	case Delete:
		canvas->RemoveShape(canvas->SelectAt(e.GetLocalPosition())); //There would be safe-statement if the agglomerates worked normally
		break;
	case CreateAnimation:
		CreateAnimationLogic(e);
		break;
	}

}

void SetPreviewShape(Shapes::Point pos) {
	delete previewShape;
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
	createAnimationButton->isHidden = true;
	newAgglomerateShapes.clear();
	canvas->SetPreviewShape(nullptr);
	previewShape = nullptr;
	if (doResetSelectedShape)
	{
		selectedShape = nullptr;
		canvas->Unselect();
	}
	if (currAnimation) {
		delete currAnimation;
		currAnimation = nullptr;
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
		if (selectedShape)
		{
			selectedShape->Move(e.GetLocalPosition());
		}
		break;
	case Rotate:
		if (selectedShape)
		{
			selectedShape->Rotate(e.GetLocalPosition());
		}
		break;
	case Scale:
		if (selectedShape)
		{
			selectedShape->Scale(e.GetLocalPosition());
		}
		break;
	case CreateAnimation:
		if (currAnimation) {
			currAnimation->UpdatePreviewPoint(e.LocalX(), e.LocalY());
		}
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
	currToolHelpLabel->SetLabel(*t.help);
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

void CreateAnimationLogic(MouseEvent& e) {
	if (!selectedShape)
		selectedShape = canvas->SelectAt(e.GetLocalPosition());
	if (selectedShape)
	{
		if (!currAnimation)
		{
			currAnimation = new Shapes::Animation(selectedShape, 60, 20, selectedShape->GetColor());
			selectedShape = (Shapes::Shape*)currAnimation;
			canvas->SetPreviewShape(selectedShape);
		}
		else
			currAnimation->AddPoint(e.GetLocalPosition());
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

void CreateAnimationButtonClick(Control* sender, MouseEvent& e)
{
	if (currAnimation)
	{
		currAnimation->EndCreating();
		currAnimation->Start();
		canvas->AddAnimation(currAnimation);
		canvas->SetPreviewShape(nullptr);
		currAnimation = nullptr;
		toolButtons[0].button->Click(e);
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
void SetupAnimation(Control*)
{
	createAnimationButton->isHidden = false;
}
#pragma endregion
