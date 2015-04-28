
class Circle : public ConvexShape
{

private:

	Vector2 c;
	float r;

public:

	Circle(const Vector2& center, float radius)
	: c(center), r(radius)
	{
	}

  void move(Vector2 dir)
  {
    c = c + dir;
  }

	float radius() const
	{
		return r;
	}

	Vector2 center() const
	{
		return c;
	}

	Vector2 getSupport(const Vector2& direction) const
	{
		Vector2 normalDir = direction.normalize();
		return c + (normalDir * r);
	}
};
