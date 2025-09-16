#include "Label.h"

Label::Label(Shapes::Point position, Shapes::Point dimensions, String text) : Control(position, dimensions)
{
	delete this->text;
	this->text = new String(text);
	cursorHoverType = IDC_ARROW;
}
void Label::WrapText(String* text, Gdiplus::Font& font, Gdiplus::Graphics& g, float maxWidth)
{
	multiLine.clear();
	String* currentLine = new String();
	int start = 0;

	while (start < text->Length()) {
		//Next space
		int spaceIndex = text->IndexOf(' ', start);
		if (spaceIndex == -1) spaceIndex = text->Length();

		//Next word
		String* word = text->Substring(start, spaceIndex);

		//Create test line and add space
		String* testLine = new String(*currentLine);
		if (currentLine->Length() > 0)
			testLine->Append(" ");
		testLine->Append(*word);

		//Measure if fit
		Gdiplus::RectF layoutRect;
		Gdiplus::RectF boundingBox;
		Gdiplus::StringFormat format;
		g.MeasureString(testLine->ToWCharArray(), testLine->Length(), &font, layoutRect, &format, &boundingBox);

		if (boundingBox.Width > maxWidth) {
			//Test line doesn't fit, (that means previous one does)
			if (currentLine->Length() > 0) {
				multiLine.push_back(currentLine);
				currentLine = word; //start new line with current word
			}
			else {
				//Word itself too long, force-break
				multiLine.push_back(word);
				currentLine = new String();
			}
		}
		else {
			delete currentLine;
			currentLine = testLine;
		}

		start = spaceIndex + 1;
	}

	if (currentLine->Length() > 0)
		multiLine.push_back(currentLine);
	isMultiLineComputed = true;
}

void Label::Draw(Gdiplus::Graphics& g)
{
	Gdiplus::SolidBrush brush(Gdiplus::Color(GetAlpha(), GetRValue(color), GetGValue(color), GetBValue(color)));
	//g.DrawString(text->ToWCharArray(), -1, &font, (Gdiplus::PointF)position, &brush);
	if (!isMultiLineComputed)
		WrapText(text, font, g, dimensions.x);
	float yOffset = 0;
	for (int i = 0; i < multiLine.size(); i++) {
		g.DrawString(multiLine[i]->ToWCharArray(), -1, &font,
			Gdiplus::PointF(position.x, position.y + yOffset), &brush);
		yOffset += font.GetHeight(&g);
	}
}

bool Label::isMouseOver(MouseEvent& e)
{
	return ((e.X() > position.x && e.X() < position.x + dimensions.x) &&
		(e.Y() > position.y && e.Y() < position.y + dimensions.y));
}

void Label::SetLabel(String s)
{
	isMultiLineComputed = false;
	Control::SetLabel(s);
}
