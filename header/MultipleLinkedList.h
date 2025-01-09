#ifndef MULTIPLE_LINKED_LIST_H
#define MULTIPLE_LINKED_LIST_H

#include <iostream>
#include <string>
#include <vector>

class MultipleLinkedList {
public:
    char layout[21][61];
    std::string roomName;
    std::vector<std::string> items;

    MultipleLinkedList *left;
    MultipleLinkedList *right;
    MultipleLinkedList *up;
    MultipleLinkedList *down;

    MultipleLinkedList(const char map[21][61], const std::string &name = "Unnamed Room") 
        : roomName(name), left(NULL), right(NULL), up(NULL), down(NULL) {
        for (int i = 0; i < 21; ++i) {
            for (int j = 0; j < 61; ++j) {
                layout[i][j] = map[i][j];
            }
        }
    }

    void connect(MultipleLinkedList *room, const std::string &direction) {
        if (direction == "left") {
            this->left = room;
            if (room)
                room->right = this;
        } else if (direction == "right") {
            this->right = room;
            if (room)
                room->left = this;
        } else if (direction == "up") {
            this->up = room;
            if (room)
                room->down = this;
        } else if (direction == "down") {
            this->down = room;
            if (room)
                room->up = this;
        }
    }

    void addItem(const std::string &item) {
        items.push_back(item);
    }

    void displayRoomInfo() const {
        std::cout << "Room: " << roomName << "\nItems in room: ";
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << items[i] << " ";
        }
        std::cout << "\n";
    }

    void displayLayout() const {
        for (int i = 0; i < 21; ++i) {
            for (int j = 0; j < 61; ++j) {
                std::cout << layout[i][j];
            }
            std::cout << '\n';
        }
    }

    ~MultipleLinkedList() {
        delete left;
        delete right;
        delete up;
        delete down;
        left = right = up = down = NULL;
    }
};

#endif

