
class GameObject;

struct CollisionOutput
{
	float hitFraction;
	float depth;
	Vector2 hitNormal;
	GameObject* hitObject;

	bool operator<(const CollisionOutput& other) const
	{
		return hitFraction < other.hitFraction;
	}
};
