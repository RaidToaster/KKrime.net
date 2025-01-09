#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>

#include "../header/Utils.h"
#include "../cpp/LoginMenu.cpp"
#include "../cpp/HowToPlayMenu.cpp"
#include "../cpp/ExitMenu.cpp"

bool inMainMenu = false;

class MenuItem
{
public:
    int x, y;
    const char *text;

    MenuItem(int x, int y, const char *text) : x(x), y(y), text(text) {}

    void draw(HANDLE hConsole, int boxWidth, int boxHeight) const
    {
        COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};

        SetConsoleCursorPosition(hConsole, pos);
        printf("%c", 201);
        for (int i = 1; i < boxWidth - 1; i++)
            printf("%c", 205);
        printf("%c\n", 187);

        for (int i = 1; i < boxHeight - 1; i++)
        {
            pos.Y = y + i;
            SetConsoleCursorPosition(hConsole, pos);
            printf("%c", 186);
            int textPadding = (boxWidth - 2 - strlen(text)) / 2;
            if (i == boxHeight / 2)
            {
                printf("%*s%s%*s", textPadding, "", text, boxWidth - 2 - textPadding - strlen(text), "");
            }
            else
            {
                printf("%*s", boxWidth - 2, "");
            }
            printf("%c\n", 186);
        }

        pos.Y = y + boxHeight - 1;
        SetConsoleCursorPosition(hConsole, pos);
        printf("%c", 200);
        for (int i = 1; i < boxWidth - 1; i++)
            printf("%c", 205);
        printf("%c\n", 188);
    }

    bool isClickInside(int clickX, int clickY, int boxWidth, int boxHeight) const
    {
        return clickX >= x && clickX < x + boxWidth && clickY >= y && clickY < y + boxHeight;
    }
};

class Menu
{
public:
    std::vector<MenuItem> items;
    int boxWidth, boxHeight;

    Menu(int width, int height) : boxWidth(width), boxHeight(height) {}

    void addItem(const MenuItem &item)
    {
        items.push_back(item);
    }

    void draw(HANDLE hConsole) const
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            items[i].draw(hConsole, boxWidth, boxHeight);
        }
    }

    int getClickedItemIndex(COORD clickPos) const
    {
        for (size_t i = 0; i < items.size(); i++)
        {
            if (items[i].isClickInside(clickPos.X, clickPos.Y, boxWidth, boxHeight))
            {
                return i;
            }
        }
        return -1;
    }
};

//Menu setupMainMenu(){
//    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
//    HANDLE hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    GetConsoleScreenBufferInfo(hOutConsole, &screenBufferInfo);
//    int width = 30;
//    int height = 5;
//    Menu mainMenu(width, height);
//    mainMenu.addItem(MenuItem(int(screenBufferInfo.dwSize.X) / 2 - width / 2, 5, "Play"));
//    mainMenu.addItem(MenuItem(int(screenBufferInfo.dwSize.X) / 2 - width / 2, 11, "How To Play"));
//    mainMenu.addItem(MenuItem(int(screenBufferInfo.dwSize.X) / 2 - width / 2, 17, "Exit"));
//    return mainMenu;
//}

Menu setupMainMenu(){
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    HANDLE hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOutConsole, &screenBufferInfo);
    int width = 30;
    int height = 5;
    int artY = 1;
    std::ifstream artFile("../assets/title.txt");
    if (artFile.is_open()) {
        std::string line;
        artY = 1;
        while (std::getline(artFile, line)) {
            COORD artPos = {static_cast<SHORT>(int(screenBufferInfo.dwSize.X) / 2 - line.length() / 2), static_cast<SHORT>(artY)};
            SetConsoleCursorPosition(hOutConsole, artPos);
            std::cout << line << std::endl;
            artY++;
        }
        artFile.close();
    } else {
        std::cerr << "Error opening ASCII art file!" << std::endl;
    }
    Menu mainMenu(width, height);
    int menuStartY = 	artY + 2;
    mainMenu.addItem(MenuItem(int(screenBufferInfo.dwSize.X) / 2 - width / 2, menuStartY, "Play"));
    mainMenu.addItem(MenuItem(int(screenBufferInfo.dwSize.X) / 2 - width / 2, menuStartY + 6, "How To Play"));
    mainMenu.addItem(MenuItem(int(screenBufferInfo.dwSize.X) / 2 - width / 2, menuStartY + 12, "Exit"));
    return mainMenu;
}

Menu setupGameMenu(){
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    HANDLE hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOutConsole, &screenBufferInfo);
    int width = 30;
    int height = 5;
    Menu gameMenu(width, height);
    gameMenu.addItem(MenuItem(int(screenBufferInfo.dwSize.X) / 2 - width / 2 + 2, 5, "Resume"));
    gameMenu.addItem(MenuItem(int(screenBufferInfo.dwSize.X) / 2 - width / 2 + 2, 11, "Quit to Main Menu"));
    return gameMenu;
}

void mainMenu(HANDLE hConsole){
    Menu mainMenu = setupMainMenu();
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    HANDLE hOutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOutConsole, &screenBufferInfo);
    while (true){
    	int artY = 1;
    	std::ifstream artFile("../assets/title.txt");
	    if (artFile.is_open()) {
	        std::string line;
	        artY = 1;
	        while (std::getline(artFile, line)) {
	            COORD artPos = {static_cast<SHORT>(int(screenBufferInfo.dwSize.X) / 2 - line.length() / 2), static_cast<SHORT>(artY)};
	            SetConsoleCursorPosition(hOutConsole, artPos);
	            std::cout << line << std::endl;
	            artY++;
	        }
	        artFile.close();
	    } else {
	        std::cerr << "Error opening ASCII art file!" << std::endl;
	    }
        mainMenu.draw(hConsole);
        //printf("%d\n", inMainMenu);
        COORD clickPos = waitForClick();
        int clickedIndex = mainMenu.getClickedItemIndex(clickPos);

        if (clickedIndex == -1)
            continue;
        clearScreen(hConsole);
        if (clickedIndex == 0)
        {
//            inMainMenu = false;
            displayLoginMenu();
        }
        else if (clickedIndex == 1)
        {
            howToPlayMenu();
        }
        else if (clickedIndex == 2)
        {
            exitMenu();
        }

        clearScreen(hConsole);
    }
}

