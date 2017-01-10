#pragma once

class Pt
{
public:
	int x, y;
	Pt();
	Pt(int x, int y);
	void operator=(const Pt &pt);
};

bool operator==(Pt a, Pt b);

Pt move(Pt pt, int dx, int dy);
