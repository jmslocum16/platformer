class Vector2;

class ConvexShape
{

public:

	ConvexShape()
	{}

	~ConvexShape()
	{
	}

	virtual Vector2 getSupport(const Vector2& direction) const = 0;
};