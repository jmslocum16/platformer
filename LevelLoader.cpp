#include "LevelLoader.h"
#include "ImageManager.h"
#include <iostream>
#include <fstream>

using namespace std;

Wall* loadWall(ifstream& file) {
  float p1x, p1y, p2x, p2y;
  bool deadly;
  file >> p1x; file >> p1y; file >> p2x; file >> p2y;
  file >> deadly;
  return new Wall(p1x, p1y, p2x, p2y, deadly);      
}

/*
 * File format:
 * Level Name
 * playerx playery
 * exitx exity
 * maxwells
 * gravitywellfactor
 * earthgravityfactor
 * numwalls
 * for numwalls:
 *   p1x p1y p2x p2y
 * numswitches
 * sx sy add numaffectedwalls
 * for numaffectedwalls:
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
    float wellFactor;
    float earthFactor;
    file >> playerX;
    file >> playerY;
    file >> exitX;
    file >> exitY;
    file >> maxWells;
    file >> wellFactor;
    file >> earthFactor;
    GravityWell::setFactor(wellFactor);
    
    Player* p = new Player(playerX, playerY, w, h, earthFactor); // strip newline?

    Exit* exit = new Exit(exitX, exitY);
    level->addStatic(exit);

    level->addDynamic(p);
    level->setPlayer(p);



    long numWalls;
    file >> numWalls;
    for (int i = 0; i < numWalls; i++)
    {
      Wall* w = loadWall(file);
      level->addStatic(w);
    }

    long numSwitches;
    file >> numSwitches;
    for (int i = 0; i < numSwitches; i++) {
      float x, y;
      int add;
      int numAffectedWalls;
      file >> x; file >> y; file >> add; file >> numAffectedWalls;
      Switch* s = new Switch(x, y, add);
      for (int j = 0; j < numAffectedWalls; j++) {
        Wall* w = loadWall(file);
        if (!add)
          level->addStatic(w);
        s->addObject(w);
      }
      level->addStatic(s);
    }

    level->setMaxGravityWells(maxWells);
    file.close();
  }


  return level;
}
