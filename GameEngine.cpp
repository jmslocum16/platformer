#include <cstdio>
#include "GameEngine.h"
#include "LevelLoader.h"

#include "GameState.h"
#include "MainMenu.h"

// Static variables
GameEngine* GameEngine::singleton = NULL;

/*** Temporarily in GameEngine for testing purposes ***/
// testing image functions
void GameEngine::loadResources() {
	printf("loading resources\n");
	loadImage("images/test.bmp", testImage);
}

// x and y in window coordinates, lower left corner of image to draw
void GameEngine::drawImage(double x, double y, Image& i) {
	double glX = -1.0 + 2.0*x/windowWidth;
	double glY = -1.0 + 2.0*y/windowWidth;
	glRasterPos2f(glX, glY);
	glDrawPixels(i.width, i.height, GL_RGB, GL_UNSIGNED_BYTE, i.data);
}
/***********************/

void displayFunc() {
  GameEngine::getSingleton()->Draw();
}

void handleEvents(unsigned char key, int x, int y) {
  GameEngine::getSingleton()->HandleEvents(key, x, y);
}

void GameEngine::Draw() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
	drawImage(320, 320, testImage);

	if (Running()) {
	  // Events 'handled' asynchronously; can be changed
	  Update();
    states.back()->Draw(this);
  }

	glFlush();  // Render now
}

void GameEngine::Init() {
  // GL init
  windowWidth = 640;
  windowHeight = 640;

  windowid = 0;

	loadResources();

	glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50);
	windowid = glutCreateWindow("Platformer");  // Create window with the given title
	glutDisplayFunc(displayFunc);       // Register callback handler for window re-paint event
	glutKeyboardFunc(handleEvents);

  // our GL init
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque

  // game state init
  MainMenu::getSingleton()->Init();

  ChangeState((GameState*)MainMenu::getSingleton());
  // audio init
  // ... other init?
  m_running = true;
  Simulator* s = loadLevel("levels/example.lev");
}

void GameEngine::Cleanup() {
  // game state cleanup
  // audio cleanup
  // ... other cleanup?
}

void GameEngine::ChangeState(GameState* state) {
  if (states.size() > 0)
    PopState();
  PushState(state);
}

void GameEngine::PushState(GameState* state) {
  states.push_back(state);
}

void GameEngine::PopState() {
  states.pop_back();
}

void GameEngine::HandleEvents(unsigned char key, int x, int y) {
  states.back()->HandleEvents(this, key, x, y);
  switch (key) {
      case 'w':
          printf("w pressed!\n");
          break;
      case 'a':
          printf("a pressed!\n");
          break;
      case 'd':
          printf("d pressed!\n");
          break;
      case 27:
          if (windowid) {
              glutDestroyWindow(windowid);
              exit(0);
          }
          break;
  }
}

void GameEngine::Update() {
  states.back()->Update(this);
}
