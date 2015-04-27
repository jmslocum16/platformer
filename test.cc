#include <iostream>

#include "Vector2Test.cc"
#include "GJK.cc"
#include "ConvexPolygon.h"
#include "ConvexHull.cc"

using namespace std;

#define EPS 0.0001

int main()
{
	testVector2();

	Vector2 pts1[4] =
	{
		Vector2(-1.0, -1.0 - EPS),
		Vector2(-1.0, -1.0 + EPS),
		Vector2(1.0, -1.0 + EPS),
		Vector2(1.0, -1.0 - EPS),
	};
	/*{
		Vector2(0, 0),
		Vector2(0, 4),
		Vector2(4, 4),
		Vector2(4, 0),
	};*/

	Vector2 pts2[4] =
	{
		Vector2(-0.95, -0.95),
		Vector2(-0.95, -0.85),
		Vector2(-0.85, -0.85),
		Vector2(-0.85, -0.95),
	};
	/*{
		Vector2(8, 0),
		Vector2(8, 4),
		Vector2(12, 4),
		Vector2(12, 0),
	};*/

	ConvexPolygon a(pts1, 4);
	//Circle a(Vector2(1, 0), 2);
	ConvexPolygon b(pts2, 4);
	//Circle b(Vector2(0, 5), 1);

	Vector2 v = Vector2(0, -10);
	vector<Vector2> result = vector<Vector2>();
  convexHull(pts1, 4, result);

	cout << collides(a, b).collision << endl;

	CollisionOutput co;
	co = collides(a, v, b);
	cout << co.hitFraction << " " << co.depth << " (" << co.hitNormal.x() << ", " << co.hitNormal.y() << ")" << endl;
}
