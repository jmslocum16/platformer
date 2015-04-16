#include "ConvexHull.cc"
#include "ConvexShape.h"
#include "ConvexPolygon.h"
#include "Circle.h"

#include <vector>

#define EPSILON .0001f


Vector2 support(const ConvexShape& a, const ConvexShape& b, const Vector2& d);
bool containsOrigin(std::vector<Vector2>& simplex, Vector2& d);
bool collides(const ConvexShape& a, const ConvexShape& b);

ConvexShape* sweep(const Circle& c, const Vector2& v)
{
	// TODO(Jaime): Need to create convex class under convex shape
	return new Circle(Vector2(0, 0), 0);
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

	// for (int i = 0; i < hull.size(); ++i)
	// {
	// 	std::cout << hull[i].x() << " " << hull[i].y() << std::endl;
	// }

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
			Vector2 a = simplex[1];
			Vector2 b = simplex[0];

			Vector2 ao = a * -1;
			Vector2 ab = b - a;

			if (ab * ao >= 0)
			{
				// std::cout << "*" << std::endl;
				d = ab.normal(ao);
			}
			else
			{
				// std::cout << "-" << std::endl;
				simplex.erase(simplex.begin());
				d = ao;
			}

			return false;
		}

		case 3:
		{
			Vector2 a = simplex[2];
			Vector2 b = simplex[1];
			Vector2 c = simplex[0];

			Vector2 ao = a * -1;
			Vector2 ab = b - a;
			Vector2 ac = c - a;

			Vector2 normal;

			if (ab * ao >= 0)
			{
				// std::cout << 1;
				normal = ab.normal(ao);

				if (ac * normal >= 0)
				{
					// std::cout << 1;
					normal = ac.normal(ao);

					if (ab * normal >= 0)
					{
						// std::cout << 1 << std::endl;
						return true;
					}
					else
					{
						// std::cout << 2 << std::endl;
						simplex.erase(simplex.begin() + 1);
						d = normal;
					}
				}
				else
				{
					// std::cout << 2 << std::endl;
					simplex.erase(simplex.begin());
					d = normal;
				}
			}
			else
			{
				// std::cout << 2;
				if (ac * ao >= 0)
				{
					// std::cout << 1;
					normal = ac.normal(ao);

					if (ab * normal >= 0)
					{
						// std::cout << 1 << std::endl;
						return true;
					}
					else
					{
						// std::cout << 2 << std::endl;
						simplex.erase(simplex.begin() + 1);
						d = normal;
					}
				}
				else
				{
					// std::cout << 2 << std::endl;
					simplex.erase(simplex.begin() + 1);
					simplex.erase(simplex.begin());
					d = ao;
				}
			}

			return false;
		}

		default:
			assert(false);
	}
}

Vector2 support(const ConvexShape& a, const ConvexShape& b, const Vector2& d)
{
	Vector2 s1 = a.getSupport(d);
	Vector2 s2 = b.getSupport(d);

	return s1 - s2;
}
