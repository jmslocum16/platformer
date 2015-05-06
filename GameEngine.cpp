#include <cstdio>
#include "GameEngine.h"
#include "LevelLoader.h"

#include "GameState.h"
#include "MainMenu.h"

// Static variables
GameEngine* GameEngine::singleton = NULL;

string GameEngine::walk_file = "images/Walk";
string GameEngine::face_file = "images/Face";
string GameEngine::fall_file = "images/Fall";
string GameEngine::jump_file = "images/Jump";
int GameEngine::num_walk = 8; // 8 frames

string l = "Left";
string r = "Right";
string e = ".bmp";

Animation GameEngine::walkLeft;
Animation GameEngine::walkRight;
Image GameEngine::faceLeft;
Image GameEngine::faceRight;
Image GameEngine::fallLeft;
Image GameEngine::fallRight;
Image GameEngine::jumpLeft;
Image GameEngine::jumpRight;

void GameEngine::loadResources()
{
  loadImage(&(face_file + l + e)[0], faceLeft);
  loadImage(&(face_file + r + e)[0], faceRight);
  loadImage(&(fall_file + l + e)[0], fallLeft);
  loadImage(&(fall_file + r + e)[0], fallRight);
  loadImage(&(jump_file + l + e)[0], jumpLeft);
  loadImage(&(jump_file + r + e)[0], jumpRight);
  char* walkLeftImages[8] =
  {
    &(walk_file + l + itoa(0) + e)[0],
    &(walk_file + l + itoa(1) + e)[0],
    &(walk_file + l + itoa(2) + e)[0],
    &(walk_file + l + itoa(3) + e)[0],
    &(walk_file + l + itoa(4) + e)[0],
    &(walk_file + l + itoa(5) + e)[0],
    &(walk_file + l + itoa(6) + e)[0],
    &(walk_file + l + itoa(7) + e)[0]
  };
  char* walkRightImages[8] =
  {
    &(walk_file + r + itoa(0) + e)[0],
    &(walk_file + r + itoa(1) + e)[0],
    &(walk_file + r + itoa(2) + e)[0],
    &(walk_file + r + itoa(3) + e)[0],
    &(walk_file + r + itoa(4) + e)[0],
    &(walk_file + r + itoa(5) + e)[0],
    &(walk_file + r + itoa(6) + e)[0],
    &(walk_file + r + itoa(7) + e)[0]
  };
  loadAnimation(num_walk, walkLeftImages, walkLeft);
  loadAnimation(num_walk, walkRightImages, walkRight);
}

void mouseFunc(int button, int state, int x, int y)
{
  MouseEvent e;
  e.button = button;
  e.state = state;
  e.x = x;
  e.y = y;
  e.modifierState = glutGetModifiers();
  GameEngine::getSingleton()->mouseInput.push_back(e);
}

void displayFunc()
{
  GameEngine::getSingleton()->Draw();
}

void keyboardFunc(unsigned char key, int x, int y)
{
  KeyEvent e;
  e.key = key;
  e.x = x;
  e.y = y;
  GameEngine::getSingleton()->keyInput.push_back(e);
}

void GameEngine::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

	if (Running())
  {
	  // Events 'handled' asynchronously; can be changed
	  Update();
    states.back()->Draw(this);
  }
  else
  {
    glutDestroyWindow(windowid);
    exit(0);
  }

	glFlush();  // Render now
}

void GameEngine::Init()
{
  // GL init
  windowWidth = 640;
  windowHeight = 640;

  windowid = 0;

	glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50);
	windowid = glutCreateWindow("Platformer");  // Create window with the given title
	glutDisplayFunc(displayFunc);       // Register callback handler for window re-paint event
	glutIdleFunc(displayFunc);          // Register callback handler for window idle
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);

  // our GL init
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque

	loadResources();

  // game state init
  MainMenu::getSingleton()->Init();

  // level loading:
  ActiveGame* level = loadLevel("levels/example.lev");

  ChangeState((GameState*)level);
  // audio init
  // ... other init?
  m_running = true;
}

void GameEngine::Cleanup()
{
  // game state cleanup
  MainMenu::getSingleton()->Cleanup();
  // audio cleanup
  // ... other cleanup?
}

void GameEngine::ChangeState(GameState* state)
{
  if (states.size() > 0)
    PopState();
  PushState(state);
}

void GameEngine::PushState(GameState* state)
{
  states.push_back(state);
}

void GameEngine::PopState()
{
  states.pop_back();
}

GameState* GameEngine::getCurrentState() {
  if (states.size() == 0)
    return NULL;
  return states.back();
}

void GameEngine::finishLevel() {
  if (hasNextLevel()) {
    loadNextLevel();
  }
}

void GameEngine::restartLevel() {
  ActiveGame* oldGame = dynamic_cast<ActiveGame*>(getCurrentState());
  const char* fname = oldGame->getFname();
  oldGame->Cleanup();
  ActiveGame* newGame = loadLevel(fname);
  ChangeState(newGame);
  delete oldGame;
  
}

void GameEngine::HandleEvents()
{
  states.back()->HandleEvents(this);
}

void GameEngine::Update()
{
  HandleEvents();
  states.back()->Update(this);
}

bool GameEngine::hasNextLevel() {
  // TODO
  return false;
}

void GameEngine::loadNextLevel() {
  // TODO
}
