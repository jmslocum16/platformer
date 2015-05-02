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
        cout << "X: " << e.x << " Y: " << e.y << endl;
        float x = (2*(float) e.x / game->windowWidth) - 1.0;
        float y = (2*(float) (game->windowHeight - e.y) / game->windowHeight) - 1.0;
        cout << "Screen x,y: " << x << "," << y << endl;
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
                GameEngine::getSingleton()->Quit();
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
