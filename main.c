#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

void initializeGrid(char grid[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = '  ';
            //sprintf(grid[i][j], "%d", i);

        }
    }
}

void placeMines(char grid[SIZE][SIZE], int numMines) {
    int count = 0;
    while (count < numMines) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        if (grid[x][y] != 'm') {
            grid[x][y] = 'm';
            count++;
        }
    }
}

void printGrid(char grid[SIZE][SIZE], int showMines) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 'v') {
                printf("   ");  // Display spaces for visited cells
            } else if (grid[i][j] == 'm' && showMines) {
                printf("[m] ");
            } else if (grid[i][j] == 'm') {
                printf("[ ] ", i, j);  // Display empty string instead of '[m]' until the player loses
            } else {
                printf("[%c] ", grid[i][j]);
                //printf("[%d:%d] ", i,j);
            }
        }
        printf("\n");
    }
}

int countMines(char grid[SIZE][SIZE], int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE && grid[newX][newY] == 'm') {
                count++;
            }
        }
    }
    return count;
}

void revealEmptyCells(char grid[SIZE][SIZE], int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || grid[x][y] != ' ') {
        return;
    }

    int mines = countMines(grid, x, y);
    if (mines > 0) {
        grid[x][y] = mines + '0';
    } else {
        grid[x][y] = 'v';  // 'v' for visited
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                revealEmptyCells(grid, x + i, y + j);
            }
        }
    }
}

int main() {
    srand(time(NULL));

    char grid[SIZE][SIZE];
    int remainingCells;

    do {
        initializeGrid(grid);

        int difficulty;
        printf("Choose difficulty (1, 2, or 3): ");
        scanf("%d", &difficulty);

        int numMines = 10 * difficulty;
        placeMines(grid, numMines);

        remainingCells = SIZE * SIZE - numMines;

        // Print the initial grid with coordinates
        printGrid(grid, 0);

        while (1) {
            int row, col;
            char action;
            printf("Choose a row, column, and action (0-%d) (Format: row col action (f for flag, u for unflag, r for reveal)): ",
                   SIZE - 1);
            int result = scanf("%d %d %c", &row, &col, &action);

            if (result != 3 || (action != 'f' && action != 'u' && action != 'r')) {
                printf("Invalid input. Please enter valid coordinates and action.\n");
                // Consume the remaining characters in the input buffer to avoid an infinite loop
                while (getchar() != '\n');
                continue;
            }

            if (action == 'f') {
                // Flag a mine
                if (grid[row][col] == ' ') {
                    grid[row][col] = 'd';
                }
            } else if (action == 'u') {
                // Unflag a mine
                if (grid[row][col] == 'd') {
                    grid[row][col] = ' ';
                }
            } else if (action == 'r') {
                // Reveal cell
                if (grid[row][col] == 'm') {
                    printf("Game over! You hit a mine.\n");
                    printGrid(grid, 1); // Reveal all mines on game over
                    break;  // Exit the move loop on game over
                } else if (grid[row][col] == ' ') {
                    revealEmptyCells(grid, row, col);
                    remainingCells--;

                    if (remainingCells == 0) {
                        printf("Congratulations! You've cleared all non-mine cells.\n");
                        printGrid(grid, 1);  // Reveal all mines on game win
                        break;  // Exit the move loop on game win
                    }
                }
            }

            // Print the updated grid after each move
            printGrid(grid, 0);
        }

        // Ask if the player wants to play again
        char replayChoice;
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &replayChoice);

        if (replayChoice != 'y' && replayChoice != 'Y') {
            printf("Goodbye! Exiting the program.\n");
            break;  // Exit the replay loop
        }

    } while (1);

    return 0;
}
