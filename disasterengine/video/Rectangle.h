#pragma once

namespace video
{
	struct Rectangle
	{
	public:
		inline Rectangle() 
		{
			this->x = 0;
			this->y = 0;
			this->w = 0;
			this->h = 0;
		}

		inline Rectangle(double x, double y, double width, double height)
		{
			this->x = x;
			this->y = y;
			this->w = width;
			this->h = height;
		}

		inline bool operator+=(const Rectangle& other)
		{
			x += other.x;
			y += other.y;
			w += other.w;
			h += other.h;
		}

		inline bool operator-=(const Rectangle& other)
		{
			x -= other.x;
			y -= other.y;
			w -= other.w;
			h -= other.h;
		}

		inline bool operator==(const Rectangle& other) const
		{
			return (x == other.x && y == other.y && w == other.w && h == other.h);
		}

		double x = 0, y = 0, w = 0, h = 0;
	};
}