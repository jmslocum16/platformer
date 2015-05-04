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
  virtual Vector2* getCenter() = 0;
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
  virtual Vector2* getCenter();
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
  virtual Vector2* getCenter();
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

class GravityWell : public StaticObject {
private:
  bool positive;
public:
  GravityWell(double dx, double dy, bool pos);
  void move(float dt) {}
  void applyForces() {}
  void collision(Vector2 n) {}
  bool isPositive() { return positive; }
  void draw();
};

// Dynamic objects

enum PlayerState
{
  SingleJump=0,
  DoubleJump,
  Ground,
  Goal
};

using namespace std;
class Player : public StaticObject
{
private:
  static string walk_file;
  static string face_file;
  static string fall_file;
  static string jump_file;
  static int num_walk;

  static Animation walkLeft;
  static Animation walkRight;
  static Image faceLeft;
  static Image faceRight;
  static Image fallLeft;
  static Image fallRight;
  static Image jumpLeft;
  static Image jumpRight;

  PlayerState state;
  Vector2 groundSlope;

  float lFrame;
  float rFrame;

public:

  static void loadAnimations();
  
  Player(float x, float y, float w, float h);
  void draw();
  void applyForces();
  void move(float dt);
  void jump();
  void left();
  void right();
  bool changeState(PlayerState s);
  bool checkState(PlayerState s) { return state == s; }
  void collision(Vector2 normal);
  Image getImage();
};
