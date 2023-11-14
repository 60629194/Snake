#include"LeaderBoard.h"

void LeaderBoard() {
    printf("this is leader board");
    int choice = 0;
    int key = 10;

    while (choice != 1) {
        system("cls"); // Clears the console screen (Windows-specific)

        printf(" Leaderboard\n");
        printf("   View Scores\n");
        printf("   Return to Homepage %s\n", (choice == 1) ? "<" : "  ");

        // Non-blocking key detection for arrow keys
        if (_kbhit()) {
            key = _getch();
            if (key == 224) { // Arrow key detected
                key = _getch(); // Get the extended key code
                switch (key) {
                case 72: // Up arrow key
                    if (choice > 0) {
                        choice--;
                    }
                    break;
                case 80: // Down arrow key
                    if (choice < 1) {
                        choice++;
                    }
                    break;
                }
            }
        }
        Sleep(100);

        if (key == 13 && choice == 1) { // Enter key for "Return to Homepage"
            return; // Exit the function to return to the main menu
        }
    }
    return 0;
}