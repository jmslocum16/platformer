#include "MainMenu.h"
#include "ActiveGame.h"
#include "LevelLoader.h"

// Static variables
MainMenu* MainMenu::singleton = NULL;

void MainMenu::Init()
{

}

void MainMenu::Cleanup()
{

}

void MainMenu::Pause()
{

}

void MainMenu::Resume()
{

}

void MainMenu::HandleEvents(GameEngine* game)
{
    // do not advance these iterators
    MouseInput::iterator mouseEnd = game->mouseInput.end();
    KeyInput::iterator keyEnd = game->keyInput.end();

    for (MouseInput::iterator iter = game->mouseInput.begin(); iter != mouseEnd; ++iter)
    {
        MouseEvent e = *iter;
        float x = (2*(float) e.x / game->windowWidth) - 1.0;
        float y = (2*(float) (game->windowHeight - e.y) / game->windowHeight) - 1.0;
        if (e.state == GLUT_DOWN)
        {
          int x1 = (x + 1) / (2.0/3.0);
          int y1 = 3-(y + 1) / (2.0/3.0);
          ActiveGame* level = loadLevel(GameEngine::levelFiles[3*y1+x1].c_str());
          GameEngine::getSingleton()->PushState((GameState*)level);
        }
    }

    for (KeyInput::iterator iter = game->keyInput.begin(); iter != keyEnd; ++iter)
    {
        KeyEvent e = *iter;
        switch (e.key)
        {
            case 27:
                GameEngine::getSingleton()->Quit();
                break;
        }
    }

    game->mouseInput.erase(game->mouseInput.begin(), mouseEnd);
    game->keyInput.erase(game->keyInput.begin(), keyEnd);
}

void MainMenu::Update(GameEngine* game)
{

}

void MainMenu::Draw(GameEngine* game)
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
  glRasterPos2f(-1 + (2.0/3.0)*j, 1.0/3.0 - 2.0/3.0*i);
  Image im = GameEngine::levels[3*i+j];
	glDrawPixels(im.width, im.height, GL_BGRA, GL_UNSIGNED_BYTE, im.data);
    }
  }
}
