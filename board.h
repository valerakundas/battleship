#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>

#include "constants.h"
#include "point.h"

class Board
{
protected:
	//declaration
	TBoard m_board; //first coordinate is vertical, second is horizontal
	std::vector<TShip> m_ships; 
	int m_shipsLeft;

public:
	//creating
	Board();
	Board(const Board &board);
	Board& operator=(const Board &board); 

	//input
	void fillRandom();
	void fillFromBoard(const Board &board);

	//output
	void showInStream(std::ostream &out);
	friend std::ostream &operator << (std::ostream &out, Board &board);

	//shooting
	bool makeShot(Pt pt, bool &isShotHit);

protected:
	//shooting
	void killShip(Pt pt);

	//deleting
	void clean();
 	void clear();
	
	bool addShip(const TShip &ship);
	bool putRandomShip(const int &sz);

	//shooting
	bool isKilled(Pt pt);

public:
	//general
	Pt getRandPoint();
	int getShipsLeft();

public: 
    CellState getCellState(Pt pt);
    Pt getFreeRandPoint();

protected:
	bool isPointInBorders(const Pt &pt) const; 
	bool isShipOnPoint(Pt pt);
	bool isAbleToPutShipOnThisPoint(const Pt &testPoint, const Pt &exceptionPoint = { -1,-1 });
	bool isAbleToPutShip(const TShip &ship);
	bool isAbleToPutShipWithPointsAndDirection(Pt pt, int sz, int shipDirection, TShip &resultingShip);
	void moveCellInDirection(Pt &pt, const int &dir);
	void cleanCell(const Pt &pt);
};

