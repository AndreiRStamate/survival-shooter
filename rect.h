#pragma once

struct rect
{
	float x, y, w, h;

	rect()
	{
		x = y = w = h = 0;
	}

	rect(float x, float y, float w, float h): x(x), y(y), w(w), h(h)
	{
	}

	friend std::ostream& operator<< (std::ostream& o, const rect& r) {
		return o << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
	}
};