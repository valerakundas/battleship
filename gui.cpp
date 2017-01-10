#include "gui.h"

#include <deque>

using namespace std;

deque<Pt> opMovesBuffer;

void commitOpsMove(DrawingBoard &me)
{
    bool shotMade = false;
    bool success;
    while (!shotMade)
        for (int i = 0; i < BoardSize; ++i)
            for (int j = 0; j < BoardSize; ++j)
            {
                int cellState = me.getCellState(Pt(i, j));
                if (cellState == CellState::wounded)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        Pt shot = Pt(i + DX[k], j + DY[k]);
                        if (me.getCellState(shot) == CellState::alive || me.getCellState(shot) == CellState::gratis)
                            shotMade = me.makeShot(shot, success);
                    }
                }
            }
    if (shotMade)
        return;
    while (!shotMade)
    {
        Pt shot = me.getFreeRandPoint();
        shotMade = me.makeShot(shot, success);
    }
}

void init(DrawingBoard &me, DrawingBoard &op)
{
    sf::Vector2f position(0.05 * WindowWidth / 2.0, 0.05 * WindowHeight / 2.0);
    sf::Vector2u size(int(WindowWidth / 2.0 * 0.9), int(WindowHeight / 2.0 * 0.9));

    me.fillRandom();
    me.setIsOpen(true);
    me.setPosition(position);
    me.setSize(size);

    position.x += float(WindowWidth / 2.0);
    op.fillRandom();
    op.setIsOpen(false);
    op.setPosition(position);
    op.setSize(size);
}

void closeGame(sf::RenderWindow &window, DrawingBoard &me, DrawingBoard &op)
{
    //TODO are you sure you want to exit game?
    window.close();
}

GameResult getGameResult(DrawingBoard &me, DrawingBoard &op)
{
    if (me.getShipsLeft() != 0 && op.getShipsLeft() != 0)
        return GameResult::none;
    if (me.getShipsLeft() != 0)
        return GameResult::meWin;
    if (op.getShipsLeft() != 0)
        return GameResult::opWin;
    return GameResult::draw;
}

void drawGameState(sf::RenderWindow &window, DrawingBoard &me, DrawingBoard &op)
{
    window.clear();
    drawBackGround(window);
    window.draw(me);
    window.draw(op);
    window.display();
    sf::sleep(PauseDuration);
}

void drawGameOverState(sf::RenderWindow &window, GameResult gameResult)
{
    window.clear();
    showMessage(window, getStringGameResult(gameResult));
    window.display();
}

void showMessage(sf::RenderTarget &target, const std::string msg)
{
    sf::Font font;
    if (!font.loadFromFile("font.ttf"))
        throw "font is not loaded";

    sf::Text text;
    text.setString(msg);
    text.setFont(font);
    text.setCharacterSize(60);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
    text.setPosition(WindowWidth / 2, WindowHeight / 2);
    target.draw(text);
}


std::string getStringGameResult(const GameResult &gameResult)
{
    //enum GameResult { draw, opWin, meWin, none };
    switch (gameResult)
    {
        case none:
            return "none";
        case opWin:
            return "opWin";
        case meWin:
            return "meWin";
        case draw:
            return "draw";
    }
    return "none";
}

bool isLeftMouseButtonPressed(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
        return true;
    return false;
}