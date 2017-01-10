#include "point.h"

Pt::Pt()
{
	x = 0;
	y = 0;
}

Pt::Pt(int x, int y)
{
	this->x = x;
	this->y = y;
}

bool operator==(Pt a, Pt b)
{
	return a.x == b.x && a.y == b.y;
}

Pt move(Pt pt, int dx, int dy)
{
	int newX = pt.x + dx;
	int newY = pt.y + dy;
	return Pt(newX, newY);
}

void Pt::operator=(const Pt &pt)
{
	x = pt.x;
	y = pt.y;
}

