#ifndef GUARD_H
#define GUARD_H
#include <vector>
#include <utility>
#include <windows.h>
#include "../header/Pathfinding.h"
#include "Player.h"
#include "PlayerHub.h"
#include "Bullet.h"

class Guard {
private:
    int x, y;
    bool isAlert;
    std::vector<std::pair<int, int> > patrolPath;
    size_t pathIndex;
    static const int detectionRange = 5;
    bool isDead;
    int damageCooldown;
    bool isTrainingMode;
    int moveDelay;
    int moveCounter;

    bool hasLineOfSight(const char layout[101][101], int x1, int y1, int x2, int y2) const {
        // Bresenham's line
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (x1 != x2 || y1 != y2) {
            if (layout[y1][x1] == '#') {
                return false;
            }

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
        return true;  // No walls detected
    }

public:
    Guard(int startX, int startY) : 
        x(startX), 
        y(startY), 
        isAlert(false), 
        pathIndex(0),
        isDead(false),
		damageCooldown(0),
		moveDelay(2),
		moveCounter(0) {}

    int getX() const { return x; }
    int getY() const { return y; }
    bool getIsAlert() const { return isAlert; }
    bool getIsDead() const { return isDead; }

    void setPatrolPath(const std::vector<std::pair<int, int> >& path) {
        patrolPath = path;
        pathIndex = 0;
    }

    void updatePosition(char layout[101][101], Player& player, Pathfinding& pathfinder) {
        if (isDead) return; 
        if (isAlert) {
        	moveCounter++;
            if (moveCounter < moveDelay) return; // Only update every moveDelay frames
            moveCounter = 0;
            std::vector<std::pair<int, int> > path = pathfinder.findPath(x, y, player.getX(), player.getY());
            if (path.size() > 1) {
                x = path[1].first;
                y = path[1].second;
            }
        } else if (!patrolPath.empty()) {
        	moveCounter++;
            if (moveCounter < moveDelay) return; // Only update every moveDelay frames
            moveCounter = 0;
            int targetX = patrolPath[pathIndex].first;
            int targetY = patrolPath[pathIndex].second;

            if (x < targetX) x++;
            else if (x > targetX) x--;

            if (y < targetY) y++;
            else if (y > targetY) y--;

            if (x == targetX && y == targetY) {
                pathIndex = (pathIndex + 1) % patrolPath.size();
            }
        }
    }
    
    void updateTrainingPosition(const PlayerHub& player, char layout[21][61], const std::vector<Guard>& otherGuards) {
	    if (isDead) return;
	
	    moveCounter++;
	    if (moveCounter < moveDelay) return;
	    moveCounter = 0;
	
	    int dx = player.x - x;
	    int dy = player.y - y;
	
	    int moveX = 0, moveY = 0;
	    if (dx != 0) moveX = (dx > 0) ? 1 : -1;
	    if (dy != 0) moveY = (dy > 0) ? 1 : -1;
	
	    int newX = x, newY = y;
	    bool positionBlocked = false;
	
	    // Prefer moving in primary direction
	    if (abs(dx) > abs(dy)) {
	        newX = x + moveX;
	        newY = y;
	    } else {
	        newX = x;
	        newY = y + moveY;
	    }
	
	    // Check wall collision
	    if (layout[newY][newX] == '#') {
	        positionBlocked = true;
	    }
	
	    // Check other guard collision
	    for (size_t i = 0; i < otherGuards.size(); ++i) {
	        if (&otherGuards[i] != this && 
	            otherGuards[i].x == newX && 
	            otherGuards[i].y == newY) {
	            positionBlocked = true;
	            break;
	        }
	    }
	
	    // Update position if not blocked
	    if (!positionBlocked) {
	        x = newX;
	        y = newY;
	    }
	}
    
    void attemptToDamagePlayer(Player& player) {
        if (damageCooldown <= 0) {
            player.takeDamage(4);
            player.setDamageColorTimer(3);
            damageCooldown = 5;
        }
    }

    void updateCooldown() {
        if (damageCooldown > 0)
            damageCooldown--;
    }

    void checkBulletCollision(std::vector<Bullet>& bullets) {
        for (int i = 0; i < bullets.size(); i++) {
            Bullet& bullet = bullets[i];
            if (bullet.getX() == x && bullet.getY() == y) {
                isDead = true; 
                break;  
            }
        }
    }

    void checkPlayerProximity(const Player& player, const char layout[101][101]) {
        int dx = abs(x - player.getX());
        int dy = abs(y - player.getY());
        
        if (dx <= detectionRange && dy <= detectionRange && !player.getIsSneaking()) {
            if (hasLineOfSight(layout, x, y, player.getX(), player.getY())) {
                isAlert = true;
            }
        }
    }

    void render(HANDLE hConsole) const {
        if (isDead) return;
        gotoXY(hConsole, x, y);
        SetConsoleTextAttribute(hConsole, isAlert ? 4 : 2);
        printf("G");
        SetConsoleTextAttribute(hConsole, 7);
    }

    bool isCollidingWith(int playerX, int playerY) const {
        return x == playerX && y == playerY;
    }
};
#endif
