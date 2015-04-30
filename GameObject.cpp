#include "GameObject.h"
#include "GameEngine.h"
#include "Util.h"
#include "ConvexPolygon.h"

#include <iostream>
using namespace std;

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

Player::Player(float x, float y, float width, float height, char* file)
{
  drawPoint = Vector2(x,y);
  forces = Vector2(0,0);
  velocity = Vector2(0,0);
  image = new Image();
  if (file)
    loadImage(file, *image);
  else
    loadImage(default_file, *image);
  float h = image->height / height*2;
  float w = image->width / width*2;
  Vector2 pts[4] = {Vector2(x,y), Vector2(x,y+h), Vector2(x+w,y+h), Vector2(x+w, y)};
  collisionObject = new ConvexPolygon(pts, 4);
  state = Falling;
}

void Player::applyForces()
{
  switch (state)
  {
    case Falling:
      forces = forces + Vector2(0,-1.5); // apply gravity if falling
      break;
    case Ground:
      forces = forces + velocity * -0.95;
    default:
      break;
  }
}

bool Player::changeState(PlayerState s)
{
  switch (state)
  {
    case Falling:
      if (s == Ground)
      {
        state = s;
        return true;
      }
      break;
    case Ground:
      state = s;
      return true;
      break;
  }
  return false;
}

void print(PlayerState state)
{
  switch(state)
  {
    case Falling: cout << "Falling" << endl; break;
    case Ground: cout << "Ground" << endl; break;
    case Goal: cout << "Goal" << endl; break;
  }
}

void Player::left()
{
  if (state == Ground)
  {
    Vector2 moveDir = groundSlope * (groundSlope * Vector2(-5,0));
    forces = forces + moveDir;
  }
}

void Player::right()
{
  if (state == Ground)
  {
    Vector2 moveDir = groundSlope * (groundSlope * Vector2(5,0));
    forces = forces + moveDir;
  }
}

void Player::jump()
{
  if (state != Falling)
  {
    velocity = Vector2(velocity.x(), 1);
    changeState(Falling);
  }
}

void Player::collision(Vector2 normal)
{
  if (state == Falling)
  {
    Vector2 n = normal.normalize();
    float proj = velocity * n;
    if (proj < 0 && velocity.y() < 0)
    {
      Vector2 collisionLoss = n * (velocity * n);
      velocity = velocity - collisionLoss;
      if (Vector2(1,0) * Vector2(1,1).normalize() > Vector2(1,0) * n)
      {
        if (n.x() != 0)
          groundSlope = Vector2(1.0/normal.x(), -1.0/normal.y()).normalize();
        else
          groundSlope = Vector2(1.0, 0);
        changeState(Ground);
      }
    }
    else
    {
      // ya hit yo head. probs.
      velocity = Vector2(velocity.x(), 0);
    }
  }
  if (state == Ground)
  {
    Vector2 n = normal.normalize();
    if (n.x() != 0)
      groundSlope = Vector2(1.0/normal.x(), -1.0/normal.y()).normalize();
    else
      groundSlope = Vector2(1.0, 0);
    float proj = velocity * n;
    Vector2 collisionLoss = n * (velocity * n);
    velocity = velocity - collisionLoss;
  }
}

void Player::move(float dt)
{
  //print(state);
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

Wall::Wall(float x1, float y1, float x2, float y2)
{
  pts = new Vector2[2];
  pts[0] = Vector2(x1, y1);
  pts[1] = Vector2(x2, y2);

  Vector2 wall[4] = {Vector2(x1, y1-EPSILON), Vector2(x1, y1+EPSILON), Vector2(x2, y2+EPSILON), Vector2(x2, y2-EPSILON)};
  collisionObject = new ConvexPolygon(wall, 4);
}

void Wall::draw()
{
  glColor3f(1.0,0,0);
  glBegin(GL_LINES);
  glVertex2f(pts[0].x(), pts[0].y());
  glVertex2f(pts[1].x(), pts[1].y());
  glEnd();
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_LOOP);
  ConvexPolygon* poly = (ConvexPolygon*) collisionObject;
  for (int i = 0; i < poly->n; i++) {
    glVertex2f(poly->pts[i].x(), poly->pts[i].y());
  }
  glEnd();
}
