#pragma once

#include <vector>
#include "Vector2.h"
#include "ImageManager.h"

class ConvexShape;

class GameObject
{
protected:
public:
  Vector2 drawPoint;
  ConvexShape* collisionObject;
  Vector2 forces;
  Vector2 velocity;
public:
  GameObject()
  {
    
  }
  virtual Image getImage() = 0;
  virtual void draw() = 0;
  virtual void move(float dt) = 0;
  virtual void applyForces() = 0;
  virtual void collision(Vector2 normal) = 0;
};

class StaticObject : public GameObject
{
protected:
  Image* image;         // or animation?
public:
  virtual Image getImage()
  {
    return *image;
  }
  virtual void draw();
};

// For game objects with animations instead of images
class AnimatedObject : public GameObject
{
protected:
  Animation* anim;
  int frame;
public:
  Image getImage()
  {
    return anim->images[frame++ % anim->numFrames];
  };
  void draw();
};

// Static objects

class Wall : public StaticObject
{
  Vector2* pts;         // for recovering angle in case we can draw image?
public:
  Wall(float x1, float y1, float x2, float y2);
  void draw();
  void move(float dt) {}
  void applyForces() {}
  void collision(Vector2 n) {}
};

class Exit : public StaticObject
{
  void collision(Vector2 n) {}
  void applyForces() {}
};

// Dynamic objects

enum PlayerState
{
  Falling=0,
  Ground,
  Goal
};

class Player : public StaticObject
{
private:
  static char* default_file;

  PlayerState state;
  Vector2 groundSlope;
public:
  Player(float x, float y, float w, float h, char* file = NULL);
  void draw();
  void applyForces();
  void move(float dt);
  void jump();
  void left();
  void right();
  bool changeState(PlayerState s);
  bool checkState(PlayerState s) { return state == s; }
  void collision(Vector2 normal);
};
