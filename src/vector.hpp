#pragma once

class CVector
{
public:
	// Values
	int x, y;

	// Constructor
	CVector() : x{0}, y{0} {}
	CVector(int x, int y) : x{x}, y{y} {}

	// Plus operator
	CVector operator+= (const CVector& rhs)
	{
		this->x += rhs.x; this->y += rhs.y;
		return *this;
	}
	friend CVector operator+ (CVector lhs, const CVector& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	// Minus operator
	CVector& operator-= (const CVector& rhs)
	{
		this->x -= rhs.x; this->y -= rhs.y;
		return *this;
	}
	friend CVector operator- (CVector lhs, const CVector& rhs)
	{
		lhs -= rhs;
		return lhs;
	}
};
