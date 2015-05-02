#include "MainMenu.h"

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

}
