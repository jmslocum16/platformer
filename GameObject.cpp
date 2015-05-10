#include "GameObject.h"
#include "GameEngine.h"
#include "Util.h"
#include "ConvexPolygon.h"
#include "ActiveGame.h"

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

#define MS_PER_FRAME 30
#define RUN_SPEED 5.0f
#define JUMP_SPEED 2.0f

float GravityWell::factor = 1.0f;

void StaticObject::draw()
{
  Image i = getImage();
	glRasterPos2f(drawPoint.x(), drawPoint.y());
	glDrawPixels(i.width, i.height, GL_BGRA, GL_UNSIGNED_BYTE, i.data);
}

Vector2* StaticObject::getCenter() {
  Image i = getImage();
  GameEngine* ge = GameEngine::getSingleton();
  return new Vector2(drawPoint.x() + ((double)i.width)/2/ge->windowWidth, drawPoint.y() + ((double)i.height)/2/ge->windowHeight);
}

void AnimatedObject::draw()
{
  //Image i = getImage();
	//glRasterPos2f(drawPoint.x(), drawPoint.y());
	//glDrawPixels(i.width, i.height, GL_BGR, GL_UNSIGNED_BYTE, i.data);
	frame++;
}

Vector2* AnimatedObject::getCenter() {
  Image i = anim->images[frame % anim->numFrames];
  return new Vector2(drawPoint.x() + i.width/2, drawPoint.y() + i.height/2);
}

Player::Player(float x, float y, float width, float height, float g)
{
  drawPoint = Vector2(x,y);
  gravity = g;
  forces = Vector2(0,0);
  velocity = Vector2(0,0);
  float h = GameEngine::getSingleton()->faceLeft.height / height*2;
  float w = GameEngine::getSingleton()->faceLeft.width / width*2;
  Image i = GameEngine::faceRight;
  float iw = i.width / width;
  float ih = i.height / height;
  float x0 = x + iw;
  float hexW = iw / 4.0;
  float hexH = 2*ih/1.1;
  Vector2 pts[4] = {Vector2(x+7.0/width,y), Vector2(x+7.0/width,y+h), Vector2(x+w-7.0/width,y+h), Vector2(x+w-7.0/width, y)};
  //Vector2 pts[6] = {Vector2(x0,y), Vector2(x0+hexW,y+hexH/3.0), Vector2(x0+hexW,y+2*hexH/3.0), Vector2(x0, y+hexH), Vector2(x0-hexW-0.1*iw, y+2*hexH/3.0), Vector2(x0-hexW, y+hexH/3.0)};
  collisionObject = new ConvexPolygon(pts, 4);
  state = SingleJump;
  lFrame = 0;
  rFrame = 0;
}

void Player::applyForces()
{
  switch (state)
  {
    case SingleJump:
    case DoubleJump: {
      forces = forces + Vector2(0,- gravity); // apply gravity if falling
      Vector2* center = getCenter();

      GameEngine* ge = GameEngine::getSingleton();
      ActiveGame* state = (ActiveGame*)(ge->getCurrentState());
      vector<GravityWell*> wells = state->getWells();
      for(vector<GravityWell*>::iterator it = wells.begin(); it != wells.end(); it++) {

        GravityWell* well = *it;
        Vector2* wellCenter = well->getCenter();
        Vector2 dif = *center - *wellCenter;
        if (well->isPositive()) {
          dif = Vector2(-dif.x(), -dif.y());
        }

        double len = dif.length();
        Vector2 result = dif.normalize();
        if (dif.length() < .1) {
          len = .1;
        }
        result = (result / (1 * len)) * GravityWell::factor;
        forces = forces + result;
        delete wellCenter;
      }
      delete center;
      break; }
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
    case SingleJump:
      if (s == Ground || s == DoubleJump)
      {
        state = s;
        return true;
      }
      break;
    case DoubleJump:
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
    case SingleJump: cout << "SingleJump" << endl; break;
    case DoubleJump: cout << "DoubleJump" << endl; break;
    case Ground: cout << "Ground" << endl; break;
    case Goal: cout << "Goal" << endl; break;
  }
}

void Player::left()
{
  // Kind of hacky to check state, but it should work
  if (checkState(Ground))
  {
    Vector2 groundSlope = Vector2(prevNormal.y(), -prevNormal.x());
    Vector2 moveDir = groundSlope * (groundSlope * Vector2(-RUN_SPEED, 0));
    forces = forces + moveDir; 
  }
  else
  {
    forces = forces + Vector2(-RUN_SPEED, 0);
  }
}

void Player::right()
{
  // Kind of hacky to check state, but it should work
  if (checkState(Ground))
  {
    Vector2 groundSlope = Vector2(prevNormal.y(), -prevNormal.x());
    Vector2 moveDir = groundSlope * (groundSlope * Vector2(RUN_SPEED, 0));
    forces = forces + moveDir; 
  }
  else
  {
    forces = forces + Vector2(RUN_SPEED, 0);
  }
}

void Player::jump()
{
  if ((state == Ground && changeState(SingleJump)) || (state == SingleJump && changeState(DoubleJump)))
  {
    velocity = Vector2(velocity.x(), max(velocity.y(), (float) JUMP_SPEED));
  }
}

void Player::collision(vector<CollisionOutput> collisions)
{

  /*
   * Thoughts on collision detection for the player:
   * 
   * ~ We're going to have to consider mulitple cases for 0 hit fractions.
   * - Ground State
   * -- We're continually colliding with at least one wall. This means that we have at least
   * one collision result with a hit fraction of 0.
   * --- Should we gather all of the hit fractions of 0 and interpolate the normal?
   * --- What if another hit fraction exists that we will begin to collide with? We're not
   * going to hit it until after a hit fraction of 0, so we technically need to consider
   * more than a single collision result.
   * ---- Should we return multiple collisions hit sorted by their hit fraction? Probably
   * --- We should ignore hit fractions of 0 whose normal is in the same direction of our movement.
   * - Jump State
   * -- Moving up
   * --- hitting walls on side, hitting head on wall, having something push you up
   * -- Moving down
   * --- hitting walls on side, hitting ground (transition), having something push you down
   */

  if (collisions.empty())
  {
    return;
  }

  CollisionOutput closest = collisions[0];
  Vector2 normal = closest.hitNormal;
  GameObject* other = closest.hitObject;

  if (dynamic_cast<Wall*>(other)) {
    Vector2 up(0, 1);
    float angleFromUp = acos(up * normal) * 180.0f / M_PI;

    if (state == SingleJump || state == DoubleJump)
    {
      Vector2 n = normal.normalize();
      float proj = velocity * n;

      Vector2 collisionLoss = n * proj;
      velocity = velocity - collisionLoss;
      prevNormal = n;

      if (abs(angleFromUp) <= 45.0)
      {
        changeState(Ground);
      }
    }
    else if (state == Ground)
    {
      Vector2 n = normal.normalize();
      if (abs(angleFromUp) <= 45.0)
      {
        prevNormal = n;
      }
      else
      {
        changeState(SingleJump);
      }
      Vector2 collisionLoss = n * (velocity * n);
      velocity = velocity - collisionLoss;
    }
  } else if (dynamic_cast<Exit*>(other)) {
    GameEngine::getSingleton()->finishLevel();
  }
}

void Player::move(float dt)
{
  drawPoint = drawPoint + velocity * dt;
  collisionObject->move(velocity * dt);
}

Image Player::getImage() {
  static int count = 0;
  GameEngine* game = GameEngine::getSingleton();
  count++;
  Image i;
  switch (state)
  {
    case SingleJump:
      i = (velocity.x() < 0 ? game->jumpLeft : game->jumpRight);
      break;
    case DoubleJump:
      i = (velocity.x() < 0 ? game->fallLeft : game->fallRight);
      break;
    case Ground:
    {
      if (velocity.x() < 0)
      {
        if (count % 4 == 0)
        lFrame -= velocity.x();
        i = game->walkLeft.images[((int) round(lFrame)) % game->num_walk];
      }
      else
      {
        if (count % 4 == 0)
        rFrame += velocity.x();
        i = game->walkRight.images[((int) round(rFrame)) % game->num_walk];
      }
      break;
    }
    default:
      i = (velocity.x() < 0 ? game->faceLeft : game->faceRight);
      break;
  }
  return i;
}

void Player::draw()
{
  Image i = getImage();
  float posX = drawPoint.x() <= -1 ? -1 + EPSILON : drawPoint.x();
  float posY = drawPoint.y() <= -1 ? -1 + EPSILON : drawPoint.y();
	glRasterPos2f(posX, posY);
	float moveX = posX - drawPoint.x();
	float moveY = posY - drawPoint.y();
	glBitmap(0,0,0,0,-moveX*GameEngine::getSingleton()->windowWidth/2,-moveY*GameEngine::getSingleton()->windowHeight/2,NULL);
	glDrawPixels(i.width, i.height, GL_BGRA, GL_UNSIGNED_BYTE, i.data);
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

Exit::Exit(double dx, double dy) {
  drawPoint = Vector2(dx, dy);
  image = &GameEngine::getSingleton()->exitDoor;
  float h = 2.0*image->height / GameEngine::getSingleton()->windowWidth;
  float w = 2.0*image->width / GameEngine::getSingleton()->windowHeight;
  Vector2 pts[4] = {Vector2(dx,dy), Vector2(dx+w,dy), Vector2(dx+w,dy+h), Vector2(dx, dy+h)};
  collisionObject = new ConvexPolygon(pts, 4); 
}

void Exit::draw() { 
  Image i = getImage();
  float h = 2*i.height / GameEngine::getSingleton()->windowWidth;
  float w = 2*i.width / GameEngine::getSingleton()->windowHeight;
 
  glRasterPos2f(drawPoint.x(), drawPoint.y()); 
  glDrawPixels(i.width, i.height, GL_BGRA, GL_UNSIGNED_BYTE, i.data);

  glBegin(GL_LINE_LOOP);
  glVertex2f(drawPoint.x(), drawPoint.y());
  glVertex2f(drawPoint.x() + w, drawPoint.y());
  glVertex2f(drawPoint.x() + w, drawPoint.y() + h);
  glVertex2f(drawPoint.x(), drawPoint.y() + h);
  glEnd();
}

GravityWell::GravityWell(double dx, double dy, bool pos) {
  drawPoint = Vector2(dx, dy);
  positive = pos;
  image = &GameEngine::getSingleton()->gravityWell;
}

void GravityWell::draw()
{
  Image i = getImage();
  //glRasterPos2f(drawPoint.x(), drawPoint.y());
  float posX = drawPoint.x() <= -1 ? -1 + EPSILON : drawPoint.x();
  float posY = drawPoint.y() <= -1 ? -1 + EPSILON : drawPoint.y();
	glRasterPos2f(posX, posY);
	float moveX = posX - drawPoint.x();
	float moveY = posY - drawPoint.y();
	glBitmap(0,0,0,0,-moveX*GameEngine::getSingleton()->windowWidth/2,-moveY*GameEngine::getSingleton()->windowHeight/2,NULL);
  if (positive)
  {
    glDrawPixels(i.width, i.height, GL_BGRA, GL_UNSIGNED_BYTE, i.data);
  }
  else
  {
    glDrawPixels(i.width, i.height, GL_RGBA, GL_UNSIGNED_BYTE, i.data);
  }
  Vector2* center = getCenter();
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POINTS);
    glVertex2f(center->x(), center->y());
  glEnd();
  delete center;
}
