#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "../header/Utils.h"

int stringToInt(const std::string& str) {
    std::istringstream iss(str);
    int num;
    iss >> num;
    return num;
}

struct PlayerEntry {
    std::string name;
    int money;
    
    PlayerEntry() : money(0) {}
    PlayerEntry(const std::string& n, int m) : name(n), money(m) {}
};

bool comparePlayerEntriesDescending(const PlayerEntry& a, const PlayerEntry& b) {
    return a.money > b.money;
}

bool comparePlayerEntriesAscending(const PlayerEntry& a, const PlayerEntry& b) {
    return a.money < b.money;
}

class LeaderboardManager {
private:
    std::vector<PlayerEntry> entries;
    static const int ENTRIES_PER_PAGE = 8;
    int currentPage;
    bool sortAscending;

    void loadLeaderboard() {
        std::ifstream leaderboardFile("../database/player_leaderboard.txt");
        entries.clear();
        std::string line;

        while (std::getline(leaderboardFile, line)) {
            std::string::size_type delimPos = line.find('#');
            if (delimPos != std::string::npos) {
                std::string playerName = line.substr(0, delimPos);
                int playerMoney = stringToInt(line.substr(delimPos + 1));
                entries.push_back(PlayerEntry(playerName, playerMoney));
            }
        }

        sortEntries();
    }

    void sortEntries() {
        if (sortAscending) {
            std::sort(entries.begin(), entries.end(), comparePlayerEntriesAscending);
        } else {
            std::sort(entries.begin(), entries.end(), comparePlayerEntriesDescending);
        }
    }

    void drawBorder(HANDLE hConsole) {
        SetConsoleTextAttribute(hConsole, 4);
        
        // Top border
        gotoXY(hConsole, 10, 2);
        printf("%c", 218);
        for (int i = 0; i < 40; i++) printf("%c", 196);
        printf("%c", 191);

        // Bottom border
        gotoXY(hConsole, 10, 14);
        printf("%c", 192);
        for (int i = 0; i < 40; i++) printf("%c", 196);
        printf("%c", 217);

        // Side borders
        for (int i = 3; i < 14; i++) {
            gotoXY(hConsole, 10, i);
            printf("%c", 179);
            gotoXY(hConsole, 51, i);
            printf("%c", 179);
        }
    }

    void drawButtons(HANDLE hConsole) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        
        // Previous Button
        gotoXY(hConsole, 12, 16);
        printf("[Prev]");

        // Next Button
        gotoXY(hConsole, 25, 16);
        printf("[Next]");

        // Sort Button
        gotoXY(hConsole, 38, 16);
        printf("[Sort]");

        // Exit Button
        gotoXY(hConsole, 45, 16);
        printf("[Exit]");
    }

    std::string intToString(int number) {
        std::ostringstream oss;
        oss << number;
        return oss.str();
    }

public:
    LeaderboardManager() : currentPage(0), sortAscending(false) {}

    void displayLeaderboard(HANDLE hConsole) {
        loadLeaderboard();
        
        while (true) {
            clearScreen(hConsole);
            drawBorder(hConsole);
            drawButtons(hConsole);

            SetConsoleTextAttribute(hConsole, 4);
            gotoXY(hConsole, 22, 3);
            printf("=== LEADERBOARD ===");

            int startIndex = currentPage * ENTRIES_PER_PAGE;
            int endIndex = startIndex + ENTRIES_PER_PAGE;
            if (endIndex > static_cast<int>(entries.size())) {
                endIndex = entries.size();
            }

            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            for (int i = startIndex; i < endIndex; i++) {
                gotoXY(hConsole, 20, 5 + (i - startIndex));
                char buffer[50];
                sprintf(buffer, "%-16s : $%-10d", 
                    entries[i].name.c_str(), 
                    entries[i].money
                );
                printf("%s", buffer);
            }

            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            gotoXY(hConsole, 25, 15);
            int totalPages = (entries.size() + ENTRIES_PER_PAGE - 1) / ENTRIES_PER_PAGE;
            char pageBuffer[30];
            sprintf(pageBuffer, "Page %d of %d", currentPage + 1, totalPages);
            printf("%s", pageBuffer);

            COORD clickPos = waitForClick(); 

            if (clickPos.X >= 12 && clickPos.X <= 18 && clickPos.Y == 16) {
                // Previous Button
                if (currentPage > 0) currentPage--;
            }
            else if (clickPos.X >= 25 && clickPos.X <= 30 && clickPos.Y == 16) {
                // Next Button
                int totalPages = (entries.size() + ENTRIES_PER_PAGE - 1) / ENTRIES_PER_PAGE;
                if (currentPage < totalPages - 1) 
                    currentPage++;
            }
            else if (clickPos.X >= 38 && clickPos.X <= 42 && clickPos.Y == 16) {
                // Sort Button
                sortAscending = !sortAscending;
                sortEntries();
            }
            else if (clickPos.X >= 45 && clickPos.X <= 49 && clickPos.Y == 16) {
                // Exit Button
                clearScreen(hConsole);
                break;
            }
            
        }
    }
};

void displayLeaderboard(HANDLE hConsole) {
	gotoXY(hConsole,0,0);
	for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 61; ++j) {
        	SetConsoleTextAttribute(hConsole, 7);
        	printf(" ");
        }
        printf("\n");
    }
    LeaderboardManager leaderboardManager;
    leaderboardManager.displayLeaderboard(hConsole);
}
