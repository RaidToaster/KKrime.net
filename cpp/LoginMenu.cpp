#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/Utils.h"
#include "../cpp/HubGame.cpp"

unsigned long hashPassword(const char *password){
    unsigned long hash = 5381;
    int c;
    while ((c = *password++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

bool isUsernameExists(const char *username){
    FILE *file = fopen("../database/users.txt", "r");
    if (!file)
        return false;

    char line[512];
    while (fgets(line, sizeof(line), file)){
        char *nameInFile = strtok(line, "#");
        if (strcmp(nameInFile, username) == 0){
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}

bool checkPassword(const char *username, const char *password){
    FILE *file = fopen("../database/users.txt", "r");
    if (!file)
        return false;

    char line[512];
    while (fgets(line, sizeof(line), file)){
        char *nameInFile = strtok(line, "#");
        char *passwordHashInFile = strtok(NULL, "\n");
        if (strcmp(nameInFile, username) == 0){
            fclose(file);
            if (strcmp(passwordHashInFile, password) == 0){
                return true;
            }
        }
    }
    return false;
}

void inputPassword(HANDLE hConsole, char *password){
    clearScreen(hConsole);
    centerText(hConsole, "Enter your passcode using the keypad:");

    const char *keypad[4][3] = {
        {"1", "2", "3"},
        {"4", "5", "6"},
        {"7", "8", "9"},
        {"<", "0", "C"}};

    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo);

    int consoleWidth = screenBufferInfo.dwSize.X;
    int consoleHeight = screenBufferInfo.dwSize.Y;

    int keypadWidth = 3 * 4 - 1;
    int keypadHeight = 4 * 2 - 1;
    COORD keypadPos = {(SHORT)((consoleWidth - keypadWidth) / 2), keypadHeight};

    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 3; ++j){
            SetConsoleCursorPosition(hConsole, {(SHORT)(keypadPos.X + j * 4), (SHORT)(keypadPos.Y + i * 2)});
            printf("[%s]", keypad[i][j]);
        }
    }

    INPUT_RECORD input;
    DWORD events;
    int passwordIndex = 0;
    bool entering = true;
    
    char passwordDisplay[255] = "";

    while (entering){
        COORD clickPos = waitForClick();

        for (int i = 0; i < 4; ++i){
            for (int j = 0; j < 3; ++j){
                if (clickPos.X >= keypadPos.X + j * 4 && clickPos.X <= keypadPos.X + j * 4 + 2 &&
                    clickPos.Y == keypadPos.Y + i * 2){

                    if (keypad[i][j][0] == '<'){
                        if (passwordIndex > 0){
                            password[--passwordIndex] = '\0';
                            passwordDisplay[passwordIndex] = ' ';
                        }
                    }
                    else if (keypad[i][j][0] == 'C'){
                        entering = false;
                    }
                    else{
                    	passwordDisplay[passwordIndex] = keypad[i][j][0];
                        password[passwordIndex++] = keypad[i][j][0];
                        password[passwordIndex] = '\0';
                        passwordDisplay[passwordIndex] = '\0';
                    }

                    COORD passwordPos = {(SHORT)((consoleWidth - 10) / 2) - 6, (SHORT)(keypadPos.Y + keypadHeight + 2)};
                    SetConsoleCursorPosition(hConsole, passwordPos);
                    printf("Password: %s\n", passwordDisplay);
                }
            }
        }
    }
    bool isValidPassword = true;
    if (strlen(password) == 0) {
        isValidPassword = false;
    } 
    if (!isValidPassword) {
        clearScreen(hConsole);
        centerText(hConsole, "Invalid password. Password cannot be empty or contain spaces.");
        Sleep(2000);
        inputPassword(hConsole, password); 
        return;
    }

}

void displayLoginMenu(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    clearScreen(hConsole);

    char username[255] = "";
    char password[255] = "";

    // ASCII Art Welcome Banner
//    const char* welcomeArt[] = {
//        "  _  ___  __     _                             _   "
//		" | |/ / |/ /    (_)                           | |  "
//		" | ' /| ' / _ __ _ _ __ ___   ___   _ __   ___| |_ "
//		" |  < |  < | '__| | '_ ` _ \ / _ \ | '_ \ / _ \ __|"
//		" | . \| . \| |  | | | | | | |  __/_| | | |  __/ |_ "
//		" |_|\_\_|\_\_|  |_|_| |_| |_|\___(_)_| |_|\___|\__|"
//		"                                                   "
//    };

	const char* welcomeArt[] = {
	        ""
	    };


    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo);
    int consoleWidth = screenBufferInfo.dwSize.X;


    for (size_t i = 0; i < sizeof(welcomeArt)/sizeof(welcomeArt[0]); ++i) {
        SetConsoleTextAttribute(hConsole, 9 + (i % 6));
        int artWidth = strlen(welcomeArt[i]);
        COORD artPos = {(SHORT)((consoleWidth - artWidth) / 2), (SHORT)(i + 2)};
        SetConsoleCursorPosition(hConsole, artPos);
        printf("%s", welcomeArt[i]);
    }
    SetConsoleTextAttribute(hConsole, 7);


    printf("\n\n");
    const char *usernamePrompt = "Enter Your Username (5-20 characters):";
    const char *usernameHints[] = {
        "Dont go below 5 characters or above 20 Characters"
    };

//    
//    for (int i = 0; i < consoleWidth; ++i) {
//        printf("%c", (i % 2 == 0) ? '=' : '-');
//    }
    printf("\n");

    while (true) {
        srand(time(NULL));
        int hintIndex = rand() % (sizeof(usernameHints)/sizeof(usernameHints[0]));
        
        int promptWidth = strlen(usernamePrompt);
        COORD promptPos = {(SHORT)((consoleWidth - 55) / 2), screenBufferInfo.dwCursorPosition.Y+5};
        SetConsoleCursorPosition(hConsole, promptPos);
        
        SetConsoleTextAttribute(hConsole, 10);
        printf("%s\n", usernamePrompt);
        
        SetConsoleTextAttribute(hConsole, 7);
        promptPos.Y++;
        SetConsoleCursorPosition(hConsole, promptPos);
        printf("%s\n", usernameHints[hintIndex]);
		
		COORD inputPos = {(SHORT)((consoleWidth - 55) / 2), screenBufferInfo.dwCursorPosition.Y+8};
		SetConsoleCursorPosition(hConsole, inputPos);
        printf("Username: ");
        scanf("%254s", username);
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        if (strlen(username) < 5 || strlen(username) > 20) {
            SetConsoleTextAttribute(hConsole, 4);
            clearScreen(hConsole);
            centerText(hConsole, "Invalid Username Length!");
            
            if (strlen(username) < 5) {
                centerText(hConsole, "Username must be at least 5 characters long.");
            } else {
                centerText(hConsole, "Username cannot exceed 20 characters.");
            }
            
            SetConsoleTextAttribute(hConsole, 14);
			gotoXY(hConsole, (consoleWidth - 29) / 2,7);
            printf("Press any key to try again...");
            
            SetConsoleTextAttribute(hConsole, 7); 
            _getch();
            clearScreen(hConsole);
        } else {
            break;
        }
    }
    
    if (isUsernameExists(username)){
        centerText(hConsole, "User exists, please log in...");
    }
    else{
        centerText(hConsole, "New user! Set up your password:");
    }

    inputPassword(hConsole, password);
    bool isValidPassword = true;
    if (strlen(password) == 0) {
        isValidPassword = false;
    }

    if (!isValidPassword) {
        clearScreen(hConsole);
        centerText(hConsole, "Invalid password. Password cannot be empty or contain spaces.");
        Sleep(2000);
        inputPassword(hConsole, password); 
        return;
    }
    unsigned long hashedPassword = hashPassword(password);

    char hashedPasswordStr[20];
    snprintf(hashedPasswordStr, sizeof(hashedPasswordStr), "%lu", hashedPassword);

    if (!isUsernameExists(username)){
        FILE *file = fopen("../database/users.txt", "a");
        if (file){
            fprintf(file, "%s#%s\n", username, hashedPasswordStr);
            fclose(file);
            clearScreen(hConsole);
            centerText(hConsole, "New account created successfully!");
            Sleep(1000);
            clearScreen(hConsole);
        }
    }
    else
    {
        if (checkPassword(username, hashedPasswordStr)){
        	clearScreen(hConsole);
            centerText(hConsole, "Login successful.");
        }
        else
        {
        	clearScreen(hConsole);
//            centerText(hConsole, "Login failed.");
            centerText(hConsole, "Login Failed, Redirecting to the menu...");
//            mainMenu(hConsole);
			Sleep(2000);
            return;
        }
    }
	clearScreen(hConsole);
    centerText(hConsole, "Redirecting to the game...");
    Sleep(1500);
//    inMainMenu = false;
    hubGame(username);
}
