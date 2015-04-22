#include "LevelLoader.h"
#include <iostream>
#include <fstream>

using namespace std;

Simulator* loadLevel(const char* filename) {
  Simulator* sim = new Simulator();
  
  ifstream file (filename);
  if (file.is_open())
  {
    string line;
    getline(file, line); // level name
    float playerX;
    float playerY;
    file >> playerX;
    file >> playerY;
    Player* p = new Player(playerX, playerY);
    sim->add(p);
    long numWalls = atoi(line.c_str());
    /*while (getline(file, line))
    {
      
    }*/
    file.close();
  }


  return sim;
}
