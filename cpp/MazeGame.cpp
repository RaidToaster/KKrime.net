#include <windows.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <map>
#include "../header/Player.h"
#include "../header/Maze.h" 
#include "../header/Utils.h"

class MazeGame {
private:
    static const int MAZE_WIDTH = 51;
    static const int MAZE_HEIGHT = 21;
    std::vector<std::vector<int> > maze;
    std::vector<Cell> solutionPath;
    Player& player;
    HANDLE hConsole;
    int totalMoney;
    bool showSolution;

    void generateMazeWithMoney() {
        Maze baseMaze(MAZE_WIDTH, MAZE_HEIGHT);
        maze = baseMaze.getMaze(); 
        
        srand(static_cast<unsigned int>(time(NULL)));
        for (int y = 1; y < MAZE_HEIGHT - 1; y++) {
            for (int x = 1; x < MAZE_WIDTH - 1; x++) {
                if (maze[y][x] == 0 && rand() % 10 == 0) {
                    maze[y][x] = 2;
                    totalMoney += 100;
                }
            }
        }
        baseMaze.solveMaze(solutionPath);
    }

    void render() {
        COORD cursorPosition;
        cursorPosition.X = 0;
        cursorPosition.Y = 0;
        SetConsoleCursorPosition(hConsole, cursorPosition);

        for (int y = 0; y < MAZE_HEIGHT; y++) {
            for (int x = 0; x < MAZE_WIDTH; x++) {
                if (x == player.getX() && y == player.getY()) {
                    SetConsoleTextAttribute(hConsole, 12);
                    printf("P");
                }
                else if (maze[y][x] == 1) {
	                SetConsoleTextAttribute(hConsole, 7);
	                bool up = (y > 0 && maze[y - 1][x] == 1);
	                bool down = (y < MAZE_HEIGHT - 1 && maze[y + 1][x] == 1);
	                bool left = (x > 0 && maze[y][x - 1] == 1);
	                bool right = (x < MAZE_WIDTH - 1 && maze[y][x + 1] == 1);

	                if (up && down && left && right) {
	                    printf("%c", 206);      
	                } else if (up && down && left) {
	                    printf("%c", 185);      
	                } else if (up && down && right) {
	                    printf("%c", 204);      
	                } else if (up && left && right) {
	                    printf("%c", 202);     
	                } else if (down && left && right) {
	                    printf("%c", 203);      
	                } else if (up && down) {
	                    printf("%c", 186);      
	                } else if (left && right) {
	                    printf("%c", 205);      
	                } else if (up && right) {
	                    printf("%c", 200);      
	                } else if (up && left) {
	                    printf("%c", 188);      
	                } else if (down && right) {
	                    printf("%c", 201);      
	                } else if (down && left) {
	                    printf("%c", 187);      
	                } else {
	                    printf("%c", 254);      
	                }
	            }
                else if (maze[y][x] == 2) {
                    SetConsoleTextAttribute(hConsole, 14);
                    printf("$");
                }
                else if (showSolution && isOnSolutionPath(x, y)) {
                    SetConsoleTextAttribute(hConsole, 8);
                    printf(".");
                }
                else {
                    SetConsoleTextAttribute(hConsole, 7);
                    printf(" ");
                }
            }
            printf("\n");
        }

        SetConsoleTextAttribute(hConsole, 7);
        printf("\nMoney collected in maze: %d", player.getMoney() - initialMoney);
        printf("\nPress 'H' to toggle solution path");
    }

    bool isOnSolutionPath(int x, int y) {
        std::vector<Cell>::iterator it;
        for (it = solutionPath.begin(); it != solutionPath.end(); ++it) {
            if (it->x == x && it->y == y) {
                return true;
            }
        }
        return false;
    }

    void handleInput() {
        if (GetAsyncKeyState('W') & 0x8000 && !isWall(player.getX(), player.getY() - 1)) {
            player.setPosition(player.getX(), player.getY() - 1);
        }
        if (GetAsyncKeyState('S') & 0x8000 && !isWall(player.getX(), player.getY() + 1)) {
            player.setPosition(player.getX(), player.getY() + 1);
        }
        if (GetAsyncKeyState('A') & 0x8000 && !isWall(player.getX() - 1, player.getY())) {
            player.setPosition(player.getX() - 1, player.getY());
        }
        if (GetAsyncKeyState('D') & 0x8000 && !isWall(player.getX() + 1, player.getY())) {
            player.setPosition(player.getX() + 1, player.getY());
        }
        if (GetAsyncKeyState('H') & 0x8000) {
            showSolution = !showSolution;
            Sleep(100);
        }
    }

    bool isWall(int x, int y) {
        if (y < 0 || y >= MAZE_HEIGHT || x < 0 || x >= MAZE_WIDTH) {
            return true;
        }
        return maze[y][x] == 1;
    }

    void collectMoney() {
        if (maze[player.getY()][player.getX()] == 2) {
            player.addMoney(100);
            maze[player.getY()][player.getX()] = 0;
        }
    }
    
	void displayMoneyBag(HANDLE hConsole) {
	    const char* moneyBag[] = {
	    "                                                                                                                  ",
        " __      __  ______   __    __        ________   ______    ______    ______   _______   ________  _______   __ ",
        "/  \\    /  |/      \\ /  |  /  |      /        | /      \\  /      \\  /      \\ /       \\ /        |/       \\ /  |",
        "$$  \\  /$$//$$$$$$  |$$ |  $$ |      $$$$$$$$/ /$$$$$$  |/$$$$$$  |/$$$$$$  |$$$$$$$  |$$$$$$$$/ $$$$$$$  |$$ |",
        " $$  \\/$$/ $$ |  $$ |$$ |  $$ |      $$ |__    $$ \\__$$/ $$ |  $$/ $$ |__$$ |$$ |__$$ |$$ |__    $$ |  $$ |$$ |",
        "  $$  $$/  $$ |  $$ |$$ |  $$ |      $$    |   $$      \\ $$ |      $$    $$ |$$    $$/ $$    |   $$ |  $$ |$$ |",
        "   $$$$/   $$ |  $$ |$$ |  $$ |      $$$$$/     $$$$$$  |$$ |   __ $$$$$$$$ |$$$$$$$/  $$$$$/    $$ |  $$ |$$/ ",
        "    $$ |   $$ \\__$$ |$$ \\__$$ |      $$ |_____ /  \\__$$ |$$ \\__/  |$$ |  $$ |$$ |      $$ |_____ $$ |__$$ | __ ",
        "    $$ |   $$    $$/ $$    $$/       $$       |$$    $$/ $$    $$/ $$ |  $$ |$$ |      $$       |$$    $$/ /  |",
        "    $$/     $$$$$$/   $$$$$$/        $$$$$$$$/  $$$$$$/   $$$$$$/  $$/   $$/ $$/       $$$$$$$$/ $$$$$$$/  $$/ ",
        "                                                                                                               "
    };
	
	    const int moneyBagHeight = sizeof(moneyBag) / sizeof(moneyBag[0]);
	
	    for (int blink = 0; blink < 6; ++blink) {
	        clearScreen(hConsole);
	        SetConsoleTextAttribute(hConsole, (blink % 2 == 0) ? 14 : 10);
	        COORD position = {20, 10};
	        if (!SetConsoleCursorPosition(hConsole, position)) {
	            std::cerr << "Error setting cursor position!\n";
	            return;
	        }
	
	        for (int i = 0; i < moneyBagHeight; ++i) {
	            printf("%s\n", moneyBag[i]);
	        }
	
	        Sleep(300);
	    }
	
	    SetConsoleTextAttribute(hConsole, 7);
	    Sleep(3000);
	}

    bool checkExit() {
        return (player.getX() == MAZE_WIDTH - 1 && player.getY() == MAZE_HEIGHT - 2);
    }

public:
    bool exitMaze;
    int initialMoney;

    MazeGame(Player& p, HANDLE console) : 
        player(p),
        hConsole(console),
        totalMoney(0),
        showSolution(false),
        exitMaze(false),
        initialMoney(p.getMoney())
    {
        maze.resize(MAZE_HEIGHT);
        for (int i = 0; i < MAZE_HEIGHT; ++i) {
            maze[i].resize(MAZE_WIDTH);
        }
        
        generateMazeWithMoney();
        player.setPosition(0, 1);
    }

    void run() {
    	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        while (!exitMaze) {
            handleInput();
            collectMoney();
            render();
            
            if (checkExit()) {
                clearScreen(hConsole);
                player.saveStats();
    			Player::updateLeaderboard(player.getName(), player.getMoney());
                displayMoneyBag(hConsole);
                clearScreen(hConsole);
                break;
            }
        }
    }
};

void mazeGame(std::string username) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Player player;
    player.setName(username);
    MazeGame maze(player, hConsole);
    maze.run();
}
