#include "gui.h"
#include "board.h"

int WinMain()
{
	try
	{
		srand((int)time(0));
		DrawingBoard me, op;
		init(me, op);

		bool gameOver = false;
		GameResult gameResult = none;

		sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "battleship by Valerik");

		if (rand() % 2)
			commitOpsMove(me);
		drawGameState(window, me, op);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					closeGame(window, me, op);
				else
				{
					if (gameOver)
					{
						gameOver = true;
						gameResult = getGameResult(me, op);
					}
					else
					{
						if (isLeftMouseButtonPressed(event))
						{
							sf::Vector2i mousePosOnWindow(event.mouseButton.x, event.mouseButton.y);
							if (op.isPosOnScreenBoard(mousePosOnWindow))
							{
								//my move 
								sf::Vector2i mousePosOnScreenBoard(mousePosOnWindow.x - (int)op.getPosition().x,
													     mousePosOnWindow.y - (int)op.getPosition().y);
								Pt gameBoardPos = op.getGameBoardPos(mousePosOnScreenBoard);
								bool success = false;
								if (!op.makeShot(gameBoardPos, success))
									continue;
								if (success)
									continue;

								//op's move
								commitOpsMove(me);
							}
						}
					}
				}
			}

			if (gameOver)
				drawGameOverState(window, gameResult);
			else
				drawGameState(window, me, op);
		}
	}
	catch (char *msg)
	{
		std::cerr << msg << "\n";
	}
	return 0;
}