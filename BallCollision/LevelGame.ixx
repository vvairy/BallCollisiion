#include "SFML/Graphics.hpp"
export module LevelGame;
import GridManager;
class Ball; //imported in GridManager module

export class LevelGame
{
public:
	LevelGame(std::shared_ptr<sf::RenderWindow> window_ptr,
		std::vector<std::shared_ptr<Ball>>& balls)
		: window(window_ptr),
		  gridManager(balls),
		  balls(balls)
	{}

	void update(float& dt)
	{
		gridManager.update(dt);

		window->clear();
		for (const auto& ball : balls)
			window->draw(ball->gball);
		window->display();
	}

private:
	GridManager gridManager;
	std::vector<std::shared_ptr<Ball>> balls;
	std::shared_ptr<sf::RenderWindow> window;
};