#include "Simulator.h"
#include "GJK.cc"
#include "Util.h"

#include <iostream>
#include <limits>
#include <algorithm>

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

    vector<CollisionOutput> collisions;
    for (int j = 0; j < statics.size(); j++)
    {
      GameObject* obj2 = statics[j];
      CollisionOutput coll = collides(*obj1->collisionObject, obj1->velocity*dt, *obj2->collisionObject);

      if (coll.hitFraction >= 0.0)
      {
        if (coll.hitFraction <= EPSILON)
        {
          if (coll.hitNormal * obj1->velocity > 0)
          {
            continue;
          }
        }

        coll.hitObject = obj2;
        collisions.push_back(coll);
      }
    }

    sort(collisions.begin(), collisions.end());

    if (collisions.empty())
    {
      obj1->move(dt);
      Player* p = dynamic_cast<Player*>(obj1);

      // Horrible performance, but should work:
      // If we can collide with something below us within a reasonable distance,
      // in this case EPSILON * 10 distance away, then we're on the ground. Otherwise,
      // we're falling.
      if (p && p->checkState(Ground))
      {
        bool hit = false;
        for (int j = 0; j < statics.size(); j++)
        {
          GameObject* obj2 = statics[j];

          if (obj2->passable())
          {
            continue;
          }

          CollisionOutput coll = collides(*obj1->collisionObject, Vector2(0, -EPSILON * 100), *obj2->collisionObject);

          if (coll.hitFraction > EPSILON)
          {
            hit = true;
            break;
          }
        }

        if (!hit)
        {
          p->changeState(SingleJump);
        }
      }
    }
    else
    {
      int firstIndex = -1;
      std::vector<CollisionOutput> passables;
      std::vector<CollisionOutput> nonPassables;
      for (int i = 0; i < collisions.size(); ++i)
      {
        if (collisions[i].hitObject->passable())
        {
          passables.push_back(collisions[i]);
          obj1->collision(passables);
          passables.clear();
        }
        else
        {
          firstIndex = i;
          for (int j = i; j < collisions.size(); ++j)
          {
            nonPassables.push_back(collisions[i]);
          }
          break;
        }
      }

      if (nonPassables.empty())
      {
        obj1->move(dt);
      }
      else
      {
        obj1->move(dt * collisions[firstIndex].hitFraction - EPSILON);        
        obj1->collision(nonPassables);
      }
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

void Simulator::removeStatic(GameObject* obj) {
  std::vector<GameObject*>::iterator pos = std::find(statics.begin(), statics.end(), obj);
  if (pos != statics.end())
    statics.erase(pos);

  pos = std::find(all.begin(), all.end(), obj);
  if (pos != all.end())
    all.erase(pos);
}

void Simulator::removeDynamic(GameObject* obj) {
  std::vector<GameObject*>::iterator pos = std::find(dynamics.begin(), dynamics.end(), obj);
  if (pos != dynamics.end())
    dynamics.erase(pos);

  pos = std::find(all.begin(), all.end(), obj);
  if (pos != all.end())
    all.erase(pos);
}
