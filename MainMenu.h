#pragma once
#include "GameState.h"

class MainMenu : public GameState
{
public:
  void Init();
  void Cleanup();

  void Pause();
  void Resume();

  void HandleEvents(GameEngine* game);
  void Update(GameEngine* game);
  void Draw(GameEngine* game);

  static MainMenu* getSingleton()
  {
    if (!singleton)
      singleton = new MainMenu();
    return singleton;
  }
private:
  static MainMenu* singleton;
};
