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
string GameEngine::well_file = "images/Well";
string GameEngine::door_file = "images/door";

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

Image GameEngine::gravityWell;
Image GameEngine::exitDoor;

Image GameEngine::levels[LEVELS];
string GameEngine::levelFiles[LEVELS] =
{
  "levels/level1.lev",
  "levels/example.lev",
  "levels/example.lev",
  "levels/example.lev",
  "levels/example.lev",
  "levels/example.lev",
  "levels/example.lev",
  "levels/example.lev",
  "levels/example.lev"
};
Image GameEngine::testImage;

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

  loadImage(&(well_file + e)[0], gravityWell);

  loadImage(&(door_file + e)[0], exitDoor);

  // either render to texture, or do something else here to make level thumbnails
  int tmpw = windowWidth, tmph = windowHeight;
  for (int i = 0; i < LEVELS; i++)
  {
    Resize(tmpw / 3, tmph / 3);
    ActiveGame* level = loadLevel(levelFiles[i].c_str());
    level->Draw(this);
    unsigned char* img = new unsigned char[4*windowWidth*windowHeight]; // RGBA
    glReadBuffer(GL_BACK);
    glReadPixels(0,0,windowWidth,windowHeight,GL_BGRA,GL_UNSIGNED_BYTE,img);
    levels[i].width = windowWidth;
    levels[i].height = windowHeight;
    levels[i].data = img;
    //loadImage("images/test.bmp", levels[i]);
  }
  Resize(tmpw, tmph);
  loadImage("images/test.bmp", testImage);
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

void GameEngine::Resize(int w, int h)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport( 0, 0, w, h );
  //glMatrixMode( GL_PROJECTION );
  //glLoadIdentity();
  gluOrtho2D( -1, 1, -1, 1 );

  //glMatrixMode( GL_MODELVIEW );
  windowWidth = w;
  windowHeight = h;
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA);

  // our GL init
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque

	loadResources();

  // game state init
  MainMenu::getSingleton()->Init();

  // level loading:
  //ActiveGame* level = loadLevel("levels/example.lev");

  //ChangeState((GameState*)level);
  ChangeState(MainMenu::getSingleton());
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
  cout << "reached exit!" << endl;
  /*if (hasNextLevel()) {
    loadNextLevel();
  }*/
  //restartLevel();
  PopState();
}

void GameEngine::restartLevel() {
  cout << "restarting level" << endl;
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
