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

	virtual Vector2 getSupport(const Vector2& direction) const = 0;
};
