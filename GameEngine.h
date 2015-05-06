#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>  // GLUT, include glu.h and gl.h
#include <stdlib.h>
#endif
#ifdef __linux__
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#endif
#include <vector>
#include <string>

#include "Util.h"
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

using namespace std;

typedef vector<MouseEvent> MouseInput;
typedef vector<KeyEvent> KeyInput;

class GameEngine
{
public:
  void Init();
  void Cleanup();

  void ChangeState(GameState* state);
  void PushState(GameState* state);
  void PopState();
  GameState* getCurrentState();
  void finishLevel();
  void restartLevel();

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

  // player resources
  static string walk_file;
  static string face_file;
  static string fall_file;
  static string jump_file;
  static int num_walk;
  static Animation walkLeft;
  static Animation walkRight;
  static Image faceLeft;
  static Image faceRight;
  static Image fallLeft;
  static Image fallRight;
  static Image jumpLeft;
  static Image jumpRight;

  // gravity well resources
  static string well_file;
  static Image gravityWell;

private:

  void loadResources();

  // the stack of states
  static GameEngine* singleton;

  std::vector<GameState*> states;
  int windowid;
  bool m_running;
  bool hasNextLevel();
  void loadNextLevel();
};
