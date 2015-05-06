#include "LevelLoader.h"
#include "ImageManager.h"
#include <iostream>
#include <fstream>

using namespace std;


/*
 * File format:
 * Level Name
 * playerx playery
 * exitx exity
 * numwalls
 * for numwalls:
 *   p1x p1y p2x p2y
 */
ActiveGame* loadLevel(const char* filename) {
  ActiveGame* level = new ActiveGame(filename);
  Simulator* sim = new Simulator();
  float h = GameEngine::getSingleton()->windowHeight;
  float w = GameEngine::getSingleton()->windowWidth;
  
  level->Init();
  level->setLevelSimulator(sim);
  
  ifstream file (filename);
  if (file.is_open())
  {
    string line;
    getline(file, line); // level name
    float playerX;
    float playerY;
    file >> playerX;
    file >> playerY;
    Player* p = new Player(playerX, playerY, w, h); // strip newline?

    float exitX;
    float exitY;
    file >> exitX;
    file >> exitY;
    Exit* exit = new Exit(exitX, exitY);
    level->addStatic(exit);

	level->addDynamic(p);
    level->setPlayer(p);



    long numWalls;
    file >> numWalls;
    for (int i = 0; i < numWalls; i++)
    {
      float p1x, p1y, p2x, p2y;
      file >> p1x; file >> p1y; file >> p2x; file >> p2y;
      Wall* w = new Wall(p1x, p1y, p2x, p2y);
      level->addStatic(w);
    }

    // TODO REMOVE HACKY AS HELL
    level->setMaxGravityWells(1);
    GravityWell* well = new GravityWell(-.5, -.5, true);
    level->addGravityWell(well);
    file.close();
  }


  return level;
}
