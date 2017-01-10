#pragma once

#include <SFML\Graphics.hpp>

#include "drawing.h"

void showMessage(sf::RenderTarget &target, const std::string msg);

void closeGame(sf::RenderWindow &window, DrawingBoard &me, DrawingBoard &op);

void commitOpsMove(DrawingBoard &me);

void init(DrawingBoard &me, DrawingBoard &op);

GameResult getGameResult(DrawingBoard &me, DrawingBoard &op);

std::string getStringGameResult(const GameResult &gameResult);

bool isLeftMouseButtonPressed(sf::Event event);

void drawGameState(sf::RenderWindow &window, DrawingBoard &me, DrawingBoard &op);

void drawGameOverState(sf::RenderWindow &window, GameResult gameResult);