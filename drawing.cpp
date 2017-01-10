#include <cassert>

#include "drawing.h"
#include "constants.h"

void drawVacantCell(sf::RenderTarget &target, sf::RectangleShape cell)
{
	cell.setFillColor(ColorVacantCell);
	target.draw(cell);
}

void drawAliveCell(sf::RenderTarget &target, sf::RectangleShape cell)
{
	cell.setFillColor(ColorAliveShipsCell);
	target.draw(cell);
}

void drawWoundedCell(sf::RenderTarget &target, sf::RectangleShape cell)
{
	cell.setFillColor(ColorWoundedShipsCell);
	target.draw(cell);

	sf::VertexArray lines(sf::Lines, 4);
	lines[0].color = lines[1].color = lines[2].color = lines[3].color = ColorWoundedShipsCell;
	sf::Vector2f pos = cell.getPosition();
	sf::Vector2f sz = cell.getSize();
	float x0 = pos.x;
	float y0 = pos.y;
	float width = sz.x;
	float height = sz.y;
	lines[0].position = sf::Vector2f(x0, y0 + height);
	lines[1].position = sf::Vector2f(x0 + width, y0);
	lines[2].position = sf::Vector2f(x0, y0);
	lines[3].position = sf::Vector2f(x0 + width, y0 + height);
	target.draw(lines);
}

void drawKilledCell(sf::RenderTarget &target, sf::RectangleShape cell)
{
	cell.setFillColor(ColorKilledShipsCell);
	target.draw(cell);
}

void drawGratisCell(sf::RenderTarget &target, sf::RectangleShape cell)
{
	cell.setFillColor(ColorGratisCell);
	target.draw(cell);
}

void drawNotAimedCell(sf::RenderTarget &target, sf::RectangleShape cell)
{
	cell.setFillColor(ColorGratisCell);
	target.draw(cell);

	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Black);
	circle.setPosition(cell.getPosition().x + cell.getSize().x / 2, cell.getPosition().y + cell.getSize().y / 2);
	circle.setRadius(2);
	target.draw(circle);
}

void drawBackGround(sf::RenderWindow &window)
{
	sf::RectangleShape sprite;
	sprite.setFillColor(ColorBackGround); //NOTE if i type number it works, if i type const, it does not
	sprite.setSize(sf::Vector2f((float)WindowWidth, (float)WindowHeight));
	sprite.setPosition(sf::Vector2f(0, 0));
	window.draw(sprite);
}

DrawingBoard::DrawingBoard(Board &board, const bool &isOpen, const sf::Vector2f &position, const sf::Vector2u &size)
	: Board(board), m_isOpen(isOpen), m_position(position), m_size(size)
{}

DrawingBoard::DrawingBoard() : m_isOpen(true), m_position(0, 0), m_size(WindowWidth, WindowHeight) {}

void DrawingBoard::draw(sf::RenderTarget & window, sf::RenderStates states) const
{
	float cellWidth = (float)m_size.x / BoardSize;
	float cellHeight = (float)m_size.y / BoardSize;

	for (int h = 0; h < BoardSize; ++h)
		for (int w = 0; w < BoardSize; ++w)
		{
			sf::Vector2f pos(m_position.x + cellWidth * w, m_position.y + cellHeight * h);
			sf::RectangleShape cell;
			cell.setPosition(pos);
			cell.setSize(sf::Vector2f(cellWidth, cellHeight));

			CellState cellState = m_board[h][w];

			if (m_isOpen)
			{
				OpenCell openCell(cell, cellState);
				window.draw(openCell);
			}
			else
			{
				ClosedCell closedCell(cell, cellState);
				window.draw(closedCell);
			}
		}

	for (int i = 0; i <= BoardSize; ++i)
	{
		sf::VertexArray lines(sf::Lines, 4);
		lines[0].position = sf::Vector2f(m_position.x + i * cellWidth, m_position.y);
		lines[1].position = sf::Vector2f(m_position.x + i * cellWidth, m_position.y + BoardSize * cellHeight);
		lines[2].position = sf::Vector2f(m_position.x, m_position.y + i * cellHeight);
		lines[3].position = sf::Vector2f(m_position.x + BoardSize * cellWidth, m_position.y + i * cellHeight);

		//NOTE check if works Color::Black 
		lines[0].color = sf::Color::Black;
		lines[1].color = sf::Color::Black;
		lines[2].color = ColorLines;
		lines[3].color = ColorLines;

		window.draw(lines);
	}
}

ClosedCell::ClosedCell(sf::RectangleShape cell, CellState cellState) : Cell(cell, cellState) {}

void ClosedCell::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	switch (m_cellState)
	{
		case vacant: drawVacantCell(target, m_cell); break;
		case gratis: drawGratisCell(target, m_cell); break;
		case alive: drawGratisCell(target, m_cell); break;
		case wounded: drawWoundedCell(target, m_cell); break;
		case killed: drawKilledCell(target, m_cell); break;
		case notAimed: drawNotAimedCell(target, m_cell); break;
		default: throw "CellState not from CellStates"; //TODO return
	}
}

OpenCell::OpenCell(sf::RectangleShape  cell, CellState  cellState) : Cell(cell, cellState) {}

void OpenCell::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	switch (m_cellState)
	{
		case vacant: drawVacantCell(target, m_cell); break;
		case gratis: drawGratisCell(target, m_cell); break;
		case alive: drawAliveCell(target, m_cell); break;
		case wounded: drawWoundedCell(target, m_cell); break;
		case killed: drawKilledCell(target, m_cell); break;
		case notAimed: drawNotAimedCell(target, m_cell); break;
		default: throw "CellState not from CellStates";//TODO remove
	}
}

bool DrawingBoard::isPosOnScreenBoard(const sf::Vector2i &pt) const
{
	if (pt.x >= m_position.x && pt.x <= m_position.x + m_size.x && 
		pt.y >= m_position.y && pt.y <= m_position.y + m_size.y)
		return true;
	return false;
}

Pt DrawingBoard::getGameBoardPos(const sf::Vector2i &gameBoardPt) const
{
	float width = (float)m_size.x;
	float height = (float)m_size.y;
	float cellWidth = width / BoardSize;
	float cellHeight = height / BoardSize;

	int gameBoardPtX = gameBoardPt.x / (int)cellWidth;
	int gameBoardPtY = gameBoardPt.y / (int)cellHeight;
	assert(gameBoardPtX >= 0);
	assert(gameBoardPtX < BoardSize);
	assert(gameBoardPtY >= 0);
	if (gameBoardPtY >= BoardSize)
	{
		throw "line 176 drawing.cpp";
	}
	assert(gameBoardPtY < BoardSize);
	return Pt(gameBoardPtX, gameBoardPtY);
}

sf::Vector2f DrawingBoard::getPosition()
{
	return m_position;
}

void DrawingBoard::setPosition(sf::Vector2f position)
{
	m_position = position;
}

bool DrawingBoard::getIsOpen()
{
	return m_isOpen;
}

void DrawingBoard::setIsOpen(bool isOpen)
{
	m_isOpen = isOpen;
}

void DrawingBoard::setSize(sf::Vector2u size)
{
	m_size = size;
}

sf::Vector2u DrawingBoard::getSize()
{
	return m_size;
}

Cell::Cell(sf::RectangleShape cell, CellState cellState) : m_cell(cell), m_cellState(cellState) {}