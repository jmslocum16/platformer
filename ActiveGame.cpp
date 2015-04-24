#include "ActiveGame.h"

void ActiveGame::Init()
{
  objects = vector<GameObject*>();
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
