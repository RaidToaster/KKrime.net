#include <windows.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "../header/Utils.h"

class Button {
private:
    int x, y;
    std::string text;
    bool isSelected;

public:
    Button(int posX, int posY, const std::string& buttonText)
        : x(posX), y(posY), text(buttonText), isSelected(false) {}

    void draw(HANDLE hConsole) {
        COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
        SetConsoleCursorPosition(hConsole, pos);
        SetConsoleTextAttribute(hConsole, isSelected ? 10 : 7);
        printf("[%s]", text.c_str());
    }

    void setSelected(bool selected) { isSelected = selected; }
    bool getSelected() const { return isSelected; }
    int getX() const { return x; }
    int getY() const { return y; }
    const std::string& getText() const { return text; }
};

class Page {
public:
    std::string content;
    Page* next;
    Page* prev;

    explicit Page(const std::string& pageContent) 
        : content(pageContent), next(NULL), prev(NULL) {}
};

class HowToPlaySystem {
private:
    Page* currentPage;
    std::vector<Button> buttons;
    int selectedButton;
    HANDLE console;

    void initializeButtons() {
        buttons.push_back(Button(30, 25, "Previous"));
        buttons.push_back(Button(45, 25, "Next"));
        buttons.push_back(Button(60, 25, "Exit"));
        //buttons[0].setSelected(true);
    }

    void drawPage() {
	    COORD pos = {0, 0};
	    SetConsoleCursorPosition(console, pos);
	    SetConsoleTextAttribute(console, 7);
	
	    printf("\n%s\n", std::string(80, '=').c_str());
	
	    std::string content = currentPage->content;
	    size_t delimiterPos = 0;
	    std::string delimiter = "\n";
	
	    while ((delimiterPos = content.find(delimiter)) != std::string::npos) {
	        std::string line = content.substr(0, delimiterPos);
	        printf("  %s\n", line.c_str());
	        content.erase(0, delimiterPos + delimiter.length());
	    }
	    printf("  %s\n", content.c_str()); 
	
	    printf("%s\n", std::string(80, '=').c_str());
	
	    for (size_t i = 0; i < buttons.size(); ++i) {
	        buttons[i].draw(console);
	    }
	}


public:
    HowToPlaySystem(HANDLE hConsole) 
        : currentPage(NULL), selectedButton(0), console(hConsole) {
        initializeButtons();

        addPage(
            "=== HUB GAME CONTROLS AND INSTRUCTIONS ===\n\n"
            "CONTROLS:\n"
            "- Use W, A, S, D keys to move your character\n"
            "- Right-click to pathfind to a location\n\n"
            "FEATURES:\n"
            "- Navigate through different rooms in the safehouse\n"
            "- Use doors (=) to move between rooms\n"
            "- Visit the Bank (B) to start the heist\n"
            "- The game saves your progress automatically\n\n"
            "TIPS:\n"
            "- Explore all rooms to find useful items\n"
            "- Plan your route before starting the heist"
        );

        addPage(
            "=== BANK HEIST GAME INSTRUCTIONS ===\n\n"
            "CONTROLS:\n"
            "- W, A, S, D: Move character\n"
            "- Left-click: Shoot in last moved direction\n"
            "- Shift: Toggle sneaking mode\n\n"
            "OBJECTIVES:\n"
            "- Find the key (K) to access the vault\n"
            "- Collect money ($) while avoiding detection\n"
            "- Reach the vault door (=) with the key\n\n"
            "HAZARDS:\n"
            "- Guards (G): Patrol and chase if they spot you\n"
            "- Cannons (C): Shoot automatically\n"
            "- Getting hit reduces health\n\n"
            "TIPS:\n"
            "- Use sneak mode to avoid detection\n"
            "- Time your movements between guard patrols\n"
            "- Shoot guards if necessary"
        );

        addPage(
            "=== MAZE VAULT GAME INSTRUCTIONS ===\n\n"
            "CONTROLS:\n"
            "- W, A, S, D: Navigate through the maze\n"
            "- H: Toggle solution path\n"
            "- Q: Exit maze (emergency only)\n\n"
            "OBJECTIVES:\n"
            "- Navigate through the randomly generated maze\n"
            "- Collect money ($) along the way\n"
            "- Reach the exit at the bottom right\n\n"
            "FEATURES:\n"
            "- Walls are displayed as solid blocks\n"
            "- Money pickups worth $100 each\n"
            "- Solution path shows optimal route when toggled\n\n"
            "TIPS:\n"
            "- Use the solution path if you get stuck\n"
            "- Try to collect all money before exiting\n"
            "- Watch your total earnings"
        );
    }

    void addPage(const std::string& content) {
        Page* newPage = new Page(content);
        
        if (!currentPage) {
            currentPage = newPage;
            currentPage->next = currentPage;
            currentPage->prev = currentPage;
        } else {
            newPage->next = currentPage->next;
            newPage->prev = currentPage;
            currentPage->next->prev = newPage;
            currentPage->next = newPage;
        }
    }

    void handleInput() {
	    COORD clickPos = waitForClick();
	    if (clickPos.X >= buttons[0].getX() && clickPos.X <= buttons[0].getX() + buttons[0].getText().length() + 2 &&
	        clickPos.Y == buttons[0].getY()) {
	        clearScreen(console);
	        buttons[0].setSelected(true);
	        buttons[1].setSelected(false);
	        buttons[2].setSelected(false);
	        currentPage = currentPage->prev;
	    } else if (clickPos.X >= buttons[1].getX() && clickPos.X <= buttons[1].getX() + buttons[1].getText().length() + 2 &&
	               clickPos.Y == buttons[1].getY()) {
	        clearScreen(console);       	
	        buttons[0].setSelected(false);
	        buttons[1].setSelected(true);
	        buttons[2].setSelected(false);
	        currentPage = currentPage->next;
	    } else if (clickPos.X >= buttons[2].getX() && clickPos.X <= buttons[2].getX() + buttons[2].getText().length() + 2 &&
	               clickPos.Y == buttons[2].getY()) {
	        clearScreen(console);
	        buttons[0].setSelected(false);
	        buttons[1].setSelected(false);
	        buttons[2].setSelected(false);
	        selectedButton = 2;
	        return;
	    }
	}

    void run() {
        bool running = true;
        while (running) {
            drawPage();
            handleInput();

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 || 
                (selectedButton == 2)) {
    			clearScreen(console);
                running = false;
            }
        }
    }

    ~HowToPlaySystem() {
        if (!currentPage) return;
        
        Page* start = currentPage;
        currentPage = currentPage->next;
        
        while (currentPage != start) {
            Page* temp = currentPage;
            currentPage = currentPage->next;
            delete temp;
        }
        delete start;
    }
};


void howToPlayMenu() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &cursorInfo);
    HowToPlaySystem howToPlay(console);
    howToPlay.run();
}
