#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <vector>
#include <utility>
#include <math.h>

#include "../header/AStar.h"
#include "../header/MultipleLinkedList.h"
#include "../header/Utils.h"
#include "../header/Guard.h"
#include "../header/PlayerHub.h"
#include "../header/Player.h"
#include "../cpp/BankGame.cpp"
#include "../cpp/PlanningGame.cpp"
#include "../cpp/Leaderboard.cpp"


bool inHub = true;

//void displayLeaderboard(HANDLE hConsole) {
//    clearScreen(hConsole);
//    gotoXY(hConsole,0,0);
//    for (int i = 0; i < 21; ++i) {
//        for (int j = 0; j < 61; ++j) {
//        	SetConsoleTextAttribute(hConsole, 7);
//        	printf(" ");
//        }
//        printf("\n");
//    }
//    std::ifstream leaderboardFile("../database/player_leaderboard.txt");
//    std::string line;
//    int y = 5;
//
//    SetConsoleTextAttribute(hConsole, 4);
//    gotoXY(hConsole, 25, y++);
//    printf("===== LEADERBOARD =====");
//    
//    SetConsoleTextAttribute(hConsole, 7);
//    y += 2;
//
//    while (std::getline(leaderboardFile, line)) {
//        std::string::size_type delimPos = line.find('#');
//        if (delimPos != std::string::npos) {
//            std::string playerName = line.substr(0, delimPos);
//            std::string playerMoney = line.substr(delimPos + 1);
//            
//            gotoXY(hConsole, 20, y);
//            printf("%-15s: $%s", playerName.c_str(), playerMoney.c_str());
//            y++;
//        }
//    }
//
//    gotoXY(hConsole, 20, y + 2);
//    printf("Press any key to continue...");
//    _getch();
//}

void exit() {
    system("cls"); 


	const char *logoLines[] = {
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#SS%?*++++++++++*?%SS#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@#%?+++??%?+KAirport++++++++++?%#@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@@@#%+++?%S#####S;yy+?%SSS#######S%?+++*%#@@@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@S?+?%S##########:y?S###################S%?++?S@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@S?+yy:+?%############*S##############%S#########S%++?S@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@%++???*+?%S####%S##S###############S%##SS#########S%+%@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@%++S#########SSSS###SS##################SSS?#############S*++%@@@@@@@@@@@@@",
    "@@@@@@@@@@@%++?S#####################################S%?+*S###############S?++%@@@@@@@@@@@",
    "@@@@@@@@@S*;S######################################%;;?##S################S*;*S@@@@@@@@@",
    "@@@@@@@@?++%################yyy##################S?+;;+%#S%###################%++?@@@@@@@@",
    "@@@@@@#;#################################SS%?+;;;%##%%#####################S*;*#@@@@@@",
    "@@@@@S++%#SS##########k#############SS%?*++;;;;+?S##%?%########################?++S@@@@@",
    "@@@@S+;yy*?S####k####kk#####SS%?*++;;;;;;;++?%S##S%*+%##########################%++S@@@@",
    "@@@S+;;+%######kk###kkk#S%++;;;;;;;;;++*?%SS##SSSSSSSS%##########################%++S@@@",
    "@@#;+%########kkSkkS##?+;;;;;;;;;;++?%SS##SSSSSSSS%##############################%++#@@@",
    "@@?;#########Skyk%##S+;;;;;;:;;++%SS#############S###########################S%kk%kk+?@@",
    "@S++S#########%yk?##S+;+;;;::;++?%SS##############################################S*k;++S@",
    "@;?##########?yk###;++;:::;++?%%S#################################################%k+*@",
    "S++S##########yy%##S+;+;::::+;%S###########S%%?**?%%?**?SS%S###################S+++S",
    "?+###########%y%##%;++;::,:;;+S##########%+;;;;;;;;;;;;;+*++;++%####################%;+?",
    ";?###########Skk##S;++;::,:;++S%S####SSS;++++++++;;;;;:::;;++;;+?####################+",
    "+;%############ykS##+;+;::,::;;+%??S##S#SSS%%%%%?????*+;;;:,,:;;;;;?##kS###############%++",
    "+;%#############Sk##?;;;;;:,,:;;+*???%%SS#########S%????+;;::,::;+;;%##kk##############%++",
    "+;%##############SS##?;;;;;;:,::;;;+*?????????S#S##S??%++;::,::;+;*##SkS#############%++",
    ";?###################?+;;+++;;;;;;;;;;;;;;;;;+%#SS####SS%;+;:,::;+;+###kk#############?+",
    "?++S###################?*++*++;;;;;;;;;++?%###########S++;:,::;+;+###kkS############+?",
    "S+;##################%%###S%?%SS%%%??%%%SS#############S%++;:::;++;###kkS###########S++S",
    "@+;y################################################SS%?+;:::;;+;+S##%ykS#####yy+y##?+*@",
    "@S+;;yS######################yy######################S%?++;::;;;+;;##SkykS#######;:%++S@",
    "@@?++yyyS###########################SS#####SS#####S%?*++;;:;;;;;;+%#Skyyk?########?;+++?@@",
    "@@#+++yyy%########################%%S%%%SS###S%%?++;;;;;;;;;;+?S#kyy#ykk#########*;++#@@",
    "@@@S++%##########################S*?%SS##S%??++;;;;;;;;++*?S###yk###yk#########;++#@@@",
    "@@@@S++%########################S*%###S?++;;;;;;++**?%%S##############k########S+++S@@@@",
    "@@@@@S+;?######################S?%##S?++;;;+*?%%S###############################;+S@@@@@",
    "@@@@@@#;*S###################S%#S?;;;+?%S###############yyy##################S*;*#@@@@@@",
    "@@@@@@@?++%#################SS#S*;;*%########################################%++?@@@@@@@@@",
    "@@@@@@@@S*;S#################%;+%########################################S*;*S@@@@@@@@@@",
    "@@@@@@@@@@%++?S##############?+?%S###########################y##########S%*++%@@@@@@@@@@@@",
    "@@@@@@@@@@@@%++S###########%?SS%S##################S########yy?yy++++++;+%@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@%+%S###########%%###################yy###########yy?+;+%@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@S?++%S########SS##############S#####S###########y++?S@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@S?++?%S##################S+#############y?*++?S@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@@@#%+++?%S#########S%?+::?########yy?+++%#@@@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@#%?+++**+++;;;+?y%???+++?%#@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#SS%?*++++++++++*?%SS#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "                      Relentlessly Move Forward and Achieve Our Dream                    ",
    "                                        24-1                                             "
};
	
	int rows = sizeof(logoLines)/sizeof(logoLines[0]);
	int COLS = strlen(logoLines[0]);
    
    for(int i = 0; i < rows; i++){
    	for(int j = 0; j < COLS; j++){
    		if (logoLines[i][j] == 'k') {
                printf("\e[1;91m%c\e[0m", logoLines[i][j]);
            } else if (logoLines[i][j] == 'y') {
                printf("\e[1;93m%c\e[0m", logoLines[i][j]);
            }
			else if (logoLines[i][j] != '@' && logoLines[i][j] != '#') {
                printf("\e[0;96m%c\e[0m", logoLines[i][j]);
            } else if (logoLines[i][j] == '@') {
                printf(" ", logoLines[i][j]);
            } else if (logoLines[i][j] == '#') {
                printf("\e[1;94m%c\e[0m", logoLines[i][j]);
            } else {
            	printf("\e[1;94m%c\e[0m", logoLines[i][j]);
			}
			if (j%20 == 0) Sleep(1);
		}
    	puts("");
	}
	
	exit(0);
}

bool checkCollision(char room[21][61], int x, int y, MultipleLinkedList*& currentRoom, PlayerHub& player, std::vector<Guard>& guards, std::string& username) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (x < 0 || x >= 61 || y < 0 || y >= 12) {
        return false;
    }
    char target = room[y][x];
    if (target == '#') {
        return false;
    } else if (target == 'T') {
        gotoXY(hConsole, 0, 63);
        printf("You found a Tool!");
        return false;
    } else if (target == '=') {
        if (y == 0 && currentRoom->up) {
            currentRoom = currentRoom->up;
            player.setPosition(x, 10);
        } else if (y == 11 && currentRoom->down) {
            currentRoom = currentRoom->down;
            player.setPosition(x, 1);
        } else if (x == 0 && currentRoom->left) {
            currentRoom = currentRoom->left;
            player.setPosition(58, y);
        } else if (x == 59 && currentRoom->right) {
            currentRoom = currentRoom->right;
            player.setPosition(1, y);
        }
        return false;
    } else if (target == 'B'){
        gotoXY(hConsole,0,13);
        printf("Do you want to go to the Bank? [Y/N]");
//        std::cout<<username;
        char input;
        while (true) {
            input = _getch(); 
            if (input == 'Y' || input == 'y') {
                bankGame(username); 
                break; 
            } else if (input == 'N' || input == 'n') {
                gotoXY(hConsole,0,13);
                printf("                                                                                        "); 
                break;
            }
        }
        return false;
    } else if (target == 'M'){
        gotoXY(hConsole,0,13);
        printf("Do you want to go to the Planning Board? [Y/N]");
        char input;
        while (true) {
            input = _getch(); 
            if (input == 'Y' || input == 'y') {
                planningGame(); 
                break; 
            } else if (input == 'N' || input == 'n') {
                gotoXY(hConsole,0,13);
                printf("                                                                                        "); 
                break;
            }
        }
        return false;
    } else if (target == 'S') {
        gotoXY(hConsole, 0, 13);
        printf("Do you want to spawn a guard? [Y/N]");
        char input;
        while (true) {
            input = _getch();
            if (input == 'Y' || input == 'y') {
                guards.push_back(Guard(58, 10));
                gotoXY(hConsole, 0, 13);
                printf("Guard Spawned!                          ");
                break;
            } else if (input == 'N' || input == 'n') {
                gotoXY(hConsole, 0, 13);
                printf("No guard spawned.                                          ");
                break;
            }
        }
        return false;
	} else if (currentRoom->layout[y][x] == 'L') {
        gotoXY(hConsole, 0, 13);
        printf("View Leaderboard? [Y/N]");
        char input;
        while (true) {
            input = _getch(); 
            if (input == 'Y' || input == 'y') {
                displayLeaderboard(hConsole);
                break; 
            } else if (input == 'N' || input == 'n') {
                gotoXY(hConsole, 0, 13);
                printf("                                                                                        "); 
                break;
            }
        }
        return false;
    } else if (currentRoom->layout[y][x] == 'E') {
        gotoXY(hConsole, 0, 13);
        printf("Exit Game? [Y/N]");
        char input;
        while (true) {
            input = _getch(); 
            if (input == 'Y' || input == 'y') {
                exit();
                break; 
            } else if (input == 'N' || input == 'n') {
                gotoXY(hConsole, 0, 13);
                printf("                                                                                        "); 
                break;
            }
        }
        return false;
	} else {
        gotoXY(hConsole,0,13);
        printf("                                                                                        ");
    }
    return true;
}

char roomTemplate[21][61] = {
    "############################################################",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "############################################################"};

char room1Layout[21][61] = {
    "#########################EEEEEEEE###########################",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "B                                                          =",
    "B                                                          =",
    "B                                                          =",
    "B                                                          =",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#########################========###########################"};

char room2Layout[21][61] = {
    "############################################################",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "=                                                         L#",
    "=                                                         L#",
    "=                                                         L#",
    "=                                                         L#",
    "#                                                          #",
    "#                                                          #",
    "#         MMMM                                             #",
    "############################################################"};

char room3Layout[21][61] = {
    "########################========############################",
    "#S                                                         #",
    "#                                                          #",
    "#                                                          #",
    "#              (x)                                         #",
    "#                                                          #",
    "#                                                          #",
    "#                                       (x)       ---------#",
    "#                                                |         #",
    "#                                                |         #",
    "#                     (x)                        |         #",
    "############################################################"};

MultipleLinkedList* currentRoom;

void hubGame(std::string username) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    clearScreen(hConsole);
    hideCursor(hConsole);
    DWORD prevMode;
    
    static MultipleLinkedList* previousRoom = 0;
    static int prevPlayerX = -1, prevPlayerY = -1;
    static std::vector<Guard> previousGuards;
    static std::vector<Bullet> previousBullets;

    MultipleLinkedList* room1 = new MultipleLinkedList(room1Layout, "Room 1");
    MultipleLinkedList* room2 = new MultipleLinkedList(room2Layout, "Room 2");
    MultipleLinkedList* room3 = new MultipleLinkedList(room3Layout, "Training Grounds");
    
    currentRoom = room1;
    room1->connect(room2, "right");
    room2->connect(room1, "left");
    room1->connect(room3, "down");
    room3->connect(room1, "up");

    currentRoom = room1;
    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 61; ++j) {
            currentRoom->layout[i][j] = room1Layout[i][j];
        }
    }

    PlayerHub player(10, 10);
    
    std::vector<Guard> trainingGroundsGuards;
    
    // Uncomment and modify if needed in C++98/C++11
    // std::vector<std::pair<int, int> > path;
    // size_t pathIndex = 0;
    
    std::vector<Bullet> trainingBullets;
    int lastDirection = 2;

    DWORD originalConsoleMode;
    GetConsoleMode(hConsole, &originalConsoleMode);
    
//    std::vector<std::pair<int, int> > prevGuardPos(trainingGroundsGuards.size());
//    std::vector<std::pair<int, int> > prevBulletPos; 

    while (true) {
        COORD cursorPosition = {0, 0};
        SetConsoleCursorPosition(hConsole, cursorPosition);

        // Render current room
        for (int i = 0; i < 21; ++i) {
            for (int j = 0; j < 61; ++j) {
                if (i == player.y && j == player.x) {
                    SetConsoleTextAttribute(hConsole, 9);
                    printf("P");
                    SetConsoleTextAttribute(hConsole, 7);
                } 
                else if (currentRoom->layout[i][j] == 'M'){
                    SetConsoleTextAttribute(hConsole, (4 << 4) | 7);
                    printf("M");
                    SetConsoleTextAttribute(hConsole, 7);
                }
                else if (currentRoom->layout[i][j] == 'B'){
                    SetConsoleTextAttribute(hConsole, (4 << 4) | 9);
                    printf("B");
                    SetConsoleTextAttribute(hConsole, 7);
                }
                else if (currentRoom->layout[i][j] == 'G'){
                    SetConsoleTextAttribute(hConsole, 2);
                    printf("G");
                    SetConsoleTextAttribute(hConsole, 7);
                }
                else if (currentRoom->layout[i][j] == 'L'){
                	SetConsoleTextAttribute(hConsole, (10 << 4) | 7);
                    printf("L");
                    SetConsoleTextAttribute(hConsole, 7);
            	}
            	else if (currentRoom->layout[i][j] == 'E'){
                	SetConsoleTextAttribute(hConsole, 4);
                    printf("%c",233);
                    SetConsoleTextAttribute(hConsole, 7);
            	}
            	else if (currentRoom->layout[i][j] == '#') {
            		printf("%c", 178);
				}
				else if (currentRoom->layout[i][j] == '=') {
            		printf("%c", 176);
				}
                else {
                    printf("%c", currentRoom->layout[i][j]);
                }
            }
            printf("\n");
        }
//		gotoXY(hConsole,0,29);
//		std::cout<<username;
        // Render bullets in training room
        //prevBulletPos.clear();
        if (currentRoom->roomName == "Training Grounds") {
            std::vector<Bullet>::iterator it = trainingBullets.begin();
            while (it != trainingBullets.end()) {
            	currentRoom->layout[it->getY()][it->getX()] = ' ';
                if (it->getX() >= 0 && it->getX() < 61 && it->getY() >= 0 && it->getY() < 21) {
                    COORD pos = {it->getX(), it->getY()};
                    SetConsoleCursorPosition(hConsole, pos);
                    SetConsoleTextAttribute(hConsole, 14);
                    currentRoom->layout[it->getY()][it->getX()] = '*';
                    SetConsoleTextAttribute(hConsole, 7);
                    //prevBulletPos.push_back({it->getX(),it->getY()});
                }
                ++it;
            }
        }


        // Commented out right-click pathfinding
        // if (!path.empty() && pathIndex < path.size()) {
        //     // Right-click pathfinding code here
        // }

        if (!player.isMoving) {
            // Movement keys
            if (GetAsyncKeyState('W') & 0x8000) {
                if (checkCollision(currentRoom->layout, player.x, player.y - 1, currentRoom, player, trainingGroundsGuards,username)) {
                    player.y -= 1;
                    lastDirection = 0; // Up
                }
            } else if (GetAsyncKeyState('S') & 0x8000) {
                if (checkCollision(currentRoom->layout, player.x, player.y + 1, currentRoom, player, trainingGroundsGuards,username)) {
                    player.y += 1;
                    lastDirection = 2; // Down
                }
            } else if (GetAsyncKeyState('A') & 0x8000) {
                if (checkCollision(currentRoom->layout, player.x - 1, player.y, currentRoom, player, trainingGroundsGuards,username)) {
                    player.x -= 1;
                    lastDirection = 3; // Left
                }
            } else if (GetAsyncKeyState('D') & 0x8000) {
                if (checkCollision(currentRoom->layout, player.x + 1, player.y, currentRoom, player, trainingGroundsGuards,username)) {
                    player.x += 1;
                    lastDirection = 1; // Right
                }
            }

            // Shooting in training room
            if (currentRoom->roomName == "Training Grounds" && GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                float directionX = 0, directionY = 0;
                switch (lastDirection) {
                    case 0: // Up
                        directionY = -1;
                        break;
                    case 1: // Right
                        directionX = 1;
                        break;
                    case 2: // Down
                        directionY = 1;
                        break;
                    case 3: // Left
                        directionX = -1;
                        break;
                }

                float magnitude = sqrt(directionX * directionX + directionY * directionY);
                directionX /= magnitude;
                directionY /= magnitude;

                trainingBullets.push_back(Bullet(player.x, player.y, directionX, directionY, true));
            }

            if (currentRoom->roomName == "Training Grounds") {
            	for (unsigned int i = 0; i < trainingGroundsGuards.size(); ++i) {
            		currentRoom->layout[trainingGroundsGuards[i].getY()][trainingGroundsGuards[i].getX()] = ' ';
	                trainingGroundsGuards[i].updateTrainingPosition(player, currentRoom->layout, trainingGroundsGuards);
	                trainingGroundsGuards[i].checkBulletCollision(trainingBullets);
	                if (trainingGroundsGuards[i].getX() >= 0 && trainingGroundsGuards[i].getX() < 61 &&
	                    trainingGroundsGuards[i].getY() >= 0 && trainingGroundsGuards[i].getY() < 12) {
	                    COORD pos = {trainingGroundsGuards[i].getX(), trainingGroundsGuards[i].getY()};
	                    SetConsoleCursorPosition(hConsole, pos);
	                    SetConsoleTextAttribute(hConsole, trainingGroundsGuards[i].getIsDead() ? 8 : 2);
	                    currentRoom->layout[trainingGroundsGuards[i].getY()][trainingGroundsGuards[i].getX()] = 'G';
	                    SetConsoleTextAttribute(hConsole, 7);
	                }
            	}
                std::vector<Bullet>::iterator it = trainingBullets.begin();
                while (it != trainingBullets.end()) {
			        if (it->getX() >= 0 && it->getX() < 61 && it->getY() >= 0 && it->getY() < 12) {
			            if (currentRoom->layout[it->getY()][it->getX()] != 'S' && 
			                currentRoom->layout[it->getY()][it->getX()] != '=') {
			                currentRoom->layout[it->getY()][it->getX()] = ' ';
			            }
			
			            currentRoom->layout[it->getY()][it->getX()] = ' ';
			            it->updateHub(currentRoom->layout);
			            
			            if (currentRoom->layout[it->getY()][it->getX()] != 'S' && 
			                currentRoom->layout[it->getY()][it->getX()] != '=') {
			                currentRoom->layout[it->getY()][it->getX()] = '*';
			            }
			        }
			
			        if (it->isOutOfBounds() || 
			            currentRoom->layout[it->getY()][it->getX()] == '#' || 
			            currentRoom->layout[it->getY()][it->getX()] == 'G' || 
			            currentRoom->layout[it->getY()][it->getX()] == 'S' || 
			            currentRoom->layout[it->getY()][it->getX()] == '=') {
			            it = trainingBullets.erase(it);
			        } else {
			            ++it;
			        }
			    }
            }

            // Commented out right-click pathfinding
            // if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && !player.isMoving) {
            // 
            // }
        }
    }
}
