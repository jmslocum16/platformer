#include <cstdio>
#include "GameEngine.h"
#include "LevelLoader.h"

#include "GameState.h"
#include "MainMenu.h"

// Static variables
GameEngine* GameEngine::singleton = NULL;

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
  changeState(newGame);
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
