#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <string>

#include "../header/Utils.h"
#include "MainMenu.cpp"

int main() {
	bool running = true;
	HANDLE hInConsole = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD prevMode = 0;
	GetConsoleMode(hInConsole, &prevMode);
    SetConsoleMode(hInConsole, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | (prevMode & ~ENABLE_QUICK_EDIT_MODE));
	clearScreen(hOutConsole);
	mainMenu(hOutConsole);
	return 0;
}
