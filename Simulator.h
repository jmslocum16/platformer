#pragma once

#include "ConvexPolygon.h"
#include "Vector2.h"
#include "GameObject.h"

#include <vector>

using namespace std;

typedef vector<GameObject*> Objects;

class Simulator {
public:
  // step physics
  void stepSimulation(float dt);

  void addStatic(GameObject* obj);
  void addDynamic(GameObject* obj);

  void removeStatic(GameObject* obj);
  void removeDynamic(GameObject* obj);

  Objects statics;
  Objects dynamics;
  Objects all;
};
