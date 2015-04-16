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
		Vector2(4, 0),
		Vector2(4.6, 4.6),
		Vector2(0, 4),
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

	Vector2 v = Vector2(10, 10);

	cout << collides(a, b) << endl;
	cout << collides(a, v, b) << endl;

}