#ifndef CONSOLE_FUNCTIONS
#define CONSOLE_FUNCTIONS

#include <stdbool.h>
#include <windows.h>

bool rightClickEnabled = true;

inline void hideCursor(HANDLE hConsole) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; 
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

inline void disableRightClick(){
	rightClickEnabled = false;
}

inline void enableRightClick(){
	rightClickEnabled = true;
}

inline void gotoXY(HANDLE hConsole, int x, int y) {
    COORD cursorPosition = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, cursorPosition);
}

inline void clearScreen(HANDLE hConsole) {
    COORD topLeft = { 0, 0 };
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    SetConsoleTextAttribute(hConsole, 7);
    GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo);
    DWORD consoleSize = screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
}

inline COORD waitForClick() {
    HANDLE hInConsole = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD input;
    DWORD events;
    bool isLeftButtonDown = false; 

    while (true) {
        ReadConsoleInput(hInConsole, &input, 1, &events);

        if (input.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = input.Event.MouseEvent;


            if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                isLeftButtonDown = true; 
            }
            else if (isLeftButtonDown && mouseEvent.dwButtonState == 0) {
                isLeftButtonDown = false;
                return mouseEvent.dwMousePosition; 
            }
        }
    }
}

inline COORD getClick() {
    HANDLE hInConsole = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD input;
    DWORD events;
    bool isLeftButtonDown = false; 
    ReadConsoleInput(hInConsole, &input, 1, &events);

    if (input.EventType == MOUSE_EVENT) {
        MOUSE_EVENT_RECORD mouseEvent = input.Event.MouseEvent;


        if (mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            isLeftButtonDown = true; 
        }
        else if (isLeftButtonDown && mouseEvent.dwButtonState == 0) {
            isLeftButtonDown = false;
            return mouseEvent.dwMousePosition; 
        }
    }
}

inline COORD holdLeftClickPosition(HANDLE hConsole) {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inputRecord;
    DWORD events;

    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    while (true) {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);
        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                COORD pos = mouseEvent.dwMousePosition;
                return pos;
            }
        }
    }
}



inline COORD getConsoleClickPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    POINT cursorPos;

    GetCursorPos(&cursorPos);
    ScreenToClient(GetConsoleWindow(), &cursorPos);

    GetConsoleScreenBufferInfo(hConsole, &csbi);

    COORD consolePos;
    consolePos.X = cursorPos.x / 2;
    consolePos.Y = cursorPos.y / 2;

    return consolePos;
}

inline COORD getRightClickPosition(HANDLE hConsole) {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inputRecord;
    DWORD events;

    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    while (rightClickEnabled) {
        ReadConsoleInput(hInput, &inputRecord, 1, &events);
        if (inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
            if (mouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
                COORD pos = mouseEvent.dwMousePosition;
                return pos;
            }
        }
    }
}

inline void centerText(HANDLE hConsole, const char* text) {
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo);
    int consoleWidth = screenBufferInfo.dwSize.X;
    COORD pos = { (SHORT)((consoleWidth - strlen(text)) / 2), 5};
    SetConsoleCursorPosition(hConsole, pos);
    printf("%s\n", text);
}

inline void setConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

inline void disableMouseInput(HANDLE hInConsole, DWORD& prevMode) {
    GetConsoleMode(hInConsole, &prevMode);
    SetConsoleMode(hInConsole, prevMode & ~ENABLE_MOUSE_INPUT);
}

inline void enableMouseInput(HANDLE hInConsole, DWORD prevMode) {
    SetConsoleMode(hInConsole, prevMode);
}

inline void clearInnerScreen(HANDLE hConsole){
	
}

#endif

