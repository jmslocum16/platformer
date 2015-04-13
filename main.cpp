#ifdef __APPLE__
#include <GLUT/glut.h>  // GLUT, include glu.h and gl.h
#include <stdlib.h>
#endif
#ifdef __linux__
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#endif

#include <stdio.h>


#include "imagemanager.h"
 
/* Initialize OpenGL Graphics */
void initGL() {
	// Set "clearing" or background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

// options
int windowWidth = 640;
int windowHeight = 640;

int windowid = 0;

// images
Image testImage;

void loadResources() {
	printf("loading resources\n");
	loadImage("images/test.bmp", testImage);
}

// x and y in window coordinates, lower left corner of image to draw
void drawImage(double x, double y, Image& i) {
	double glX = -1.0 + 2.0*x/windowWidth;
	double glY = -1.0 + 2.0*y/windowWidth;
	glRasterPos2f(glX, glY);
	glDrawPixels(i.width, i.height, GL_RGB, GL_UNSIGNED_BYTE, i.data);
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

	drawImage(320, 320, testImage);

	printf("display\n");
	
	glFlush();  // Render now
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
            printf("w pressed!\n");
			break;
		case 'a':
            printf("a pressed!\n");
			break;
        case 'd':
            printf("d pressed!\n");
            break;
		case 27:
			if (windowid) {
            	glutDestroyWindow(windowid);
            	exit(0);
			}
            break;
	}
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);          // Initialize GLUT

	loadResources();

	glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50);
	windowid = glutCreateWindow("sadf");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutKeyboardFunc(keyboard);
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the event-processing loop
	return 0;
}
