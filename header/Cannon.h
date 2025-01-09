#ifndef CANNON_H
#define CANNON_H

#include <vector>
#include <utility>
#include <windows.h>
#include "Bullet.h"

class Cannon {
private:
    int x, y;
    int cooldown;
    bool active;
    static const int maxCooldown = 6;

public:
    Cannon(int startX, int startY) : 
        x(startX), 
        y(startY), 
        cooldown(0), 
        active(true) {}

    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return active; }

    void shoot(std::vector<Bullet>& bullets, int shooterX, int shooterY) {
	    if (cooldown <= 0 && active) {
	        bullets.push_back(Bullet(shooterX, shooterY, 0, 1, false)); 
	        cooldown = maxCooldown;
	    }
	    if (cooldown > 0) cooldown--;
	}


    void toggle() {
        active = !active;
    }

    void render(HANDLE hConsole) const {
        gotoXY(hConsole, x, y);
        SetConsoleTextAttribute(hConsole, 6);
        printf("C");
        SetConsoleTextAttribute(hConsole, 7);
    }
};

#endif
