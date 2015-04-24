#include <cstdio>
#include "GameEngine.h"
#include "LevelLoader.h"

#include "GameState.h"
#include "MainMenu.h"

// Static variables
GameEngine* GameEngine::singleton = NULL;

/*** Temporarily in GameEngine for testing purposes ***/
// testing image functions
void GameEngine::loadResources()
{
	printf("loading resources\n");
	loadImage("images/test.bmp", testImage);
}
/***********************/

void displayFunc()
{
  GameEngine::getSingleton()->Draw();
}

void handleEvents(unsigned char key, int x, int y)
{
  GameEngine::getSingleton()->HandleEvents(key, x, y);
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

	loadResources();

	glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50);
	windowid = glutCreateWindow("Platformer");  // Create window with the given title
	glutDisplayFunc(displayFunc);       // Register callback handler for window re-paint event
	glutIdleFunc(displayFunc);          // Register callback handler for window idle
	glutKeyboardFunc(handleEvents);

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

void GameEngine::HandleEvents(unsigned char key, int x, int y)
{
  states.back()->HandleEvents(this, key, x, y);
  switch (key)
  {
      case 'w':
          printf("w pressed!\n");
          break;
      case 'a':
          printf("a pressed!\n");
          break;
      case 'd':
          printf("d pressed!\n");
          break;
  }
}

void GameEngine::Update()
{
  states.back()->Update(this);
}
