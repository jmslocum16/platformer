#pragma once
class Vector2;

class ConvexShape
{

public:

	ConvexShape()
	{}

	virtual ~ConvexShape()
	{
	}

  virtual void move(Vector2 dir) = 0;
	virtual Vector2 getSupport(const Vector2& direction) const = 0;
};
