#include <iostream>

#include "Vector2Test.cc"
#include "GJK.cc"
#include "ConvexPolygon.h"

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
}