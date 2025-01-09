#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class MultipleLinkedList{
public:
	int data;
	MultipleLinkedList* left;
	MultipleLinkedList* right;
	MultipleLinkedList* down;
	MultipleLinkedList* up;
	
	MultipleLinkedList(int value) : data(value), left(NULL), right(NULL), down(NULL), up(NULL) {}
	
	void connnect(MultipleLinkedList* node, const char* direction){
		if(strcmp(direction, "left") == 0){
			this->left = node;
			if (node) node->right = this;
		} else if (strcmp(direction, "right") == 0){
			this->right = node;
			if (node) node->left = this;
		} else if (strcmp(direction, "up") == 0){
			this->up = node;
			if (node) node->down = this;
		} else if (strcmp(direction, "down") == 0){
			this->down = node;
			if (node) node->up = this;
		}
	}
	
	MultipleLinkedList* addNode(int value, const char* direction){
		MultipleLinkedList* newNode = new MultipleLinkedList(value);
		connect(newNode, direction);
		return newNode;
	}
	
	 ~MultipleLinkedList() {
        delete left;   
        delete right;
        delete up;
        delete down;
        left = right = up = down = nullptr;
    }
};


