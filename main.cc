#include <iostream>

#include "Vector2Test.cc"
#include "GJK.cc"
#include "ConvexPolygon.h"
#include "ConvexHull.cc"

using namespace std;

int main()
{
	testVector2();

	Vector2 pts1[4] =
	{
		Vector2(0, 0),
		Vector2(1, 0),
		Vector2(1, 1),
		Vector2(0, 1),
	};

	Vector2 pts2[4] =
	{
		Vector2(5, 5),
		Vector2(15, 5),
		Vector2(15, 15),
		Vector2(5, 15),
	};

	ConvexPolygon a(pts1, 4);
	ConvexPolygon b(pts2, 4);

	cout << collides(a, b) << endl;

	Vector2 points[7] =
	{
		Vector2(0, 3),
		Vector2(2, 2),
		Vector2(1, 1),
		Vector2(2, 1),
		Vector2(3, 0),
		Vector2(0, 0),
		Vector2(3, 3)
	};

    int n = 7;

    vector<Vector2> hull;
    convexHull(pts2, 4, hull);

    for (int i = 0; i < hull.size(); ++i)
    {
    	cout << hull[i].x() << " " << hull[i].y() << endl;
    }
}