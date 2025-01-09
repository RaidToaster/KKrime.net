#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <cmath>
#include <iostream>
#include <algorithm>

class AStar {
public:
    struct Node {
        int x, y;
        int g, h;
        Node* parent;

        Node(int x, int y, int g = 0, int h = 0, Node* parent = NULL)
            : x(x), y(y), g(g), h(h), parent(parent) {}

        int f() const { return g + h; }
    };

    AStar(int width, int height, const char layout[21][61])
        : width(width), height(height), layout(layout) {}

    std::vector<std::pair<int, int> > findPath(int startX, int startY, int goalX, int goalY) {
        std::vector<std::pair<int, int> > path;
        std::priority_queue<Node*, std::vector<Node*>, CompareNodes> openList;
        std::vector<std::vector<bool> > closedList(height, std::vector<bool>(width, false));

        Node* startNode = new Node(startX, startY, 0, calculateHeuristic(startX, startY, goalX, goalY));
        openList.push(startNode);

        while (!openList.empty()) {
            Node* currentNode = openList.top();
            openList.pop();

            if (currentNode->x == goalX && currentNode->y == goalY) {
                while (currentNode != NULL) {
                    path.push_back({ currentNode->x, currentNode->y });
                    currentNode = currentNode->parent;
                }
                std::reverse(path.begin(), path.end());
                break;
            }

            closedList[currentNode->y][currentNode->x] = true;

            for (int i = 0; i < 4; ++i) {
                int newX = currentNode->x + directions[i].first;
                int newY = currentNode->y + directions[i].second;

                if (isValid(newX, newY) && !closedList[newY][newX] && layout[newY][newX] != '#') {
                    int g = currentNode->g + 1;
                    int h = calculateHeuristic(newX, newY, goalX, goalY);
                    Node* neighbor = new Node(newX, newY, g, h, currentNode);
                    openList.push(neighbor);
                }
            }
            
            for(int i = 0; i < path.size(); ++i) { 
	            Node* nodeToDelete = NULL;
	

	            Node * curr = startNode; 
	            while(curr != NULL && (curr->x != path[i].first || curr->y != path[i].second)){
	                curr = curr->parent;
	            }
	            nodeToDelete = curr;
	            
	            if(nodeToDelete != NULL){
	                delete nodeToDelete;
	            }
	        }
        }
        return path;
    }

private:
    int width, height;
    const char(*layout)[61];


    const std::pair<int, int> directions[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    int calculateHeuristic(int x1, int y1, int x2, int y2) const {
        return abs(x1 - x2) + abs(y1 - y2);
    }

    bool isValid(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    struct CompareNodes {
        bool operator()(Node* a, Node* b) {
            return a->f() > b->f();
        }
    };
};

#endif

