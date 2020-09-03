#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <tuple>
#include <vector>
#include <queue>

using namespace std;

const short int COLLUMNS = 30;
const short int ROWS = 30;
const short int SPEED = 1;

short int grid[ROWS][COLLUMNS] = {};
queue<pair<short int, short int>> snakeBody;
short int snakeX = 15; 
short int snakeY = 15;
short int direction = 2; // Up: 1, Down: -1, Right: 2, Left: 0

bool gameOver = false;
int points = 0;

// Prototypes //
void displayGrid(); // Outputs the grid's current state
void moveSnake(); // Moves the snake based on variables
void getKeyPress(); // Gets the key pressed [YIELDS]
void handleInput(); // Handles snake movement
void mainMenu(); // Main menu
void generateFood(); // Generates one food randomly on the grid
void gameEnd(); // Show end screen

int main() {  
  struct termios old_tio, new_tio;

  // These functions let you detect keypresses without having to press enter 
  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

  mainMenu();

  // Set up default snake length
  for (int i=0; i<1; i++) {
    snakeX++;
    snakeBody.emplace(snakeX, snakeY);
    grid[snakeY][snakeX] = 1;
  }

  std::thread first(handleInput);

  generateFood();
  while (!gameOver) {
    moveSnake();
    displayGrid();
    this_thread::sleep_for(chrono::milliseconds(400));
  }
  gameEnd();

  return 0;
}

void mainMenu() {
  cout << "==============================================\n";
  cout << "==============================================\n";
  cout << "============ Alphexus's Snake Game ===========\n";
  cout << "==============================================\n";
  cout << "==============================================\n";
  cout << "==============================================\n";
  cout << "============= Press \"p\" to play ==============\n";
  cout << "==============================================\n";
  cout << "==============================================\n";
  char c = 'x';
  while (c!='p') {
    c = getchar();
  }
}

void gameEnd() {
  cout << "\033[2J"; // Clears the terminal
  cout << "==============================================\n";
  cout << "==============================================\n";
  cout << "================= GAME OVER ==================\n";
  cout << "==============================================\n";
  cout << "==============================================\n"; 
  cout << "  You ended up with " << points << " point(s) \n"; 
}

void generateFood() {
  int randX = rand() % COLLUMNS + 1;
  int randY = rand() % ROWS + 1; 
  if (grid[randY][randX] == 1) {
    return generateFood();
  }
  grid[randY][randX] = -1;
}

void displayGrid() {
  cout << "\033[2J"; // Clears the terminal
  cout << "Points: " << points << "                    Alphexus's Terminal C++ Snake Game\n";

  for (int y=0; y<30; y++) {
    for (int x=0; x<30; x++) {
      int state = grid[y][x];
      if (state == 0) {
        cout << "[ ]";
      } else if (state == 1) {
        cout << " * ";
      } else if (state == -1) {
        cout << " @ ";
      }
    }
    cout << "\n";
  }
}

// Some algorithm I made to get the tail of the snake 
// Which became useless as I found a better option by using queues

/*tuple <int, int> findSnakeTail(int visited[COLLUMNS][ROWS], int x, int y) {
  for (int nextY=-1; nextY<2; nextY++) {
    for (int nextX=-1; nextX<2; nextX++) {
      if (grid[y+nextY][x+nextX] == 1 && visited[y+nextY][x+nextX] == 0) {
        visited[y+nextY][x+nextX] = 1;
        return findSnakeTail(visited, x+nextX, y+nextY);
      }
    }
  }

  return std::make_tuple(x, y);
}*/ 

void updatePosition() {
  if (direction == 1) {
    snakeY--;
  } else if (direction == -1) {
    snakeY++;
  } else if (direction == 2) {
    snakeX++;
  } else if (direction == 0) {
    snakeX--;
  }
}

void moveSnake() {
  pair tail = snakeBody.front();
  short int x = tail.first;
  short int y = tail.second;
  snakeBody.pop();

  grid[y][x] = 0; // Snake tail
  updatePosition();

  if (snakeY == 30 || snakeY == 0) {
    gameOver = true;
  } else if (snakeX == 30 || snakeX == 0) {
    gameOver = true;
  }

  if (grid[snakeY][snakeX] == -1) { // If it's food
    snakeBody.emplace(snakeX, snakeY);
    grid[snakeY][snakeX] = 1;
    updatePosition();
    snakeBody.emplace(snakeX, snakeY);
    grid[snakeY][snakeX] = 1;
    points++;
    generateFood();
  } else if (grid[snakeY][snakeX] == 1) {
    gameOver = true;
  } else {
    snakeBody.emplace(snakeX, snakeY);
    grid[snakeY][snakeX] = 1;
  }
};

void handleInput() {
  while (true) {
    char c = getchar();
    if (c == 'w') {
      direction = 1;
    } else if(c == 'a') {
      direction = 0;
    } else if(c == 's') {
      direction = -1;
    } else if(c == 'd') {
      direction = 2;
    } 
  }
}
