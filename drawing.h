#pragma once

#include <SFML/Graphics.hpp>
#include "board.h"

void drawBackGround(sf::RenderWindow &window);

void drawVacantCell(sf::RenderWindow &window, sf::RectangleShape cell);
void drawAliveCell(sf::RenderWindow &window, sf::RectangleShape cell);
void drawWoundedCell(sf::RenderWindow &window, sf::RectangleShape cell);
void drawKilledCell(sf::RenderWindow &window, sf::RectangleShape cell);
void drawGratisCell(sf::RenderWindow &window, sf::RectangleShape &cell);
void drawNotAimedCell(sf::RenderWindow &window, sf::RectangleShape cell);

//TODO add blocking shooting at this board
class DrawingBoard : public Board, public sf::Drawable
{
protected:
	sf::Vector2f m_position;
	sf::Vector2u m_size;
	bool m_isOpen;

public:
	DrawingBoard();
	DrawingBoard(Board &board, const bool &isOpen, const sf::Vector2f &position, const sf::Vector2u &size);

	virtual void draw(sf::RenderTarget &window, sf::RenderStates states) const;

	bool isPosOnScreenBoard(const sf::Vector2i &pt) const;
	Pt getGameBoardPos(const sf::Vector2i &gameBoardPt) const;
	

	//getters and setters
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);
	bool getIsOpen();
	void setIsOpen(bool isOpen);
	void setSize(sf::Vector2u size);
	sf::Vector2u getSize();
};

class Cell : public sf::Drawable
{
protected:
	sf::RectangleShape m_cell;
	CellState m_cellState;

public:
	Cell(sf::RectangleShape cell, CellState cellState);
	virtual void draw(sf::RenderTarget &window, sf::RenderStates states) const = 0;
};

class ClosedCell : public Cell
{
public:
	ClosedCell(sf::RectangleShape cell, CellState cellState);
	virtual void draw(sf::RenderTarget &window, sf::RenderStates states) const;
};

class OpenCell : public Cell
{
public:
	OpenCell(sf::RectangleShape cell, CellState cellState);
	virtual void draw(sf::RenderTarget &window, sf::RenderStates states) const;
};