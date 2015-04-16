
class Circle : public ConvexShape
{

private:

	Vector2 center;
	float radius;

public:

	Circle(const Vector2& center, float radius)
	: center(center), radius(radius)
	{
	}

	Vector2 center() const
	{
		return center;
	}

	Vector2 getSupport(const Vector2& direction) const
	{
		Vector2 normalDir = direction.normalize();
		return center + (normalDir * radius);
	}
};
