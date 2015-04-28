#include "Simulator.h"
#include "GJK.cc"
#include <iostream>
#include <limits>

using namespace std;

void Simulator::stepSimulation(float dt)
{
  for (int i = 0; i < dynamics.size(); i++)
  {
    GameObject* obj1 = dynamics[i];
    obj1->velocity = obj1->velocity + obj1->forces * dt;
    obj1->forces = Vector2(0, -0.5); // forces -> gravity
    CollisionOutput best;
    best.hitFraction = numeric_limits<float>::infinity();
    for (int j = 0; j < statics.size(); j++)
    {
      GameObject* obj2 = statics[j];
      CollisionOutput coll = collides(*obj1->collisionObject, obj1->velocity*dt, *obj2->collisionObject);
      if (coll.hitFraction < 0)
        continue;
      if (coll.hitFraction < best.hitFraction)
        best = coll;
    }
    if (best.hitFraction == numeric_limits<float>::infinity())
    {
      //obj1->drawPoint = obj1->drawPoint + obj1->velocity * dt;
      obj1->move(dt);
    }
    else
    {
      obj1->velocity = Vector2(obj1->velocity.x(), -obj1->velocity.y());
      obj1->move(dt * best.hitFraction);
    }
  }
}

void Simulator::add(GameObject* obj)
{
  cout << obj->drawPoint.x() << "," << obj->drawPoint.y() << endl;
}

void Simulator::addStatic(GameObject* obj)
{
  statics.push_back(obj);
  all.push_back(obj);
}

void Simulator::addDynamic(GameObject* obj)
{
  dynamics.push_back(obj);
  all.push_back(obj);
}
