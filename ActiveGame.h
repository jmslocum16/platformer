#pragma once
#include "GameState.h"
#include "Simulator.h"
#include "GameObject.h"

#include <vector>

class ActiveGame : public GameState
{
public:
  ActiveGame(const char* fn) { fname = fn; }
  void Init();
  void Cleanup();

  void Pause();
  void Resume();

  void HandleEvents(GameEngine* game);
  void Update(GameEngine* game);
  void Draw(GameEngine* game);

  void setLevelSimulator(Simulator* s) { sim = s; }
  void setMaxGravityWells(int n) { max_wells = n; }
  void addGravityWell(GravityWell* well);
  vector<GravityWell*> getWells() { return wells; }
  void setPlayer(Player* p) { player = p; }

  void addStatic(GameObject* obj);
  void addDynamic(GameObject* obj);
  void removeStatic(GameObject* obj);
  void removeDynamic(GameObject* obj);

 const char* getFname() {return fname; }
private:
  const char* fname;
  vector<GameObject*> objects;
  vector<GravityWell*> wells;
  Player* player;
  Simulator* sim;
  bool exit_reached;
  bool restart;
  bool paused;
  int max_wells;
  long double lastUpdate; // time in miliseconds of last update (time relative to glutInit call)

  void removeObject(GameObject* obj);
};
