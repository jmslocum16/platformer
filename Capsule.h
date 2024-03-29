
class Capsule : public ConvexShape
{

private:

	Vector2 centerA, centerB;
	float radius;

public:

	Capsule(const Vector2& center1, const Vector2& center2, float radius)
	: centerA(center1), centerB(center2), radius(radius)
	{
	}

	Vector2 getRange()
	{
	  return Vector2(std::min(centerA.y(), centerB.y()) - radius, std::max(centerA.y(), centerB.y()) + radius);
	}

  void move(Vector2 dir)
  {
    centerA = centerA + dir;
    centerB = centerB + dir;
  }

	Vector2 getSupport(const Vector2& direction) const
	{
		Vector2 normalDir = direction.normalize();
		Vector2 a = centerA + (normalDir * radius);
		Vector2 b = centerB + (normalDir * radius);

		if (a * normalDir > b * normalDir)
		{
			return a;
		}
		else
		{
			return b;
		}
	}
};
