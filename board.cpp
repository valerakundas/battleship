#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

#include "board.h"
#include "constants.h"
#include "drawing.h"

Board::Board()
{
	m_shipsLeft = 0;
	m_board.resize(BoardSize);
	for (int h = 0; h < BoardSize; ++h)
		m_board[h].resize(BoardSize);
	for (int h = 0; h < BoardSize; ++h)
		for (int w = 0; w < BoardSize; ++w)
			m_board[h][w] = gratis;
}

Board::Board(const Board &board)
{
	m_shipsLeft = board.m_shipsLeft;
	m_board.resize(BoardSize);
	for (int h = 0; h < BoardSize; ++h)
		m_board[h].resize(BoardSize);
	for (int h = 0; h < BoardSize; ++h)
		for (int w = 0; w < BoardSize; ++w)
			m_board[h][w] = board.m_board[h][w];
}

Board& Board::operator=(const Board &board)
{
	m_board.resize(BoardSize);
	for (int h = 0; h < BoardSize; ++h)
		m_board[h].resize(BoardSize);
	for (int h = 0; h < BoardSize; ++h)
		for (int w = 0; w < BoardSize; ++w)
			m_board[h][w] = board.m_board[h][w];
	m_ships = board.m_ships;
	return *this;
}

void Board::clean()
{
	m_shipsLeft = 0;
	m_ships.clear();
	for (int h = 0; h < BoardSize; ++h)
		for (int w = 0; w < BoardSize; ++w)
			m_board[h][w] = gratis;
}

void Board::clear()
{
	m_shipsLeft = 0;
	m_board.clear();
	m_ships.clear();
}

void Board::fillRandom()
{
	clean();
	for (int sz = 4; sz >= 1; --sz)
	{
		int cnt = 5 - sz;
		for (int i = 0; i < cnt; ++i)
			putRandomShip(sz);
	}
}

Pt Board::getRandPoint()
{
	return Pt(rand() % BoardSize, rand() % BoardSize);
}

CellState Board::getCellState(Pt pt)
{
	return m_board[pt.x][pt.y];
}

Pt Board::getFreeRandPoint()
{
	//TODO reimplement
	Pt pt = getRandPoint();
	CellState cs = getCellState(pt);
	while (cs == wounded || cs == killed || cs == notAimed)
	{
		pt = getRandPoint();
		cs = getCellState(pt);
	}
	return pt;
}

int Board::getShipsLeft()
{
	return m_shipsLeft;
}

bool Board::isShipOnPoint(const Pt pt)
{
	CellState &cell = m_board[pt.y][pt.x];
	if (cell == alive || cell == wounded)
		return true;
	return false;
}

bool Board::isPointInBorders(const Pt &a) const
{
	return a.x >= 0 && a.x < BoardSize && a.y >= 0 && a.y < BoardSize;
}

bool Board::addShip(const TShip &ship)
{
	if (isAbleToPutShip(ship))
	{
		m_ships.push_back(ship);
		for (int i = 0; i < (int)ship.size(); ++i)
		{
			Pt cur = ship[i];
			m_board[cur.y][cur.x] = alive;
		}
		m_shipsLeft++;
		return true;
	}
	return false;
}

void Board::fillFromBoard(const Board & board)
{
	for (int h = 0; h < BoardSize; ++h)
		for (int w = 0; w < BoardSize; ++w)
			m_board[h][w] = board.m_board[h][w];
}

void Board::showInStream(std::ostream &out)
{
	for (int i = 0; i < BoardSize; ++i)
	{
		for (int j = 0; j < BoardSize; ++j)
			out << m_board[i][j] << " ";
		out << "\n";
	}
	out << "\n\n";
}

bool Board::putRandomShip(const int &sz)
{
	for (int indx = 0; indx < 100000; ++indx)
	{
		Pt pt = getRandPoint();
		assert(pt.y >= 0 && pt.y < BoardSize);
		assert(pt.x >= 0 && pt.y < BoardSize);
		while (m_board[pt.y][pt.x] != gratis)
			pt = getRandPoint();

		int shipDirectionTemp = rand() % 4;

		for (int shipDirection = shipDirectionTemp; shipDirection < 4; ++shipDirection)
		{
			TShip ship;
			if (!isAbleToPutShipWithPointsAndDirection(pt, sz, shipDirection, ship))
			{
				for (int i = 0; i < (int)ship.size(); ++i)
					std::cout << ship[i].x << ";" << ship[i].y << "\n";
				continue;
			}
			else
			{
				if (!addShip(ship))
					continue;
				return true;
			}
		}
	}
	return false;
}

bool Board::isAbleToPutShip(const TShip &ship)
{
	if (!isAbleToPutShipOnThisPoint(ship[0], { -1,-1 })) //first point checked without previous one
		return false;
	for (int i = 1; i < (int)ship.size(); ++i)
		if (!isAbleToPutShipOnThisPoint(ship[i], ship[i - 1]))
			return false;
	return true;
}

void Board::moveCellInDirection(Pt &pt, const int &dir)
{
	pt.x += DX[dir];
	pt.y += DY[dir];
}

void Board::cleanCell(const Pt &pt)
{
	for (int h = -1; h <= 1; ++h)
		for (int w = -1; w <= 1; ++w)
		{
			Pt curPt(pt.x + w, pt.y + h);
			if (isPointInBorders(curPt) && m_board[curPt.y][curPt.x] == gratis)
				m_board[curPt.y][curPt.x] = notAimed;
		}
}

bool Board::isAbleToPutShipOnThisPoint(const Pt &testPoint, const Pt &exceptionPoint) //exceptionPoint - point where a part of current ship lies
{
	for (int w = -1; w <= 1; ++w)
		for (int h = -1; h <= 1; ++h)
		{
			Pt check_pt(testPoint.x + w, testPoint.y + h); 
			if (!isPointInBorders(check_pt))
				continue;
			if (check_pt == testPoint)
				continue;
			if (check_pt == exceptionPoint)
				continue;
			CellState &cur = m_board[check_pt.y][check_pt.x];
			if (cur == wounded || cur == alive || cur == killed)
				return false;
		}
	return true;
}

bool Board::isAbleToPutShipWithPointsAndDirection(Pt pt, int sz, int shipDirection, TShip &resultingShip)
{
	Pt curPt = pt, prev = { -1,-1 };
	TShip ship;
	ship.push_back(curPt);

	for (int j = 1; j < sz; ++j)
	{
		prev = curPt;
		curPt = move(curPt, DX[shipDirection], DY[shipDirection]);
		if (!isPointInBorders(curPt))
			return false;
		if (isAbleToPutShipOnThisPoint(curPt, prev))
			ship.push_back(curPt);
		else
			return false;
	}
	resultingShip = ship;
	return true;
}

std::ostream& operator << (std::ostream &out, Board &a)
{
	for (int i = 0; i < BoardSize; ++i)
	{
		for (int j = 0; j < BoardSize; ++j)
			out << (char)a.m_board[i][j];
		out << "\n";
	}
	out << "\n\n";
	return out;
}

bool Board::makeShot(Pt pt, bool &isShotHit) //return true if shot was made
{
      if (!isPointInBorders(pt))
            return false;
	CellState &cell = m_board[pt.y][pt.x];
	isShotHit = false;
	switch (cell)
	{
		case vacant:
		case alive:
		{
			cell = wounded;
			if (isKilled(pt))
				killShip(pt);
			isShotHit = true;
			return true;
		}
		case wounded:
		case killed:
		case notAimed:
			return false;
		case gratis:
		{
			cell = notAimed;
			return true;
		}
	}
	assert(0);
	return false;
}

bool Board::isKilled(Pt pt)
{
	//0-down, 1-left, 2-up, 3-right
	int dir = 0;
	for (int i = 0; i < 4; ++i)
	{
		Pt nextPt = move(pt, DX[i], DY[i]);
		while (isPointInBorders(nextPt) && (m_board[nextPt.y][nextPt.x] == alive || m_board[nextPt.y][nextPt.x] == wounded))
		{
			if (m_board[nextPt.y][nextPt.x] == alive)
				return false;
			nextPt = move(nextPt, DX[i], DY[i]);
		}
	}
	return true;
}

void Board::killShip(Pt pt)
{
	//0-down, 1-left, 2-up, 3-right
	int dir = 0;
	m_board[pt.y][pt.x] = killed;
	cleanCell(pt);
	m_shipsLeft--;
	for (int i = 0; i < 4; ++i)
	{
		Pt nextPt = move(pt, DX[i], DY[i]);
		while (isPointInBorders(nextPt) && m_board[nextPt.y][nextPt.x] == wounded)
		{
			m_board[nextPt.y][nextPt.x] = killed;
			cleanCell(nextPt);
			nextPt = move(nextPt, DX[i], DY[i]);
		}
	}
}

