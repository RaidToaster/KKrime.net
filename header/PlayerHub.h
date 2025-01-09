      
// PlayerHub.h
#ifndef PLAYERHUB_H
#define PLAYERHUB_H

class PlayerHub {
public:
    int x, y;
    bool isMoving;

    PlayerHub(int startX, int startY) : x(startX), y(startY), isMoving(false) {}

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }
};

#endif // PLAYERHUB_H

    
