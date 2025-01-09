#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include "../header/Utils.h"

class Player {
private:
    int x, y;
    int health;
    int money;
    bool isMoving;
    bool isSneaking;
    bool hasKey;
    std::string name;
    int damageColorTimer;

    static void merge(std::vector<std::pair<std::string, int> >& arr, 
                      std::vector<std::pair<std::string, int> >& left, 
                      std::vector<std::pair<std::string, int> >& right) {
        size_t i = 0, j = 0, k = 0;
        
        while (i < left.size() && j < right.size()) {
            if (left[i].second > right[j].second) {
                arr[k++] = left[i++];
            } else {
                arr[k++] = right[j++];
            }
        }
        
        while (i < left.size()) {
            arr[k++] = left[i++];
        }
        
        while (j < right.size()) {
            arr[k++] = right[j++];
        }
    }
    
    static void mergeSort(std::vector<std::pair<std::string, int> >& arr) {
        if (arr.size() <= 1) {
            return;
        }
        
        size_t mid = arr.size() / 2;
        std::vector<std::pair<std::string, int> > left(arr.begin(), arr.begin() + mid);
        std::vector<std::pair<std::string, int> > right(arr.begin() + mid, arr.end());
        
        mergeSort(left);
        mergeSort(right);
        
        merge(arr, left, right);
    }

    static std::string getCurrentPlayerStatsPath() {
        return "../database/current_player_stats.txt";
    }

    static std::string getLeaderboardPath() {
        return "../database/player_leaderboard.txt";
    }

public:
    Player() : x(0), y(0), health(100), money(0), isMoving(false), isSneaking(false), hasKey(false), name("") {}
    
    Player(int startX, int startY) :
        x(startX), y(startY),
        health(100),
        money(0),
        isMoving(false),
        isSneaking(false),
        hasKey(false),
        name(""),
		damageColorTimer(0) {}

    int getX() const { return x; }
    int getY() const { return y; }
    int getHealth() const { return health; }
    int getMoney() const { return money; }
    bool getIsMoving() const { return isMoving; }
    bool getIsSneaking() const { return isSneaking; }
    bool getHasKey() const { return hasKey; }
    std::string getName() const { return name; }

    void setName(std::string playerName) { name = playerName; }
    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }
    void setMoving(bool moving) { isMoving = moving; }
    void setSneaking(bool sneaking) { isSneaking = sneaking; }
    void setHasKey(bool key) { hasKey = key; }

    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0;
    }

    void addMoney(int amount) {
        money += amount;
    }

    void saveStats() {
        std::ofstream file(getCurrentPlayerStatsPath().c_str());
        if (file.is_open()) {
            file << name << "#" << money;
            file.close();
        }
    }

    bool loadStats() {
        std::ifstream file(getCurrentPlayerStatsPath().c_str());
        std::string line;
        if (file.is_open() && std::getline(file, line)) {
            std::string::size_type delimPos = line.find('#');
            if (delimPos != std::string::npos) {
                name = line.substr(0, delimPos);
                std::stringstream ss(line.substr(delimPos + 1));
                ss >> money;
                file.close();
                return true;
            }
            file.close();
        }
        return false;
    }

    static void updateLeaderboard(std::string playerName, int playerMoney) {
        std::vector<std::pair<std::string, int> > leaderboard;
        std::ifstream inFile(getLeaderboardPath().c_str());
        std::string line;
        
        while (std::getline(inFile, line)) {
            std::string::size_type delimPos = line.find('#');
            if (delimPos != std::string::npos) {
                std::string existingName = line.substr(0, delimPos);
                std::stringstream ss(line.substr(delimPos + 1));
                int existingMoney;
                ss >> existingMoney;
                
                bool playerFound = false;
                for (size_t i = 0; i < leaderboard.size(); ++i) {
                    if (leaderboard[i].first == existingName) {
                        leaderboard[i].second = std::max(leaderboard[i].second, existingMoney);
                        playerFound = true;
                        break;
                    }
                }
                
                if (!playerFound) {
                    leaderboard.push_back(std::make_pair(existingName, existingMoney));
                }
            }
        }
        inFile.close();
        bool playerFound = false;
        for (size_t i = 0; i < leaderboard.size(); ++i) {
            if (leaderboard[i].first == playerName) {
                leaderboard[i].second = std::max(leaderboard[i].second, playerMoney);
                playerFound = true;
                break;
            }
        }

        if (!playerFound) {
            leaderboard.push_back(std::make_pair(playerName, playerMoney));
        }

        mergeSort(leaderboard);

        std::ofstream outFile(getLeaderboardPath().c_str());
        for (size_t i = 0; i < leaderboard.size(); ++i) {
            outFile << leaderboard[i].first << "#" << leaderboard[i].second << std::endl;
        }
        outFile.close();
    }
    
    void setDamageColorTimer(int timer) {
	    damageColorTimer = timer;
	}
	
	void updateDamageTimer() {
	    if (damageColorTimer > 0) {
	        damageColorTimer--;
	    }
	}
	
	bool isInDamageState() {
	    return damageColorTimer > 0;
	}

    void render(HANDLE hConsole) {
        SetConsoleTextAttribute(hConsole, isSneaking ? 8 : 12);
        gotoXY(hConsole, x, y);
        printf("P");
        SetConsoleTextAttribute(hConsole, 7); 
    }
};
#endif
