#pragma once

#include <vector>
#include "Vector2.h"
#include "ImageManager.h"
#include "ConvexPolygon.h"

class GameObject
{
protected:
  Vector2 drawPoint;
  ConvexPolygon collisionObject;
  Vector2 forces;
  Vector2 velocity;
  Image* image;         // or animation?
public:
  virtual Image getImage() = 0;
  virtual void draw() = 0;
};

class StaticObject : public GameObject
{
protected:
  Image* image;         // or animation?
public:
  virtual Image getImage() {
    return *image;
  }
  virtual void draw();
}

// For game objects with animations instead of images
class AnimatedObject : public GameObject
{
protected:
  Animation* anim;
  int frame;
public
  Image getImage() {
    return anim->images[frame % anim->numFrames];
  }
  public void draw();
};

// Static objects

class Wall : public StaticObject
{
  Vector2* pts;         // for recovering angle in case we can draw image 
public:
  Image getImage() {
    return *image;
  }
};

class Exit : public StaticObject
{
};

// Dynamic objects

class Player : public StaticObject
{
};
