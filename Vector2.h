#include <cmath>

class Vector2
{

private:

	float i, j;

public:
	Vector2()
	: i(0), j(0)
	{}

	Vector2(float i, float j)
	: i(i), j(j)
	{}

	Vector2(const Vector2& other)
	: i(other.i), j(other.j)
	{}

	~Vector2()
	{}

	float x() const
	{
		return i;
	}

	float y() const
	{
		return j;
	}

	bool operator==(const Vector2& other) const
	{
		return i == other.i && j == other.j;
	}

	bool operator!=(const Vector2& other) const
	{
		return !(*this == other);
	}

	bool equals(const Vector2& other) const
	{
		return *this == other;
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(i + other.i, j + other.j);
	}

	Vector2 add(const Vector2& other) const
	{
		return *this + other;
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(i - other.i, j - other.j);
	}

	Vector2 sub(const Vector2& other) const
	{
		return *this - other;
	}

	float operator*(const Vector2& other) const
	{
		return i * other.i + j * other.j;
	}

	float dot(const Vector2& other) const
	{
		return *this * other;
	}

	Vector2 operator*(float c) const
	{
		return Vector2(i * c, j * c);
	}

	Vector2 operator/(float c) const
	{
		return Vector2(i / c, j / c);
	}

	Vector2 scale(float c) const
	{
		return *this * c;
	}

	float cross(const Vector2& other) const
	{
		return i * other.j - j * other.i;
	}

	float lengthSq() const
	{
		return i * i + j * j;
	}

	Vector2 normalize() const
	{
		return *this / sqrt(lengthSq());
	}

	Vector2 perp() const
	{
		return Vector2(-j, i);
	}

	Vector2 left() const
	{
		return Vector2(j, -i);
	}

	Vector2 right() const
	{
		return Vector2(-j, i);
	}

	Vector2 normal(const Vector2& other) const
	{
		float ax_by = i * other.j;
		float ay_bx = j * other.i;

		return Vector2(j * (ay_bx - ax_by), i * (ax_by - ay_bx));
	}
};
