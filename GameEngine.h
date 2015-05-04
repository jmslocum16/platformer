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

struct MouseEvent
{
  int modifierState; // shift, ctrl, alt
  int state; // up or down
  int button;
  int x;
  int y;
};

struct KeyEvent
{
  char key;
  int x;
  int y;
};

typedef std::vector<MouseEvent> MouseInput;
typedef std::vector<KeyEvent> KeyInput;

class GameEngine
{
public:
  void Init();
  void Cleanup();

  void ChangeState(GameState* state);
  void PushState(GameState* state);
  void PopState();
  GameState* getCurrentState();

  void HandleEvents();
  void Update();
  void Draw();

  bool Running() { return m_running; }
  void Quit() { m_running = false; }

  static GameEngine* getSingleton() {
    if (!singleton)
      singleton = new GameEngine();
    return singleton;
  }

  int windowWidth;
  int windowHeight;

  MouseInput mouseInput;
  KeyInput keyInput;
  Image testWellImage;

private:
  // the stack of states
  static GameEngine* singleton;

  std::vector<GameState*> states;
  int windowid;
  bool m_running;
};
