#pragma once

#include "ConvexPolygon.h"
#include "Vector2.h"
#include "GameObject.h"

#include <vector>

using namespace std;

class Simulator {
public:
  // step physics
  void stepSimulation(float dt);

  void add(GameObject* obj);  
};
