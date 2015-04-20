#include "GameObject.h"

void StaticObject::draw() {
  Image* i = getImage();
	glRasterPos2f(drawPoint.x(), drawPoint.y());
	glDrawPixels(i->width, i->height, GL_RGB, GL_UNSIGNED_BYTE, i->data);
}

void AnimatedObject::draw() {
  Image* i = getImage();
	glRasterPos2f(drawPoint.x(), drawPoint.y());
	glDrawPixels(i->width, i->height, GL_RGB, GL_UNSIGNED_BYTE, i->data);
	frame++;
}
