#ifdef __APPLE__
#include <GLUT/glut.h>  // GLUT, include glu.h and gl.h
#include <stdlib.h>
#endif
#ifdef __linux__
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#endif

#include <stdio.h>
 
/* Initialize OpenGL Graphics */
void initGL() {
	// Set "clearing" or background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

// options
int windowWidth = 640;
int windowHeight = 640;


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(-1, -1);
    glVertex2f(0, 0);
    glEnd();

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
	}
}

 
/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc, argv);          // Initialize GLUT

	glutInitWindowSize(windowWidth, windowHeight);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50);
	glutCreateWindow("sadf");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutKeyboardFunc(keyboard);
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the event-processing loop
	return 0;
}
