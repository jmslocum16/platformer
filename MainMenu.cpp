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

void MainMenu::HandleEvents(GameEngine* game, unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            GameEngine::getSingleton()->Quit();
            break;
    }
}

void MainMenu::Update(GameEngine* game)
{

}

void MainMenu::Draw(GameEngine* game)
{

}
