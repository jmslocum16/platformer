#include "Simulator.h"
#include <iostream>

using namespace std;

void Simulator::stepSimulation(float dt)
{
  // empty
}

void Simulator::add(GameObject* obj)
{
  cout << obj->drawPoint.x() << "," << obj->drawPoint.y() << endl;
}
