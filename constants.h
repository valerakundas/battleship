#pragma once

#include "point.h"

//Y represents sth strange
//delete vacant and set all error cells as gratis
enum CellState { vacant = 'Y', gratis = ' ', alive = 'O', wounded = 'x', killed = 'X', notAimed = '.' }; 
enum GameResult { draw, opWin, meWin, none };

static const int BoardSize = 10;
static const int DX[4] = { 0, -1, 0, 1 }; //down, left, up, right
static const int DY[4] = { 1, 0, -1, 0 }; //down, left, up, right

static const int WindowWidth = 800;
static const int WindowHeight = 500;
static const sf::Time PauseDuration = sf::milliseconds(100);

static const sf::Color ColorVacantCell = sf::Color(0, 0, 0); //black
static const sf::Color ColorAliveShipsCell = sf::Color(255, 255, 0); //yellow
static const sf::Color ColorKilledShipsCell = sf::Color(123, 104, 238); //purple
static const sf::Color ColorWoundedShipsCell = sf::Color(0, 255, 0); //green
static const sf::Color ColorGratisCell = sf::Color(255, 255, 255); //white
static const sf::Color ColorBackGround = sf::Color(255, 255, 255); //white
static const sf::Color ColorError = sf::Color(0, 0, 0); //black
static const sf::Color ColorLines = sf::Color(128, 0, 128); //purple

typedef std::vector<std::vector<CellState>> TBoard;
typedef std::vector<Pt> TShip;

//NOTE change Color(a,b,c) to Color::Yellow and find out why it's not working
