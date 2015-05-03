#include "Simulator.h"
#include "GJK.cc"
#include <iostream>
#include <limits>
#include "Util.h"

using namespace std;

void Simulator::stepSimulation(float dt)
{
  for (int i = 0; i < dynamics.size(); i++)
  {
    GameObject* obj1 = dynamics[i];
    obj1->applyForces();
    obj1->velocity = obj1->velocity + obj1->forces * dt;

    if (obj1->velocity.length() < EPSILON)
      obj1->velocity = Vector2(0,0);

    obj1->forces = Vector2(0,0);
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
      obj1->move(dt);

      Player* p = dynamic_cast<Player*>(obj1);

      if (p)
      {
        p->changeState(Falling);
      }
    }
    else
    {
      obj1->collision(best.hitNormal);
      obj1->move(dt * best.hitFraction - EPSILON);
    }
  }
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
