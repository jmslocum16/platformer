#pragma once
#include "GameState.h"
#include "Simulator.h"
#include "GameObject.h"

#include <vector>

class ActiveGame : public GameState
{
public:
  void Init();
  void Cleanup();

  void Pause();
  void Resume();

  void HandleEvents(GameEngine* game);
  void Update(GameEngine* game);
  void Draw(GameEngine* game);

  void setLevelSimulator(Simulator* s) { sim = s; }
  void setMaxPortals(int n) { max_portals = n; }
  void setPlayer(Player* p) { player = p; }

  void addStatic(GameObject* obj);
  void addDynamic(GameObject* obj);
private:
  vector<GameObject*> objects;
  Player* player;
  Simulator* sim;
  bool exit_reached;
  bool restart;
  bool paused;
  int max_portals;
  int num_portals;
  long double lastUpdate; // time in miliseconds of last update (time relative to glutInit call)
};
