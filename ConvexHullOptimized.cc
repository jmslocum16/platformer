#pragma once

#include "Util.h"

#include <iostream>
#include <stack>
#include <algorithm>

Vector2 anchor;

int orientation(const Vector2& p, const Vector2& q, const Vector2& r)
{
	float v = ((q.y() - p.y()) * (r.x() - q.x())) - ((q.x() - p.x()) * (r.y() - q.y()));

	if (v == 0)
	{
		return 0;
	}
	else if (v > 0)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

bool compare(const Vector2& a, const Vector2& b)
{
	int o = orientation(anchor, a, b);

	if (o == 0)
	{
		return (anchor - a).lengthSq() <= (anchor - b).lengthSq();
	}

	return o == 2;
}

void convexHull(const Vector2* pts, int n, std::vector<Vector2>& hull)
{
	if (n < 3)
	{
		return;
	}

	std::vector<Vector2> points;

	int index = 0;
	anchor = pts[0];
	for (int i = 1; i < n; ++i)
	{
		if (pts[i] < anchor)
		{
			anchor = pts[i];
			index = i;
		}
	}

	for (int i = 0; i < n; ++i)
	{
		if (i != index)
		{
			points.push_back(pts[i]);
		}
	}

	std::sort(points.begin(), points.end(), compare);

	hull.push_back(pts[index]);
	hull.push_back(points[0]);
	hull.push_back(points[1]);

	for (int i = 2; i < n - 1; ++i)
	{
		while (orientation(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) != 2)
		{
			hull.erase(hull.begin() + (hull.size() - 1));
		}

		hull.push_back(points[i]);
	}
}