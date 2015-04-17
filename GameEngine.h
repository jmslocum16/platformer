#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>  // GLUT, include glu.h and gl.h
#include <stdlib.h>
#endif
#ifdef __linux__
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#endif
#include <vector>

#include "ImageManager.h"

class GameState;

class GameEngine
{
public:
  void Init();
  void Cleanup();

  void ChangeState(GameState* state);
  void PushState(GameState* state);
  void PopState();

  void HandleEvents(unsigned char key, int x, int y);
  void Update();
  void Draw();

  bool Running() { return m_running; }
  void Quit() { m_running = false; }

  static GameEngine* getSingleton() {
    if (!singleton)
      singleton = new GameEngine();
    return singleton;
  }

private:
  void drawImage(double x, double y, Image& i);
  void loadResources();

  // the stack of states
  static GameEngine* singleton;

  std::vector<GameState*> states;
  int windowid;
  int windowWidth;
  int windowHeight;

  Image testImage;

  bool m_running;
};
