#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <memory>
#include "../header/Player.h"
#include "../header/Guard.h"
#include "../header/Cannon.h"
#include "../header/Bullet.h"
#include "../header/Pathfinding.h"

#include "MazeGame.cpp"

const char bankLayout[101][101] = {
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

const char* gameOverArt = 
"   _____          __  __ ______    ______      ________ _____  _ \n"
"  / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\ |\n"
" | |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) | |\n"
" | | |_ | / /\\ \\ | |\\/| |  __|   | |  | | \\\\/ / |  __| |  _  / |\n"
" | |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\_|\n"
"  \\_____/_/    \\\\_\\|  |_|______|  \\____/   \\/   |______|_|  \\_(_)\n"
"                                                                 \n"
"                                                                 \n";

class BankGame {
private:
    Player player;
    std::vector<Guard> guards;
    std::vector<Cannon> cannons;
    std::vector<Bullet> bullets;
    char gameMap[101][101];
    HANDLE hConsole;
    Pathfinding pathfinder;
    int lastDirection;
    bool running = true;
    

    void initializeGuards() {
        Guard guard1(30, 8);
        std::vector<std::pair<int, int> > path1;
        path1.push_back(std::make_pair(30, 8));
        path1.push_back(std::make_pair(20, 8));
        path1.push_back(std::make_pair(20, 3));
        path1.push_back(std::make_pair(20, 8));
        path1.push_back(std::make_pair(30, 8));
        guard1.setPatrolPath(path1);
        guards.push_back(guard1);

        Guard guard2(50, 6);
        std::vector<std::pair<int, int> > path2;
        path2.push_back(std::make_pair(50, 6));
        path2.push_back(std::make_pair(50, 7));
        path2.push_back(std::make_pair(50, 8));
        path2.push_back(std::make_pair(50, 9));
        guard2.setPatrolPath(path2);
        guards.push_back(guard2);
        
        Guard guard3(55,14);
        Guard guard4(55,20);
        guards.push_back(guard3);
        guards.push_back(guard4);
    }

    void initializeCannons() {
        cannons.push_back(Cannon(65, 5));
        cannons.push_back(Cannon(70, 5));
        cannons.push_back(Cannon(30, 11));
        cannons.push_back(Cannon(60, 11));
    }

    void handlePlayerInput() {
        if (GetAsyncKeyState('W') & 0x8000 && gameMap[player.getY() - 1][player.getX()] != '#' && gameMap[player.getY() - 1][player.getX()] != 'C') {
            player.setPosition(player.getX(), player.getY() - 1);
            lastDirection = 0;
        }
        if (GetAsyncKeyState('S') & 0x8000 && gameMap[player.getY() + 1][player.getX()] != '#' && gameMap[player.getY() + 1][player.getX()] != 'C') {
            player.setPosition(player.getX(), player.getY() + 1);
            lastDirection = 2;
        }
        if (GetAsyncKeyState('A') & 0x8000 && gameMap[player.getY()][player.getX() - 1] != '#' && gameMap[player.getY()][player.getX() - 1] != 'C') {
            player.setPosition(player.getX() - 1, player.getY());
            lastDirection = 3;
        }
        if (GetAsyncKeyState('D') & 0x8000 && gameMap[player.getY()][player.getX() + 1] != '#' && gameMap[player.getY()][player.getX() + 1] != 'C') {
            player.setPosition(player.getX() + 1, player.getY());
            lastDirection = 1;
        }
        if (GetAsyncKeyState(VK_ESCAPE)) {
            running = false;
            return;
        }
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
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

            bullets.push_back(Bullet(player.getX(), player.getY(), directionX, directionY, true));
        }
        player.setSneaking(GetAsyncKeyState(VK_SHIFT) & 0x8000);
    }


    void updateEntities(std::string username) {
    	player.updateDamageTimer();
//    	gotoXY(hConsole, 84,1);
//    	std::cout<<player.isInDamageState();
    	gotoXY(hConsole, 0, 24);
    	std::cout<<username;
        if (player.getX() == 79 && player.getY() == 22) {
            player.setHasKey(true);
            gameMap[22][79] = ' '; 
        }

        if (gameMap[player.getY()][player.getX()] == '=') {
            if (player.getHasKey()) {
            	clearScreen(hConsole);
            	running = false;
                mazeGame(username);
            } else {
                gotoXY(hConsole, 0, 26);
                printf("You need the key to open the vault door!");
            }
        }

        for (std::vector<Guard>::iterator it = guards.begin(); it != guards.end(); ++it) {
            it->checkPlayerProximity(player, gameMap);
            it->updatePosition(gameMap, player, pathfinder);
			it->checkBulletCollision(bullets);
            if (it->isCollidingWith(player.getX(), player.getY()) && it->getIsDead() == false) {
                it->attemptToDamagePlayer(player);
            }
            it->updateCooldown();
        }

        for (std::vector<Cannon>::iterator it = cannons.begin(); it != cannons.end(); ++it) {
            it->shoot(bullets,it->getX(), it->getY());
        }

        for (std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end();) {
		    it->update(gameMap);
		
		    if (it->checkCollision(player, player.getX(), player.getY()) || it->isOutOfBounds()) {
		        it = bullets.erase(it);
		    } else {
		        ++it;
		    }
		}

        if (gameMap[player.getY()][player.getX()] == '$') {
            player.addMoney(100);
            gameMap[player.getY()][player.getX()] = ' ';
        }
    }

    void render() {
    	SetConsoleTextAttribute(hConsole, 7);
        COORD cursorPosition = {0, 0};
        SetConsoleCursorPosition(hConsole, cursorPosition);

        char frameBuffer[24][101];
        memcpy(frameBuffer, gameMap, sizeof(frameBuffer));

        for (std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
            if (it->getY() >= 0 && it->getY() < 24 && it->getX() >= 0 && it->getX() < 101) {
                frameBuffer[it->getY()][it->getX()] = '*';
            }
        }

        for (std::vector<Guard>::iterator it = guards.begin(); it != guards.end(); ++it) {
            frameBuffer[it->getY()][it->getX()] = 'G';
        }

        for (std::vector<Cannon>::iterator it = cannons.begin(); it != cannons.end(); ++it) {
            frameBuffer[it->getY()][it->getX()] = 'C';
        }

        frameBuffer[player.getY()][player.getX()] = 'P';

        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < strlen(gameMap[i]); j++) {
                SetConsoleTextAttribute(hConsole, 7);

               
                if (frameBuffer[i][j] == '$') SetConsoleTextAttribute(hConsole, 14);
                if (frameBuffer[i][j] == '=') SetConsoleTextAttribute(hConsole, 12);
                if (frameBuffer[i][j] == '*') SetConsoleTextAttribute(hConsole, 14);
                if (frameBuffer[i][j] == 'G') SetConsoleTextAttribute(hConsole, 2);
                if (frameBuffer[i][j] == 'C') SetConsoleTextAttribute(hConsole, 6);
                if (frameBuffer[i][j] == 'K') SetConsoleTextAttribute(hConsole, 10);
                if (frameBuffer[i][j] == 'P') {
			        SetConsoleTextAttribute(hConsole, 
			            player.isInDamageState() ? 4 :
			            (player.getIsSneaking() ? 8 : 9) 
			        );
			    }
				if (frameBuffer[i][j] == '#'){
                	SetConsoleTextAttribute(hConsole, 7);
            		printf("%c", 178);
				}else{
					printf("%c", frameBuffer[i][j]);
				} 

            }
            printf("\n");
        }
		
		gotoXY(hConsole, 0, 25);
    	printf("                                                                                ");
        gotoXY(hConsole, 0, 25);
        printf("Health: %d | Sneaking: %s | Key: %s",
               player.getHealth(),
               player.getIsSneaking() ? "Y" : "N",
               player.getHasKey() ? "Y" : "N");
    	gotoXY(hConsole, 0, 26);
    	printf("You can exit by pressing ESC");
    }
    
    void centerAsciiArt(HANDLE hConsole, const char* art) {
	    CONSOLE_SCREEN_BUFFER_INFO csbi;
	    GetConsoleScreenBufferInfo(hConsole, &csbi);
	    int screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	
	    std::vector<std::string> lines;
	    std::string line;
	    const char* p = art;
	    while (*p) {
	        if (*p == '\n') {
	            lines.push_back(line);
	            line = "";
	        } else {
	            line += *p;
	        }
	        p++;
	    }
	    lines.push_back(line);
	
	    int artHeight = lines.size();
	    int startRow = (csbi.srWindow.Bottom - csbi.srWindow.Top + 1 - artHeight) / 2;
	    int startCol = (screenWidth - lines[0].length()) / 2;
	
	    COORD pos;
	    for (size_t i = 0; i < lines.size(); ++i) {
	        pos.X = startCol;
	        pos.Y = startRow + i;
	        SetConsoleCursorPosition(hConsole, pos);
	        std::cout << lines[i] << std::endl;
	    }
	}

public:
    BankGame()
        : player(2, 20),
          hConsole(GetStdHandle(STD_OUTPUT_HANDLE)),
          pathfinder(101, 101, gameMap) {
        clearScreen(hConsole);
        hideCursor(hConsole);
        memcpy(gameMap, bankLayout, sizeof(gameMap));
        initializeGuards();
        initializeCannons();
    }
    
	Player getPlayer(){
    	return player;
	}

    void run(std::string username) {
    	if (running == false) clearScreen(hConsole);
        while (running) {
            handlePlayerInput();
            updateEntities(username);
            render();
            if (running == false) {
            	gotoXY(hConsole,79,22);
            	SetConsoleTextAttribute(hConsole,7);
            	clearScreen(hConsole);	
			}
            if (player.getHealth() <= 0) {
            	gotoXY(hConsole,79,22);
            	SetConsoleTextAttribute(hConsole,7);
                gotoXY(hConsole, 30, 12);
                clearScreen(hConsole);
                SetConsoleTextAttribute(hConsole, 4);
                centerAsciiArt(hConsole, gameOverArt);
                //player.saveStats();
                Sleep(3000);
                clearScreen(hConsole);
                break;
            }
        }
    }
};

void bankGame(std::string username) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    BankGame game;
    game.getPlayer().setName(username);
    game.run(username);
}
