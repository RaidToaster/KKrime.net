#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <memory>
#include "../header/Utils.h"


bool running = true;
char planLayout[101][101] = {
    "#################################################################################",
    "#                                                                               #",
    "#                                                                               #",
    "#                                                                               #",
    "#              ####     #########################################################",
    "#              #                                                 C    C  ##$$$$$#",
    "#              #                                                        ==#$$$$$#",
    "#              #                                                        ==#$$$$$#",
    "#              #                                                        ==#$$$$$#",
    "#              #                                                         ##$$$$$#",
    "#              ##################################################################",
    "#              #              C                             C                   #",
    "#              #                                                                #",
    "#              #                                                                #",
    "#              #                                                                #",
    "#              #                                                                #",
    "#####      #####                                                                #",
    "#              #                                                                #",
    "#              #                                                                #",
    "#                                                                               #",
    "#                                                                               #",
    "#              #                                                                #",
    "#              #                                                               K#",
    "#################################################################################"
};

void planningGame(){
	char OGLayout[101][101] = {
    "#################################################################################",
    "#                                                                               #",
    "#                                                                               #",
    "#                                                                               #",
    "#              ####     #########################################################",
    "#              #                                                 C    C  ##$$$$$#",
    "#              #                                                        ==#$$$$$#",
    "#              #                                                        ==#$$$$$#",
    "#              #                                                        ==#$$$$$#",
    "#              #                                                         ##$$$$$#",
    "#              ##################################################################",
    "#              #              C                             C                   #",
    "#              #                                                                #",
    "#              #                                                                #",
    "#              #                                                                #",
    "#              #                                                                #",
    "#####      #####                                                                #",
    "#              #                                                                #",
    "#              #                                                                #",
    "#                                                                               #",
    "#                                                                               #",
    "#              #                                                                #",
    "#              #                                                               K#",
    "#################################################################################"
};
	running = true;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	clearScreen(hConsole);
	char draw = '*';
	while (running){
		gotoXY(hConsole, 0, 25);
		printf("Press Q/ESC to quit || Press R to reset the map || Press E to erase || Press D to draw");
		COORD cursorPosition = {0, 0};
        SetConsoleCursorPosition(hConsole, cursorPosition);
		for(int i = 0; i < 24; i++){
			for(int j = 0; j <81; j++){
				SetConsoleTextAttribute(hConsole, 7);

               
                if (planLayout[i][j] == '$') SetConsoleTextAttribute(hConsole, 14);
                if (planLayout[i][j] == '=') SetConsoleTextAttribute(hConsole, 12);
                if (planLayout[i][j] == '*') SetConsoleTextAttribute(hConsole, 14);
                if (planLayout[i][j] == 'G') SetConsoleTextAttribute(hConsole, 2);
                if (planLayout[i][j] == 'C') SetConsoleTextAttribute(hConsole, 6);
                if (planLayout[i][j] == 'K') SetConsoleTextAttribute(hConsole, 10);
				if (planLayout[i][j] == '#'){
                	SetConsoleTextAttribute(hConsole, 7);
            		printf("%c", 178);
				}else{
					printf("%c", planLayout[i][j]);
				} 

            }
            printf("\n");
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            running = false;
            clearScreen(hConsole);
            break; 
        }
        if (GetAsyncKeyState('Q') & 0x8000) {
            running = false;
            clearScreen(hConsole);
            break;
        }
        
        if (GetAsyncKeyState('E') & 0x8000) {
            draw = ' ';
        }
        
        if (GetAsyncKeyState('D') & 0x8000) {
            draw = '*';
        }
        
        if (GetAsyncKeyState('R') & 0x8000) {
            for(int i = 0; i < 24; i++){
				for(int j = 0; j <81; j++){
					planLayout[i][j] = OGLayout[i][j];
				}
			}
        }

        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            COORD clickPosition = holdLeftClickPosition(hConsole);
            int clickX = clickPosition.X;
            int clickY = clickPosition.Y;

            if (clickX > 0 && clickX < 79 && clickY > 0 && clickY < 23) {
                planLayout[clickY][clickX] = draw;
            }
        }

	}
}


