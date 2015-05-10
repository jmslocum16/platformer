#pragma once

#include "assert.h"
#include "ConvexShape.h"
#include "Vector2.h"
#include <limits>

class ConvexPolygon : public ConvexShape
{	
public:
	Vector2* pts;
	Vector2 range;
	int n;
  ConvexPolygon()
  {
    pts = NULL;
    n = 0;
  }

	ConvexPolygon(const Vector2* points, int numPoints)
	: n(numPoints)
	{
		assert(numPoints > 0);
		pts = new Vector2[numPoints];
		float min = std::numeric_limits<float>::infinity();
		float max = -std::numeric_limits<float>::infinity();
		for (int i = 0; i < numPoints; ++i)
		{
			pts[i] = points[i];
		  min = std::min(min, pts[i].y());
		  max = std::max(max, pts[i].y());
		}
		range = Vector2(min, max);
	}

	Vector2 getRange()
	{
    return range;
	}

  void move(Vector2 dir)
  {
    for (int i = 0; i < n; i++)
    {
      pts[i] = pts[i] + dir;
    }
  }

	int numPoints() const
	{
		return n;
	}

	Vector2 getPoint(int index) const
	{
		assert(index >= 0 && index < n);
		return pts[index];
	}

	Vector2 getSupport(const Vector2& direction) const
	{
		int bestIndex = 0;
		float maxDot = pts[0] * direction;

		for (int i = 1; i < n; ++i)
		{
			float dotProduct = pts[i] * direction;

			if (dotProduct > maxDot)
			{
				maxDot = dotProduct;
				bestIndex = i;
			}
		}

		return pts[bestIndex];
	}
};
