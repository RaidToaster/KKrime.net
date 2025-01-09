#ifndef BULLET_H
#define BULLET_H

#include <windows.h>
#include <cmath>
#include "Player.h"

class Bullet {
private:
    float x, y;
    float vx, vy;
    bool isPlayerBullet;
    static const int damage = 10;
    static const int speed = 1;

public:
    Bullet(int startX, int startY, float directionX, float directionY, bool fromPlayer)
    : x(startX), y(startY), isPlayerBullet(fromPlayer) {
	    float magnitude = sqrt(directionX * directionX + directionY * directionY);
	    vx = (directionX / magnitude) * speed;
	    vy = (directionY / magnitude) * speed;
	}

    int getX() const { return static_cast<int>(x); }
    int getY() const { return static_cast<int>(y); }

    void update(char gameMap[101][101]) {
        float newX = x + vx;
        float newY = y + vy;

        if (newX >= 0 && newX < 101 && newY >= 0 && newY < 101 && gameMap[static_cast<int>(newY)][static_cast<int>(newX)] != '#') {
            x = newX;
            y = newY;
        } else {
            x = -1;
            y = -1;
        }
    }
    
    void updateHub(char gameMap[21][61]) {
        float newX = x + vx;
        float newY = y + vy;

        if (newX >= 0 && newX < 61 && newY >= 0 && newY < 21 && gameMap[static_cast<int>(newY)][static_cast<int>(newX)] != '#') {
            x = newX;
            y = newY;
        } else {
            x = -1;
            y = -1;
        }
    }

    bool isOutOfBounds() const {
        return x < 0 || x >= 101 || y < 0 || y >= 101;
    }

    bool checkCollision(Player& player, int shooterX, int shooterY) const {
	    if (x == shooterX && y == shooterY) {
	        return false;
	    }
        if (abs(x - player.getX()) < 2 && abs(y - player.getY()) < 2) {
            if (!isPlayerBullet) {
                player.takeDamage(damage);
                player.setDamageColorTimer(3);
                return true;
            }
            return false;
        }
	    return false;
	}

    void render(HANDLE hConsole) const {
        gotoXY(hConsole, static_cast<int>(x), static_cast<int>(y));
        SetConsoleTextAttribute(hConsole, 14);
        printf("*");
        SetConsoleTextAttribute(hConsole, 7);
    }
};

#endif
