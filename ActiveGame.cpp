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

void ActiveGame::HandleEvents(GameEngine* game, unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
            player->forces = player->forces + Vector2(0, 5);
            break;
        case 'a':
            player->forces = player->forces + Vector2(-5, 0);
            break;
        case 'd':
            player->forces = player->forces + Vector2(5, 0);
            break;
        case 27:
            GameEngine::getSingleton()->Quit();
            break;
    }
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
