#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct gridpattern {
    int size;
    char** layout;

    int playerPos;
    int snakePos;
    int goalPos;
};

typedef struct gridpattern Board;

int randLim(int limit) {
    // return a random number between 0 and limit inclusive.
    return rand() % (limit);
}

void freeBoard(Board* grid) {
    // Frees a Board from memory
    int size = grid -> size;
    char** layout = grid -> layout;

    // Free individual rows in the board
    for (int i = 0; i < size; i++) {
        free(layout[i]);
    }

    free(layout);
    free(grid);
}

void placeObjects(Board* grid) {

    int side = (grid -> size);
    int x,y;

    // Puts a player at a specific (x,y) based on their position
    y = grid -> playerPos / side;
    x = grid -> playerPos % side;
    (grid -> layout)[y][x] = 'P';

    // Same for snake
    y = grid -> snakePos / side;
    x = grid -> snakePos % side;
    (grid -> layout)[y][x] = 'S';

    // Same for goal
    y = grid -> goalPos / side;
    x = grid -> goalPos % side;
    (grid -> layout)[y][x] = 'G';
}

void initializePositions(Board* grid) {
    // Initializes the board with random positions for all objects
    int side = (grid -> size);

    grid -> playerPos = randLim(side*side);
    grid -> snakePos = randLim(side*side);
    grid -> goalPos = randLim(side*side);
}

Board* createBoard(int size) {
    // creates a new Board and returns it
    Board* grid = malloc(sizeof(Board));

    char** columns = (char**)malloc(sizeof(char*)*size);
    if (columns == NULL) {
      exit(0);
    }

    // Creating rows in the board
    for (int i = 0; i < size; i++) {
        char* row = (char*)malloc(sizeof(char)*size);
        if (row == NULL) {
          exit(0);
        }

        for (int j = 0; j < size; j++) {
            row[j] = '.';
        }

        columns[i] = row;
    }

    // Assign all values to the new grid
    grid -> size = size;
    grid -> layout = columns;

    // Gives initial plaements for the board objects
    initializePositions(grid);

    return grid;
}

void printBoard(Board* grid) {
    // Prints out a board with all objexts and empty space
    int size = grid -> size;
    char** layout = grid -> layout;

    // Places the new objexts
    placeObjects(grid);

    // Displays the board line by line
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c ", layout[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int getValidMove(Board* grid, int position, char move) {
    // Does some math to see if a move is valid
    // Out of bound moves return board size + 2
    int side = grid -> size;

    // w: up
    // a: left
    // s: down
    // d: right
    // n: empty move
    if (move == 'w') {
        if (position - side < 0) {
            // out of bounds; call getValidMove() again
            return side + 2;
        } else {
            return -side;
        }
    } else if (move == 'a') {
        if (position % side == 0) {
            // out of bounds; call getValidMove() again
            return side + 2;
        } else {
            return -1;
        }
    } else if (move == 's') {
        if (position + side > side * side) {
            // out of bounds; call getValidMove() again
            return side + 2;
        } else {
            return side;
        }
    } else if (move == 'd') {
        if (position % side == side - 1) {
            // out of bounds; call getValidMove() aagain
             return side + 2;
        } else {
            return 1;
        }
    } else if (move == 'n') {
        return 0;
    } else {
        return side + 2;
    }
}

void movePlayer(Board* grid) {
    // Gets a player's move from the command line
    int move;
    char input;
    int side = grid -> size;

    // Continue scanning until a valid input is found
    do {
        scanf("%c", &input);
        move = getValidMove(grid, grid -> playerPos, input);
    } while (move == side + 2);

    // update the player position once a valid move is inputted
    grid -> layout[grid -> playerPos/side][grid -> playerPos%side] = '.';
    grid -> playerPos += move;

}

void moveSnake(Board* grid) {
    // Moves the snake by one tile in a random direction
    int key, move;
    char* dict = "nwasd";
    int side = grid -> size;

    // Continues trying to find a random move until a valid one is found
    do {
        key = randLim(5);
        move = getValidMove(grid, grid -> snakePos, dict[key]);
    } while (move == side + 2);

    // Update the snake position once a valid move is found
    grid -> layout[grid -> snakePos/side][grid -> snakePos%side] = '.';
    grid -> snakePos += move;
}

int collisionDetect(Board* grid) {
    // Checks if the player has collided with the snake or the goal
    if (grid -> playerPos == grid -> snakePos) {
        // Collide with snake, lose the game
        printf("\n-----YOU LOSE-----\n\n");
        return 1;
    } else if (grid -> playerPos == grid -> goalPos)
        // Collide with the goal, win the game
        printf("\n+++++YOU WIN+++++\n\n");
        return 1;
    }

    // Nothing yet, Continue playing
    return 0;
}


int main() {
    srand(time(NULL));

    // Create a new board
    Board* board = createBoard(20);

    // Run the game until the player hits the snake or the goal
    do {
        printBoard(board);
        movePlayer(board);
        moveSnake(board);
    } while (collisionDetect(board) == 0);

    // Let the past die. Kill it if you have to
    freeBoard(board);
    return 0;
}
