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
        case 27:
            GameEngine::getSingleton()->Quit();
            break;
    }
}

void ActiveGame::Update(GameEngine* game)
{
  // update simulation
  static long count = 0;
  float dt = (glutGet(GLUT_ELAPSED_TIME) - lastUpdate) / 1000.0;
  sim->stepSimulation(dt);
  lastUpdate += dt * 1000;
  /*if (++count > 60)
  {
    cout << "dt " << dt << " ELAPSED " << glutGet(GLUT_ELAPSED_TIME) << endl;
    count = 0;
  }*/
}

void ActiveGame::Draw(GameEngine* game)
{
  for (int i = 0; i < objects.size(); i++)
  {
    objects[i]->draw();
  }
}

void ActiveGame::add(GameObject* obj)
{
  objects.push_back(obj);
  // Add object ids to the simulator? or something similar
}
