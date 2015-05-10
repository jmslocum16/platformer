#include "ActiveGame.h"

#include <iostream>

void ActiveGame::Init()
{
  objects = vector<GameObject*>();
  lastUpdate = glutGet(GLUT_ELAPSED_TIME);
}

void ActiveGame::Cleanup()
{
  delete sim;
}

void ActiveGame::Pause()
{
  paused = true;
}

void ActiveGame::Resume()
{
  paused = false;
}

void ActiveGame::HandleEvents(GameEngine* game)
{
    // do not advance these iterators
    MouseInput::iterator mouseEnd = game->mouseInput.end();
    KeyInput::iterator keyEnd = game->keyInput.end();

    for (MouseInput::iterator iter = game->mouseInput.begin(); iter != mouseEnd; ++iter)
    {
        MouseEvent e = *iter;
        if (e.state == GLUT_DOWN)
        {
          float x = (2*(float) e.x / game->windowWidth) - 1.0;
          float y = (2*(float) (game->windowHeight - e.y) / game->windowHeight) - 1.0;
          switch (e.button)
          {
            case GLUT_MIDDLE_BUTTON:
              removeWell();
              break;
            case GLUT_LEFT_BUTTON:
            case GLUT_RIGHT_BUTTON:
              GravityWell* well = new GravityWell(x - GameEngine::gravityWell.width / (float)GameEngine::getSingleton()->windowWidth,
                                                  y - GameEngine::gravityWell.height / (float)GameEngine::getSingleton()->windowHeight, e.button == GLUT_LEFT_BUTTON);
              addGravityWell(well);
              break;
          }
        }
    }

    for (KeyInput::iterator iter = game->keyInput.begin(); iter != keyEnd; ++iter)
    {
        KeyEvent e = *iter;
        switch (e.key)
        {
            case 'w':
                player->jump();
                break;
            case 'a':
                player->left();
                break;
            case 'd':
                player->right();
                break;
            case 27:
                GameEngine::getSingleton()->PopState();
                break;
            case 'r':
                GameEngine::getSingleton()->restartLevel();
                break;
        }
    }

    game->mouseInput.erase(game->mouseInput.begin(), mouseEnd);
    game->keyInput.erase(game->keyInput.begin(), keyEnd);
}

void ActiveGame::Update(GameEngine* game)
{
  // update simulation
  static long count = 59;
  float dt = (glutGet(GLUT_ELAPSED_TIME) - lastUpdate) / 1000.0;
  if (++count > 60)
  {
    count = 0;
  }
  sim->stepSimulation(dt);
  lastUpdate += dt * 1000;
}

void ActiveGame::Draw(GameEngine* game)
{
  glRasterPos2f(-1 + EPSILON, -1 + EPSILON);
  Image i = GameEngine::background;
	glDrawPixels(i.width, i.height, GL_BGRA, GL_UNSIGNED_BYTE, i.data);
  for (int i = 0; i < objects.size(); i++)
  {
    objects[i]->draw();
  }
}

void ActiveGame::addStatic(GameObject* obj)
{
  objects.push_back(obj);
  sim->addStatic(obj);
}

void ActiveGame::addDynamic(GameObject* obj)
{
  objects.push_back(obj);
  sim->addDynamic(obj);
}

void ActiveGame::removeObject(GameObject* obj) {
  vector<GameObject*>::iterator position = find(objects.begin(), objects.end(), obj);
  if (position != objects.end())
    objects.erase(position);
}

void ActiveGame::removeStatic(GameObject* obj) {
  removeObject(obj);
  sim->removeStatic(obj);
}

void ActiveGame::removeDynamic(GameObject* obj) {
  removeObject(obj);
  sim->removeDynamic(obj);
}

void ActiveGame::removeWell()
{
  vector<GravityWell*>::iterator first = wells.begin();
  GravityWell* well = *first;
  wells.erase(first);
  removeObject(well);
	delete well;
}

void ActiveGame::addGravityWell(GravityWell* well) {
  if (wells.size() == max_wells)
  {
    removeWell();
  }
  wells.push_back(well);
  objects.push_back(well);
}

