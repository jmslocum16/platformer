#pragma once

#include "Util.h"

int orientation(const Vector2& p, const Vector2& q, const Vector2& r)
{
	float value = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
	if (abs(value) < 1e-6)
	{
		return 0;
	}
	else if (value > 0)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

void convexHull(const Vector2* points, int n, std::vector<Vector2>& result)
{
	if (n < 3)
	{
		return;
	}

	int best = 0;
	for (int i = 0; i < n; ++i)
	{
		if (points[i].x() < points[best].x() || (points[i].x() == points[best].x() && points[i].y() < points[best].y()))
		{
			best = i;
		}
	}
	int p = best, q;
	do
	{
		q = (p + 1) % n;
		for (int i = 0; i < n; ++i)
		{
		  int v = orientation(points[p], points[i], points[q]);
			if (v == 2)
			{
				q = i;
			}
			else if (v == 0)
			{
			  if ((points[p] - points[i]).length() > (points[p] - points[q]).length())
			  {
				  q = i;
				}
			}
		}
		result.push_back(points[q]);
		p = q;
	}
	while (p != best && n != result.size());
}
