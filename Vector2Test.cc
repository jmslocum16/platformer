#include "Vector2.h"

#include "assert.h"

void testEquals();
void testAdd();
void testSub();
void testDot();
void testScale();
void testLength();

void testVector2()
{
	testEquals();
	testAdd();
	testSub();
	testDot();
	testScale();
	testLength();
}

void testEquals()
{
	Vector2 a(1, 3);
	Vector2 b(1, 3);

	assert(a == b);
	assert(!(a != b));
}

void testAdd()
{
	Vector2 a(3, 2);
	Vector2 b(4, 100);

	Vector2 c = a + b;
	Vector2 d = a.add(b);

	assert(c == d);
	assert(c.x() == 7 && c.y() == 102);
}

void testSub()
{
	Vector2 a(10, 0);
	Vector2 b(4, 1);

	Vector2 c = a - b;
	Vector2 d = a.sub(b);

	assert(c == d);
	assert(c.x() == 6 && c.y() == -1);
}

void testDot()
{
	Vector2 a(3, 4);
	Vector2 b(5, 6);

	float c = a * b;
	float d = a.dot(b);

	assert(c == d);
	assert(c == 39);
}

void testScale()
{
	Vector2 a(3, 7);
	float b = 4;

	Vector2 c = a * b;
	Vector2 d = a.scale(b);

	assert(c == d);
	assert(c.x() == 12 && c.y() == 28);
}

void testLength()
{
	Vector2 a(3, 4);

	assert(a.lengthSq() == 25);
}