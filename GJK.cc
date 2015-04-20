#include "ConvexHull.cc"
#include "ConvexShape.h"
#include "ConvexPolygon.h"
#include "Circle.h"
#include "Capsule.h"

#include <vector>
#include <limits>

#define EPSILON .000001f

struct GJKOutput
{
	bool collision;
	std::vector<Vector2> simplex;

	GJKOutput()
	: collision(false)
	{
	}
};

struct MinkowskiEdge
{
	float distance;
	Vector2 normal;
	int index;
};

struct CollisionOutput
{
	float hitFraction;
	float depth;
	Vector2 mtv;
};

Vector2 support(const ConvexShape& a, const ConvexShape& b, const Vector2& d);
bool containsOrigin(std::vector<Vector2>& simplex, Vector2& d);
GJKOutput collides(const ConvexShape& a, const ConvexShape& b);

Vector2 tripleProduct(const Vector2& a, const Vector2& b, const Vector2& c)
{
	double ac = a.x() * c.x() + a.y() * c.y();
	double bc = b.x() * c.x() + b.y() * c.y();
	return Vector2(b.x() * ac - a.x() * bc, b.y() * ac - a.y() * bc);
}

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

MinkowskiEdge findClosestEdge(const std::vector<Vector2>& simplex)
{
	MinkowskiEdge closest;

	closest.distance = std::numeric_limits<float>::infinity();
	for (int i = 0; i < simplex.size(); ++i)
	{
		int j = (i + 1) % simplex.size();

		Vector2 a = simplex[i];
		Vector2 b = simplex[j];

		Vector2 e = b - a;
		Vector2 oa = a;
		// Vector2 n = tripleProduct(e, oa, e);
		Vector2 n = e.left();
		n = n.normalize();
		float d = n * a;

		if (d < closest.distance)
		{
			closest.distance = d;
			closest.normal = n;
			closest.index = j;
		}
	}

	return closest;
}

void epa(const ConvexShape& a, const ConvexShape& b, const GJKOutput& gjkOut, CollisionOutput& co)
{
	std::vector<Vector2> simplex = gjkOut.simplex;

	while (true)
	{
		MinkowskiEdge e = findClosestEdge(simplex);

		Vector2 p = support(a, b, e.normal);
		float d = p * e.normal;

		if (d - e.distance < EPSILON)
		{
			co.mtv = e.normal;
			co.depth = d;
			return;
		}
		else
		{
			simplex.insert(simplex.begin() + e.index, p);
		}
	}
}

CollisionOutput collides(const ConvexShape& a, const Vector2& v, const ConvexShape& b)
{
	CollisionOutput co;
	ConvexShape* fullSweep = sweep(a, v);

	if (!collides(*fullSweep, b).collision)
	{
		co.hitFraction = -1.0f;
		return co;
	}

	delete fullSweep;

	float lo = 0.0f, hi = 1.0f, mid;
	float best = 1.0f;

	GJKOutput bestOutput;

	while (lo <= hi && (hi - lo) > EPSILON)
	{
		mid = lo + (hi - lo) / 2;
		ConvexShape* partialSweep = sweep(a, v * mid);

		GJKOutput output = collides(*partialSweep, b);
		if (output.collision)
		{
			best = mid;
			hi = mid;
			bestOutput = output;
		}
		else
		{
			lo = mid;
		}

		delete partialSweep;
	}

	co.hitFraction = best;

	ConvexShape* bestSweep = sweep(a, v * best);
	epa(*bestSweep, b, bestOutput, co);
	delete bestSweep;

	return co;
}


GJKOutput collides(const ConvexShape& a, const ConvexShape& b)
{
	GJKOutput out;
	std::vector<Vector2> simplex;
	Vector2 d(1, 1);

	simplex.push_back(support(a, b, d));
	d = d * -1;

	while (true)
	{
		Vector2 point = support(a, b, d);

		if (point * d < 0)
		{
			return out;
		}

		simplex.push_back(point);

		if (containsOrigin(simplex, d))
		{
			out.collision = true;
			out.simplex = simplex;

			return out;
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