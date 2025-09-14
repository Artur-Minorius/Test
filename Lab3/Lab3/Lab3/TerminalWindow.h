#pragma once
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include "Scene.h"
#include "Shape.h"
#include "Rectangle.h"
#include "MouseEvent.h"
class TerminalWindow {
public:
	TerminalWindow(int width, int height, Scene*);
	~TerminalWindow();
	void messageLoop();

private:
	HWND hwnd; // Window Handle
	int width, height; // Size
	Scene* scene;
	bool isMouseDown = false;
	bool isDragging = false;
	// Window procedure : handles all window messages
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		// Get access to Terminal Window pointer instance stored in window data
		TerminalWindow* self = reinterpret_cast<TerminalWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	
		if (self) {
			MouseEvent e(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), self->isDragging, uMsg);
			if (self->scene) {
				self->scene->HandleMouseEvent(e);
			}
		}
		switch (uMsg) {
		case WM_NCCREATE: { //Before creation
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
			return TRUE;
		}
		case WM_DESTROY: // Closing
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN: // Kew presses
			std::cout << "Key pressed: " << wParam << std::endl;
			return 0;

		case WM_CHAR:  // Character typed (respects shift/caps)
			std::cout << "Char typed: " << (char)wParam << std::endl;
			return 0;

		case WM_MOUSEMOVE: { // Mouse moved over window - get acutal coordinates of the mouse
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			std::cout << "Mouse moved to: (" << x << ", " << y << ")" << std::endl;
			if (self->isMouseDown) {
				self->isDragging = true;
			}
			return 0;
		}
		case WM_LBUTTONDOWN: { // LMB down
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			std::cout << "Left mouse button down at: (" << x << ", " << y << ")" << std::endl;
			/*Shapes::Rectangle* s = new Shapes::Rectangle(x, y, 5, 5, COLORREF(RGB(255, 255, 255)));
			self->scene->AddShape(s);*/
			self->isMouseDown = true;
			return 0;
		}

		case WM_LBUTTONUP: { // LMB up
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			std::cout << "Left mouse button up at: (" << x << ", " << y << ")" << std::endl;
			self->isMouseDown = false;
			self->isDragging = false;
			return 0;
		}
		case WM_TIMER:
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case WM_SETCURSOR: {
			if (self->scene)
			{
				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(hwnd, &pt);
				MouseEvent e(pt.x, pt.y, self->isDragging, uMsg);
				Control* hovered = self->scene->FindControlAt(e);
				
				if (hovered) {
					SetCursor(LoadCursor(NULL, hovered->GetHoverCursor()));
					return TRUE;
				}
				else {
					SetCursor(LoadCursor(NULL, IDC_ARROW));
					return TRUE;
				}
			}
		}
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			self->scene->DrawAll();
			EndPaint(hwnd, &ps);
			return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam); //Dispatch other events to default windows procedure
	}
};