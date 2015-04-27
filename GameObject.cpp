#include "GameObject.h"
#include "GameEngine.h"

char* Player::default_file = "images/test.bmp";

void StaticObject::draw()
{
  Image i = getImage();
	glRasterPos2f(drawPoint.x(), drawPoint.y());
	glDrawPixels(i.width, i.height, GL_RGB, GL_UNSIGNED_BYTE, i.data);
}

void AnimatedObject::draw()
{
  //Image i = getImage();
	//glRasterPos2f(drawPoint.x(), drawPoint.y());
	//glDrawPixels(i.width, i.height, GL_RGB, GL_UNSIGNED_BYTE, i.data);
	frame++;
}

void Player::draw()
{
  Image i = getImage();
	glRasterPos2f(drawPoint.x(), drawPoint.y());
	glDrawPixels(i.width, i.height, GL_RGB, GL_UNSIGNED_BYTE, i.data);
}

void Wall::draw()
{
  glBegin(GL_LINES);
  glVertex2f(pts[0].x(), pts[1].y());
  glVertex2f(pts[1].x(), pts[1].y());
  glEnd();
}
