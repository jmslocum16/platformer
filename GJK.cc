#include "ConvexHull.cc"
#include "ConvexShape.h"
#include "ConvexPolygon.h"
#include "Circle.h"
#include "Capsule.h"

#include <vector>

#define EPSILON .000001f


Vector2 support(const ConvexShape& a, const ConvexShape& b, const Vector2& d);
bool containsOrigin(std::vector<Vector2>& simplex, Vector2& d);
bool collides(const ConvexShape& a, const ConvexShape& b);

ConvexShape* sweep(const Circle& c, const Vector2& v)
{
	return new Capsule(c.center(), c.center() + v, c.radius());
}

ConvexShape* sweep(const ConvexPolygon& a, const Vector2& v)
{
	std::vector<Vector2> points, hull;

	for (int i = 0; i < a.numPoints(); ++i)
	{
		points.push_back(a.getPoint(i));
		points.push_back(a.getPoint(i) + v);
	}

	convexHull(&points[0], points.size(), hull);

	return new ConvexPolygon(&hull[0], hull.size());
}

ConvexShape* sweep(const ConvexShape& a, const Vector2& v)
{
	const Circle* c = dynamic_cast<const Circle*>(&a);

	if (c == 0)
	{
		const ConvexPolygon* cp = dynamic_cast<const ConvexPolygon*>(&a);

		assert(cp);

		return sweep(*cp, v);
	}

	return sweep(*c, v);
}

float collides(const ConvexShape& a, const Vector2& v, const ConvexShape& b)
{
	ConvexShape* fullSweep = sweep(a, v);

	if (!collides(*fullSweep, b))
	{
		return -1.0;
	}

	delete fullSweep;

	float lo = 0.0f, hi = 1.0f, mid;
	float best = 1.0f;

	while (lo <= hi && (hi - lo) > EPSILON)
	{
		mid = lo + (hi - lo) / 2;
		ConvexShape* partialSweep = sweep(a, v * mid);

		if (collides(*partialSweep, b))
		{
			best = mid;
			hi = mid;
		}
		else
		{
			lo = mid;
		}

		delete partialSweep;
	}

	return best;
}


bool collides(const ConvexShape& a, const ConvexShape& b)
{
	std::vector<Vector2> simplex;
	Vector2 d(1, 1);

	simplex.push_back(support(a, b, d));
	d = d * -1;

	while (true)
	{
		Vector2 point = support(a, b, d);

		if (point * d < 0)
		{
			return false;
		}

		simplex.push_back(point);

		if (containsOrigin(simplex, d))
		{
			return true;
		}
	}
}

bool containsOrigin(std::vector<Vector2>& simplex, Vector2& d)
{
	switch (simplex.size())
	{
		case 2:
		{
			const Vector2& a = simplex[1];
			const Vector2& b = simplex[0];

			const Vector2& ao = a * - 1;
			const Vector2& ab = b - a;

			d = Vector2(-ab.y(), ab.x());

			if (d * ao < 0)
			{
				d = d * -1;
			}

			return false;
		}

		case 3:
		{
			const Vector2& a = simplex[2];
			const Vector2& b = simplex[1];
			const Vector2& c = simplex[0];

			const Vector2& ao = a * -1;
			const Vector2& ab = b - a;
			const Vector2& ac = c - a;

			d = Vector2(-ab.y(), ab.x());
			if (d * c > 0)
			{
				d = d * -1;
			}

			if (d * ao > 0)
			{
				simplex.erase(simplex.begin());
				return false;
			}

			d = Vector2(-ac.y(), ac.x());
			if (d * b > 0)
			{
				d = d * -1;
			}

			if (d * ao > 0)
			{
				simplex.erase(simplex.begin() + 1);
				return false;
			}

			return true;
		}

		default:
			assert(false);
	}
}

Vector2 support(const ConvexShape& a, const ConvexShape& b, const Vector2& d)
{
	Vector2 s1 = a.getSupport(d);
	Vector2 s2 = b.getSupport(d * -1);

	return s1 - s2;
}