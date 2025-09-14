#include <iostream>
#include <windows.h>
#include "TerminalWindow.h"
#include "List.h"
#include "Form1.h"
int main() {
    Form1* form = new Form1();
    TerminalWindow terminal(800, 620, form);
    terminal.messageLoop();
    return 0;
}