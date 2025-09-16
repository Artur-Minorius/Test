#include "TerminalWindow.h"


TerminalWindow::TerminalWindow(int width, int height, Scene* scene) : width(width), height(height), scene(scene) {
    //Register this class as a window
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc; //Messages handling procedure
    wc.hInstance = GetModuleHandle(nullptr); //This program instance
    wc.lpszClassName = L"GDI+ Drawing Window Lab3"; //Class name
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //Set black background
    wc.cbWndExtra = sizeof(void*); 
    RegisterClass(&wc); // Register the class with Windows

    hwnd = CreateWindowExW(
        0, L"GDI+ Drawing Window Lab3", L"Terminal",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, //"Style"
        CW_USEDEFAULT, CW_USEDEFAULT, //Position
        width, height, //Size
        nullptr, nullptr, GetModuleHandle(nullptr), this //No parent, no menu, this program instance, this class
    );
    scene->SetHWND(&hwnd);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    SetTimer(hwnd, 1, 16, NULL);
}

TerminalWindow::~TerminalWindow() {
    scene->~Scene();
}

void TerminalWindow::messageLoop() {
    MSG msg; //Messages or "events" to process in WindowProc
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg); //Translation
        DispatchMessage(&msg); //Send to WindowProc
    }
}