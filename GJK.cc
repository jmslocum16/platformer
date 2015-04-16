#include "ConvexShape.h"
#include "ConvexHull.cc"

#include <vector>

#define EPSILON .0001f


Vector2 tripleProduct(const Vector2& a, const Vector2& b, const Vector2& c);
Vector2 support(const ConvexShape& a, const ConvexShape& b, const Vector2& d);
bool containsOrigin(std::vector<Vector2>& simplex, Vector2& d);

bool collides(const ConvexShape& a, const ConvexShape& b)
{
	std::vector<Vector2> simplex;
	Vector2 d(1, 0);

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

ConvexShape sweep(const Circle& c, const Vector2& v)
{
	// TODO(Jaime): Need to create convex class under convex shape
	return Circle(0, 0);
}

ConvexShape sweep(const ConvexPolygon& a, const Vector2& v)
{
	vector<Vector2> points, hull;

	for (int i = 0; i < a.size(); ++i)
	{
		points.push_back(a[i]);
		points.push_back(a[i] + v);
	}

	convexHull(points.begin(), points.size(), hull);
	return ConvexPolygon(points.begin(), points.size());
}

float collides(const ConvexShape& a, const Vector2& v, const ConvexShape& b)
{
	ConvexShape fullSweep = sweep(a, v);

	if (!collides(fullSweep, b))
	{
		return -1.0;
	}

	float lo = 0.0f, hi = 1.0f, mid;
	float best = 1.0f;

	while (lo <= hi && (hi - lo) <= EPSILON)
	{
		mid = lo + (hi - lo) / 2;
		ConvexShape partialSweep = sweep(a, v * mid);

		if (collides(partialSweep, b))
		{
			best = mid;
			hi = mid - 1;
		}
		else
		{
			lo = mid + 1;
		}
	}

	return best;
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

// bool containsOrigin(std::vector<Vector2>& simplex, Vector2& d)
// {
// 	Vector2 a = simplex[simplex.size() - 1];
// 	Vector2 ao = a * -1;

// 	if (simplex.size() == 3)
// 	{
// 		Vector2 b = simplex[1];
// 		Vector2 c = simplex[0];

// 		Vector2 ab = b - a;
// 		Vector2 ac = c - a;

// 		Vector2 abPerp = tripleProduct(ac, ab, ab);
// 		Vector2 acPerp = tripleProduct(ab, ac, ac);

// 		float acLocation = acPerp * ao;
		
// 		if (acLocation >= 0.0f)
// 		{
// 			simplex.erase(simplex.begin() + 1);
// 			d = acPerp;
// 		}
// 		else
// 		{
// 			float abLocation = abPerp.dot(ao);

// 			if (abLocation < 0.0f)
// 			{
// 				return true;
// 			}
// 			else
// 			{
// 				simplex.erase(simplex.begin());
// 				d = abPerp;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		Vector2 b = simplex[0];
// 		Vector2 ab = b - a;
// 		d = tripleProduct(ab, ao, ab);

// 		if (d.lengthSq() <= EPSILON)
// 		{
// 			d = Vector2(ab.y(), -ab.x());
// 		}
// 	}

// 	return false;
// }

Vector2 support(const ConvexShape& a, const ConvexShape& b, const Vector2& d)
{
	Vector2 s1 = a.getSupport(d);
	Vector2 s2 = b.getSupport(d);

	return s1 - s2;
}

Vector2 tripleProduct(const Vector2& a, const Vector2& b, const Vector2& c)
{
	float ac = a.x() * c.x() + a.y() * c.y();
	float bc = b.x() * c.x() + b.y() * c.y();

	return Vector2(b.x() * ac - a.x() * bc, b.y() * ac - a.y() * bc);
}