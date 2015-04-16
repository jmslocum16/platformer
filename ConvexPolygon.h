
#include "assert.h"

class ConvexPolygon : public ConvexShape
{

private:
	
	Vector2* pts;
	int n;

public:

	ConvexPolygon(const Vector2* points, int numPoints)
	: n(numPoints)
	{
		assert(numPoints > 0);
		pts = new Vector2[numPoints];

		for (int i = 0; i < numPoints; ++i)
		{
			pts[i] = points[i];
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
		float maxDot = direction * pts[0];

		for (int i = 1; i < n; ++i)
		{
			float dotProduct = direction * pts[i];

			if (dotProduct > maxDot)
			{
				maxDot = dotProduct;
				bestIndex = i;
			}
		}

		return pts[bestIndex];
	}
};
