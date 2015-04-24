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

  void HandleEvents(GameEngine* game, unsigned char key, int x, int y);
  void Update(GameEngine* game);
  void Draw(GameEngine* game);

  void setLevelSimulator(Simulator* s) { sim = s; }
  void setMaxPortals(int n) { max_portals = n; }
  void add(GameObject* obj);

private:
  vector<GameObject*> objects;
  Simulator* sim;
  bool exit_reached;
  bool restart;
  bool paused;
  int max_portals;
  int num_portals;
};
