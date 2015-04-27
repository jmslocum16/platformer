#include "LevelLoader.h"
#include "ImageManager.h"
#include <iostream>
#include <fstream>

using namespace std;

ActiveGame* loadLevel(const char* filename) {
  ActiveGame* level = new ActiveGame();
  Simulator* sim = new Simulator();
  level->Init();
  level->setLevelSimulator(sim);
  
  ifstream file (filename);
  if (file.is_open())
  {
    string line;
    getline(file, line); // level name
    getline(file, line); // player image file
    float playerX;
    float playerY;
    file >> playerX;
    file >> playerY;
    Player* p = new Player(playerX, playerY, &line[0]); // strip newline?
    level->add(p);
    long numWalls;
    file >> numWalls;
    for (int i = 0; i < numWalls; i++)
    {
      float p1x, p1y, p2x, p2y;
      file >> p1x; file >> p1y; file >> p2x; file >> p2y;
      Wall* w = new Wall(p1x, p1y, p2x, p2y);
      level->add(w);
    }
    file.close();
  }


  return level;
}
