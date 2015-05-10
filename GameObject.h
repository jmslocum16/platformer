#pragma once

#include <vector>
#include "Vector2.h"
#include "ImageManager.h"
#include "CollisionOutput.h"

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
  GameObject* last;
  GameObject()
  {
    
  }
  virtual Image getImage() = 0;
  virtual void draw() = 0;
  virtual void move(float dt) = 0;
  virtual void applyForces() = 0;
  virtual void collision(std::vector<CollisionOutput> collisions) = 0;
  virtual Vector2* getCenter() = 0;
  virtual bool passable() = 0;
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
  void collision(std::vector<CollisionOutput> collisions) {}
  bool passable() { return false; }
};

class Exit : public StaticObject
{
public:
  Exit(double dx, double dy);
  void move(float dt) {}
  void collision(std::vector<CollisionOutput> collisions) {}
  void applyForces() {}
  void draw();
  bool passable() { return true; }
};

class GravityWell : public StaticObject {
private:
  bool positive;
public:
  static float factor;
  GravityWell(double dx, double dy, bool pos);
  void move(float dt) {}
  void applyForces() {}
  void collision(std::vector<CollisionOutput> collisions) {}
  bool isPositive() { return positive; }
  static void setFactor(double f) { factor = f; }
  void draw();
  bool passable() { return true; }
};

class Switch : public StaticObject {
private:
  bool on;
  bool add;
  Image* imageOff;
  std::vector<GameObject*> toModify;
public:
  Switch(double dx, double dy, bool add);
  void press();
  Image getImage();
  void addObject(GameObject* obj);
  void move(float dt) {}
  void applyForces() {}
  void collision(std::vector<CollisionOutput> collisions) {}
  bool passable() { return true; }
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

  PlayerState state;
  Vector2 prevNormal;

  float lFrame;
  float rFrame;

  float gravity;

public:
  
  Player(float x, float y, float w, float h, float g);
  void draw();
  void applyForces();
  void move(float dt);
  void jump();
  void left();
  void right();
  bool changeState(PlayerState s);
  bool checkState(PlayerState s) { return state == s; }
  void collision(std::vector<CollisionOutput> collisions);
  Image getImage();
  bool passable() { return true; }
};
