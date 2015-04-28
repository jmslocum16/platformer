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

#include <iostream>
using namespace std;

Player::Player(float x, float y, float width, float height, char* file)
{
  drawPoint = Vector2(x,y);
  forces = Vector2(0,0);
  velocity = Vector2(0,0);
  cout << "init: " << drawPoint.x() << " , " << drawPoint.y() << endl;
  image = new Image();
  if (file)
    loadImage(file, *image);
  else
    loadImage(default_file, *image);
  float h = image->height / height*2;
  float w = image->width / width*2;
  Vector2 pts[4] = {Vector2(x,y), Vector2(x,y+h), Vector2(x+w,y+h), Vector2(x+w, y)};
  for (int i = 0; i < 4; i++)
  {
    cout << pts[i].x() << ", " << pts[i].y() << endl;
  }
  collisionObject = new ConvexPolygon(pts, 4);
}

void Player::move(float dt)
{
  drawPoint = drawPoint + velocity * dt;
  collisionObject->move(velocity * dt);
}

void Player::draw()
{
  Image i = getImage();
	glRasterPos2f(drawPoint.x(), drawPoint.y());
	glDrawPixels(i.width, i.height, GL_RGB, GL_UNSIGNED_BYTE, i.data);
  ConvexPolygon* poly = (ConvexPolygon*) collisionObject;
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < poly->n; i++) {
    glVertex2f(poly->pts[i].x(), poly->pts[i].y());
  }
  glEnd();
}

#define E 0.0001

Wall::Wall(float x1, float y1, float x2, float y2)
{
  pts = new Vector2[2];
  pts[0] = Vector2(x1, y1);
  pts[1] = Vector2(x2, y2);

  Vector2 wall[4] = {Vector2(x1, y1-E), Vector2(x1, y1+E), Vector2(x2, y2+E), Vector2(x2, y2-E)};
  collisionObject = new ConvexPolygon(wall, 4);
}

void Wall::draw()
{
  glBegin(GL_LINES);
  glVertex2f(pts[0].x(), pts[1].y());
  glVertex2f(pts[1].x(), pts[1].y());
  glEnd();
}
