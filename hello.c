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
    int size = grid -> size;
    char** layout = grid -> layout;

    for (int i = 0; i < size; i++) {
        free(layout[i]);
    }

    free(layout);
    free(grid);
}

void placeObjects(Board* grid) {

    int side = (grid -> size);
    int x,y;

    y = grid -> playerPos / side;
    x = grid -> playerPos % side;
    (grid -> layout)[y][x] = 'P';

    y = grid -> snakePos / side;
    x = grid -> snakePos % side;
    (grid -> layout)[y][x] = 'S';

    y = grid -> goalPos / side;
    x = grid -> goalPos % side;
    (grid -> layout)[y][x] = 'G';
}

void initializePositions(Board* grid) {
    int side = (grid -> size);

    grid -> playerPos = randLim(side*side);
    grid -> snakePos = randLim(side*side);
    grid -> goalPos = randLim(side*side);
}

Board* createBoard(int size) {
    Board* grid = malloc(sizeof(Board));

    char** columns = (char**)malloc(sizeof(char*)*size);
    if (columns == NULL) {
      exit(0);
    }

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

    grid -> size = size;
    grid -> layout = columns;

    initializePositions(grid);

    return grid;
}

void printBoard(Board* grid) {
    int size = grid -> size;
    char** layout = grid -> layout;

    placeObjects(grid);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c ", layout[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int getValidMove(Board* grid, int position, char move) {
    int side = grid -> size;

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
    int move;
    char input;
    int side = grid -> size;

    do {
        scanf("%c", &input);
        move = getValidMove(grid, grid -> playerPos, input);
    } while (move == side + 2);

    grid -> layout[grid -> playerPos/side][grid -> playerPos%side] = '.';
    grid -> playerPos += move;

}

void moveSnake(Board* grid) {
    int key, move;
    char* dict = "nwasd";
    int side = grid -> size;

    do {
        key = randLim(5);
        move = getValidMove(grid, grid -> snakePos, dict[key]);
    } while (move == side + 2);

    grid -> layout[grid -> snakePos/side][grid -> snakePos%side] = '.';
    grid -> snakePos += move;
}

int collisionDetect(Board* grid) {
    if (grid -> playerPos == grid -> snakePos) {
        printf("\n-----YOU LOSE-----\n\n");
        return 1;
    } else if (grid -> playerPos == grid -> goalPos) {
        printf("\n+++++YOU WIN+++++\n\n");
        return 1;
    }

    return 0;
}


int main() {
  srand(time(NULL));
  Board* test = createBoard(20);

  do {
      printBoard(test);
      movePlayer(test);
      moveSnake(test);
  } while (collisionDetect(test) == 0);

  freeBoard(test);
  return 0;
}
