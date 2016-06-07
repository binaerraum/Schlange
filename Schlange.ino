//
// Schlange for Arduboy https://www.arduboy.com/
// 2016 Patrick Gerdsmeier <pgATbinaerraumDOTde>
//

#include <Arduboy.h>
#include <stdlib.h>


//
// Def
//
#define BLOCKSIZE  4
#define XMAX       (WIDTH / BLOCKSIZE)
#define YMAX       (HEIGHT / BLOCKSIZE)
#define SNAKEMAX   255
#define FRAMERATE  2


//
// Types
//
enum Direction {
    none,
    left,
    right,
    up,
    down
};

struct Pos {
    byte x;
    byte y;
};

struct Snake {
    byte len;
    Pos pos[SNAKEMAX + 1];
    Direction currentDirection;
};

typedef Pos Apple;


//
// Vars
//
Arduboy arduboy;

Snake snake;
Apple apple;
int score;


//
// Check collisions
//
bool didHitBoundaries(Pos *pos) {

    return  
        pos->x < 0 ||
        pos->x >= XMAX ||
        pos->y < 0 ||
        pos->y >= YMAX;
}


bool didHitSnake(Pos *pos) {
    
    for (int i = 0; i < snake.len; i++) {
        
        if (snake.pos[i].x == pos->x && 
            snake.pos[i].y == pos->y) {
        
            return true;
        }

    }

    return false;
}


bool didHitApple(Pos *pos) {

    return
        pos->x == apple.x && 
        pos->y == apple.y;
}


//
// Apple
//
void initApple() {

    do {
        apple.x = rand() % XMAX;
        apple.y = rand() % YMAX;
    } while (didHitSnake(&apple));
}
    

void drawApple() {

    arduboy.drawRect(
        apple.x * BLOCKSIZE, 
        apple.y * BLOCKSIZE, 
        1 * BLOCKSIZE,
        1 * BLOCKSIZE,
        1
    );
}


// 
// Snake
//
void initSnake() {

    snake.len = 3;
    snake.pos[0] = {12, 5};
    snake.pos[1] = {11, 5};
    snake.pos[2] = {10, 5};
    snake.currentDirection = right;
}


void drawSnake() {

    for (int i = 0; i < snake.len; i++) {
        arduboy.fillRect(
            snake.pos[i].x * BLOCKSIZE, 
            snake.pos[i].y * BLOCKSIZE, 
            1 * BLOCKSIZE, 
            1 * BLOCKSIZE,
            1
        );
    }
}


void moveSnake(Pos nextPos) {

    for (int i = snake.len; i > 0; i--) {
        snake.pos[i] = snake.pos[i - 1];
    }

    snake.pos[0] = nextPos;
}


void eatApple() {

    if (snake.len < SNAKEMAX) {
        snake.len += 1;
    }
    
    score += 1;
    initApple();
}


Pos getNextPos(Pos pos, Direction direction) {

    switch (direction) {
        case left:
          pos.x--;
          break;
        case right:
          pos.x++;
          break;
        case up:
          pos.y--;
          break;
        case down:
          pos.y++;
          break;          
    }

    return pos;
}


//
// Init
//
void initGame() {

    score = 0;

    initSnake();
    initApple();
}
  

//
// Main init
//
void setup() {

    arduboy.begin();
    arduboy.setFrameRate(FRAMERATE);
    initGame();
}


//
// Main loop
//
void loop() {

    // Check key        
    if(arduboy.pressed(RIGHT_BUTTON)) {
        snake.currentDirection = right;
    }

    if(arduboy.pressed(LEFT_BUTTON)) {
        snake.currentDirection = left;
    }
    
    if(arduboy.pressed(UP_BUTTON)) {
        snake.currentDirection = up;
    }
    
    if(arduboy.pressed(DOWN_BUTTON)) {
        snake.currentDirection = down;
    }

    // Check framerate
    if (!(arduboy.nextFrame())) {
    
        return;
    }

    // Get next position
    Pos currentPos = snake.pos[0];
    Pos nextPos = getNextPos(currentPos, snake.currentDirection);

    // Check collisions
    if (didHitSnake(&nextPos) ||
        didHitBoundaries(&nextPos)) {
        initGame();

        return;
    }

    // Check if next pos is on the apple
    else
    if (didHitApple(&nextPos)) {
        eatApple();
    }

    moveSnake(nextPos);

    arduboy.clear();

    arduboy.setCursor(0, 0);
    arduboy.print(score);
    
    drawApple();    
    drawSnake();

    arduboy.display();
}

