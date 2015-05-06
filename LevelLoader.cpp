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
    float exitX;
    float exitY;
    int maxWells;
    float factor;
    file >> playerX;
    file >> playerY;
    file >> exitX;
    file >> exitY;
    file >> maxWells;
    file >> factor;
    GravityWell::setFactor(factor);
    Player* p = new Player(playerX, playerY, w, h); // strip newline?

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

    level->setMaxGravityWells(maxWells);
    /*GravityWell* well = new GravityWell(1, -.5, false);
    level->addGravityWell(well);*/
    file.close();
  }


  return level;
}
