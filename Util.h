#pragma once

#include "Vector2.h"
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

static string printVector(Vector2 v)
{
  stringstream s;
  s << "x: ";
  s << v.x();
  s << ", y: ";
  s << v.y();
  return s.str();
}

static string itoa(int i)
{
  stringstream s;
  s << i;
  return s.str();
}
