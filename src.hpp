#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <utility>
#include <deque>
#include <vector>

struct Map;
enum class instruction{UP, DOWN, LEFT, RIGHT, NONE};
//the instruction of the snake

bool is_food(Map* map, int x, int y);
//return true if the cell at (x,y) is food

bool is_wall(Map* map, int x, int y);
//return true if the cell at (x,y) is wall

void eat_food(Map* map, int x, int y);
//remove the food at (x,y)

int get_height(Map* map);
//return the height of the map

int get_width(Map* map);
//return the width of the map


struct Snake{
  // store the necessary information of the snake
  // use any data structure you like
  // you can also add any necessary function
  //------------------------------------------
  // TODO
  
  std::deque<std::pair<int, int>> body;  // body positions, front is head
  instruction current_direction;
  
  void initialize(int x, int y, instruction ins){
    // (x,y) is the position of the head of the snake, ins is the initial orientation of the snake
    body.clear();
    body.push_front({x, y});
    current_direction = ins;
  }
  
  int get_length(){
    //return the length of the snake
    return body.size();
  }
  
  bool move(Map* map, instruction ins){
    //the current map and the next instruction
    //return false if the snake is dead
    //return true if the snake is alive
    // If the instruction is NONE, keep the current direction
    if (ins != instruction::NONE) {
        // Check for reverse direction (e.g., going down and then up)
        if ((current_direction == instruction::UP && ins == instruction::DOWN) ||
            (current_direction == instruction::DOWN && ins == instruction::UP) ||
            (current_direction == instruction::LEFT && ins == instruction::RIGHT) ||
            (current_direction == instruction::RIGHT && ins == instruction::LEFT)) {
            return false;  // Snake dies if it tries to move in reverse direction
        }
        current_direction = ins;
    }
    
    // Calculate new head position
    int new_x = body.front().first;
    int new_y = body.front().second;
    
    switch(current_direction) {
        case instruction::UP:
            new_x--;
            break;
        case instruction::DOWN:
            new_x++;
            break;
        case instruction::LEFT:
            new_y--;
            break;
        case instruction::RIGHT:
            new_y++;
            break;
        default:
            break;
    }
    
    // Check if the new position hits a wall or the snake's body
    if (is_wall(map, new_x, new_y)) {
        return false;
    }
    
    // Check if the new position hits the snake's body (excluding the tail which will move)
    for (size_t i = 0; i < body.size() - 1; i++) {  // Don't check the tail
        if (body[i].first == new_x && body[i].second == new_y) {
            return false;
        }
    }
    
    // Check if the new position is food
    bool has_food = is_food(map, new_x, new_y);
    if (has_food) {
        eat_food(map, new_x, new_y);
        // If there's food, we don't pop the tail (snake grows)
    } else {
        // If no food, remove the tail
        body.pop_back();
    }
    
    // Add the new head position
    body.push_front({new_x, new_y});
    
    return true;
  }
  
  std::pair<int, std::pair<int,int>*> get_snake(){
    //return the length of the snake and a pointer to the array of the position of the snake
    //you can store the head as the first element  
    int length = body.size();
    std::pair<int, int>* positions = new std::pair<int, int>[length];
    
    int i = 0;
    for (const auto& pos : body) {
        positions[i++] = pos;
    }
    
    return {length, positions};
  }
};

const int MaxWidth = 20;
struct Map{
  // store the necessary information of the map
  bool wall[MaxWidth][MaxWidth];
  bool food[MaxWidth][MaxWidth];
  int width, height;
  
  int get_height(){
    //return the height of the map
    return height;
  }
  
  int get_width(){
    //return the width of the map
    return width;
  }
  
  bool is_food(int x, int y){
    //return true if the cell at (x,y) is food
    if (x < 0 || x >= height || y < 0 || y >= width) {
        return false;
    }
    return food[x][y];
  }
  
  void eat_food(int x, int y){
    //eat the food at (x,y)
    if (x >= 0 && x < height && y >= 0 && y < width) {
        food[x][y] = false;
    }
  }
  
  bool is_wall(int x, int y){
    //return true if the cell at (x,y) is wall
    if (x < 0 || x >= height || y < 0 || y >= width) {
        return true;  // Out of bounds is considered a wall
    }
    return wall[x][y];
  }
  // use any data structure you like
  // you can also add any necessary function
  //------------------------------------------
  //TODO
  
  
  void initialize(Snake *snake){
    char str[MaxWidth];
    int head_x = -1 , head_y = -1;
    instruction ins;
    
    // Read map dimensions
    std::cin >> height >> width;
    
    // Read the map layout
    for (int i = 0; i < height; i++) {
        std::cin >> str;
        for (int j = 0; j < width; j++) {
            wall[i][j] = (str[j] == '#');
            food[i][j] = (str[j] == '*');
            if (str[j] == '@') {
                head_x = i;
                head_y = j;
            }
        }
    }
    
    // Read initial direction
    char dir;
    std::cin >> dir;
    switch(dir) {
        case 'U':
            ins = instruction::UP;
            break;
        case 'D':
            ins = instruction::DOWN;
            break;
        case 'L':
            ins = instruction::LEFT;
            break;
        case 'R':
            ins = instruction::RIGHT;
            break;
        default:
            ins = instruction::NONE;
            break;
    }
    
    // Initialize the snake
    snake->initialize(head_x, head_y, ins);
  }
  
  void print(Snake *snake){
    // Create a grid to represent the current state
    char grid[MaxWidth][MaxWidth];
    
    // Initialize grid with empty spaces
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (wall[i][j]) {
                grid[i][j] = '#';
            } else if (food[i][j]) {
                grid[i][j] = '*';
            } else {
                grid[i][j] = '.';
            }
        }
    }
    
    // Add snake body to the grid
    auto snake_body = snake->get_snake();
    int length = snake_body.first;
    std::pair<int, int>* positions = snake_body.second;
    
    // Head is at positions[0]
    grid[positions[0].first][positions[0].second] = '@';
    
    // Body parts (excluding head)
    for (int i = 1; i < length; i++) {
        grid[positions[i].first][positions[i].second] = 'o';
    }
    
    // Print the grid
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << grid[i][j];
        }
        std::cout << std::endl;
    }
    
    // Clean up allocated memory
    delete[] positions;
  }
};

struct Game{
  Map *map;
  Snake *snake;
  int score;
  int round;
  void initialize(){
    map = new Map();
    snake = new Snake();
    map->initialize(snake);
    score = 0;
    round = 0;
  }
  bool step()
  {
    char str[MaxWidth];
    std::cin >> str;
    instruction ins;
    switch(str[0]){
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }
    if(snake->move(map, ins)){
      score++;
    }else{
      return false;
    }
    return true;
  }
  void print(){
    std::cout<< "Round: " << round << std::endl;
    map->print(snake);
  }
  void play()
  {
    while(step()){
      round++;
      print();
    }
    score += snake->get_length() ;
    std::cout << "Game Over" << std::endl;
    std::cout << "Score: " << score << std::endl;
  }
};
#endif