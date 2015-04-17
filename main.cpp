#include <stdio.h>

#include "GameEngine.h"

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);          // Initialize GLUT

	GameEngine* game = GameEngine::getSingleton();
	game->Init();

	glutMainLoop();                 // Enter the event-processing loop
	return 0;
}
